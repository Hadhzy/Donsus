/*
 Codegen for the DONSUS COMPILER
Todo:
- build vtable: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable
- visit each of the ast nodes
- optimisation is not needed as the IR builder already constant one
- IRBuilder
- assign them in symbol table

- global support for variables
- function call
- multiple return type
 - having the ability to print out multiple things
- printf
 */
#include "../../Include/codegen/codegen.h"

namespace DonsusCodegen {
// similar to donsus_sema_is_exist
auto is_global_sym(std::string &name, utility::handle<DonsusSymTable> table)
    -> bool {
  DonsusSymTable::sym result = table->get(name);
  if (result.key == "global." + result.short_name) {
    return true;
  }
  return false;
}

/*
 Get the symbol based on node's name, this will just simply
 call table->get().
 This function is used when passing in arguments for the print function.
 We need to identify the node type and based on its name get the sym.inst from
 the symbol table. This can be DONSUS_IDENTIFIER, DONSUS_EXPRESSION,
 DONSUS_FUNCTION_CALL, etc. printf(a) # DONSUS_IDENTIFIER printf(2+5) #
 DONSUS_EXPRESSION
 * */
auto sym_from_node(utility::handle<donsus_ast::node> &node,
                   utility::handle<DonsusSymTable> table)
    -> DonsusSymTable::sym {
  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER:
    return table->get(node->get<donsus_ast::identifier>().identifier_name);
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION:
    return table->get(node->get<donsus_ast::variable_decl>().identifier_name);
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION:
    return table->get(node->get<donsus_ast::variable_decl>().identifier_name);
  default: {
  }
  }
}

Bitness GetBitness() {
  const auto target_triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
  if (target_triple.isArch32Bit())
    return Bitness::x32;
  if (target_triple.isArch64Bit())
    return Bitness::x64;
  std::cerr << "Target triple: " << target_triple.normalize()
            << " is not supported because it is not 32bit or 64bit";
  abort();
}
void DonsusCodeGenerator::Finish() const {
  Builder->SetInsertPoint(main_block);
  Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
}

void DonsusCodeGenerator::create_entry_point() {
  // create an entry function which can be used in the first block
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
  std::string name = "main";

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, name, *TheModule);
}

int DonsusCodeGenerator::create_object_file() {
  // default_optimisation();
  const auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  TheModule->setTargetTriple(TargetTriple);

  std::string Error;
  const auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }

  const auto CPU = "generic";
  const auto Features = "";

  llvm::TargetOptions opt;
  auto TheTargetMachine = Target->createTargetMachine(
      TargetTriple, CPU, Features, opt, llvm::Reloc::PIC_);

  TheModule->setDataLayout(TheTargetMachine->createDataLayout());

  // Checks the module or validity.
  if (llvm::verifyModule(*TheModule, &llvm::errs())) {
    llvm::errs() << "Error: Module verification failed.\n";
    return 1;
  }

  auto Filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

  if (EC) {
    llvm::errs() << "Could not open file: " << EC.message();
    return 1;
  }

  llvm::legacy::PassManager pass;
  const auto FileType = llvm::CGFT_ObjectFile;

  if (TheTargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TheTargetMachine can't emit a file of this type";
    return 1;
  }

  pass.run(*TheModule);

  dest.flush();

  llvm::outs() << "Wrote " << Filename << "\n";
  return 0;
}

void DonsusCodeGenerator::default_optimisation() {
  // Create the analysis managers.
  // These must be declared in this order so that they are destroyed in the
  // correct order due to inter-analysis-manager references.

  llvm::LoopAnalysisManager LAM;
  llvm::FunctionAnalysisManager FAM;
  llvm::CGSCCAnalysisManager CGAM;
  llvm::ModuleAnalysisManager MAM;

  // Create the new pass manager builder.
  // Take a look at the PassBuilder constructor parameters for more
  // customization, e.g. specifying a TargetMachine or various debugging
  // options.
  llvm::PassBuilder PB;

  // Register all the basic analyses with the managers.
  PB.registerModuleAnalyses(MAM);
  PB.registerCGSCCAnalyses(CGAM);
  PB.registerFunctionAnalyses(FAM);
  PB.registerLoopAnalyses(LAM);
  PB.crossRegisterProxies(LAM, FAM, CGAM, MAM);

  // Create the pass manager.
  // This one corresponds to a typical -O2 optimization pipeline.
  llvm::ModulePassManager MPM =
      PB.buildPerModuleDefaultPipeline(llvm::OptimizationLevel::O2);

  // optimise IR
  MPM.run(*TheModule, MAM);
}

void DonsusCodeGenerator::Link() const {
  // Link
  std::vector<std::filesystem::path> obj_paths = {"output.o"};
  // hard code it into linux
  std::filesystem::path exe_path = "test";

  std::string linker_cmd;
  linker_cmd = platform.GetLinkerCommand(obj_paths, exe_path, GetBitness());
  std::cout << "command: " << linker_cmd;
  system(linker_cmd.c_str());
}
llvm::Value *
DonsusCodeGenerator::compile(utility::handle<donsus_ast::node> &n,
                             utility::handle<DonsusSymTable> &table) {
  /*
  we either pass in the actual ast structure or the high level node including
  children
   * */
  switch (n->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION: {
    return visit(n, n->get<donsus_ast::variable_decl>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION: {
    return visit(n, n->get<donsus_ast::variable_decl>(), table, true);
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL: {
    return visit(n->get<donsus_ast::function_decl>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    return visit(n->get<donsus_ast::function_def>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
    return visit(n->get<donsus_ast::if_statement>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT: {
    return visit(n, n->get<donsus_ast::assignment>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER: {
    return visit(n->get<donsus_ast::identifier>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION: {
    return visit(n, n->get<donsus_ast::number_expr>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
    return visit(n, n->get<donsus_ast::expression>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
    return visit(n->get<donsus_ast::function_call>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT: {
    return visit(n->get<donsus_ast::else_statement>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    return visit(n, n->get<donsus_ast::return_kw>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION: {
    return visit(n->get<donsus_ast::string_expr>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION: {
    return visit(n, n->get<donsus_ast::print_expr>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION: {
    return visit(n, n->get<donsus_ast::bool_expr>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION: {
    return visit(n, n->get<donsus_ast::unary_expr>(), table);
  }
  }
}

DonsusCodeGenerator::DonsusCodeGenerator(
    std::unique_ptr<llvm::LLVMContext> context,
    std::unique_ptr<llvm::Module> module,
    std::unique_ptr<llvm::IRBuilder<>> builder)
    : TheContext(std::move(context)), TheModule(std::move(module)),
      Builder(std::move(builder)) {

  create_entry_point();
  llvm::Function *TheFunction = TheModule->getFunction("main");

  // // assert here

  llvm::BasicBlock *entry =
      llvm::BasicBlock::Create(*TheContext, "entry_point", TheFunction);
  main_block = entry;
  Builder->SetInsertPoint(entry);
}

llvm::Value *DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                                        donsus_ast::variable_decl &ca_ast,
                                        utility::handle<DonsusSymTable> &table,
                                        bool is_definition) {
  auto type = ca_ast.identifier_type;
  auto name = ca_ast.identifier_name;

  // instead of putting them in the main, they should be created globals
  if (is_global_sym(name, table)) {
    // it must have an initial value thus we can't do it with declarations
    llvm::Constant *initial_value;
    if (is_definition) {
      initial_value =
          llvm::dyn_cast<llvm::Constant>(compile(ast->children[0], table));
    } else {
      initial_value = nullptr; // this will be deduced to 'external'
    }

    llvm::GlobalVariable *c = new llvm::GlobalVariable(
        *TheModule, map_type(make_type(type)), false,
        llvm::GlobalValue::LinkageTypes::ExternalLinkage, initial_value, name);
    table->setInst(name, c);
    Builder->SetInsertPoint(main_block);
    return c;
  }

  // variable definition
  if (is_definition) {
    // CreateAlloca (Type *Ty, Value *ArraySize=nullptr, const Twine &Name="")
    llvm::AllocaInst *Allocadef =
        Builder->CreateAlloca(map_type(make_type(type)), 0, name);
    table->setInst(name, Allocadef);
    llvm::Value *def_value = compile(ast->children[0], table);
    Builder->CreateStore(def_value, Allocadef);
    llvm::Value *CurVardef =
        Builder->CreateLoad(Allocadef->getAllocatedType(), Allocadef, name);
    return CurVardef;
  }

  llvm::AllocaInst *Alloca =
      Builder->CreateAlloca(map_type(make_type(type)), 0, name);

  table->setInst(name, Alloca);
  llvm::Value *decl_value =
      llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0));
  Builder->CreateStore(decl_value, Alloca);
  llvm::Value *CurVardef =
      Builder->CreateLoad(Alloca->getAllocatedType(), Alloca, name);
  return CurVardef;
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::assignment &ac_ast,
                           utility::handle<DonsusSymTable> &table) {
  llvm::Value *res;
  auto identifier_name = ac_ast.identifier_name;
  auto op = ac_ast.op;

  if (is_global_sym(identifier_name, table)) {
    Builder->SetInsertPoint(main_block);
  }

  DonsusSymTable::sym sym1 = table->get(identifier_name);
  llvm::AllocaInst *A = llvm::dyn_cast<llvm::AllocaInst>(sym1.inst);
  llvm::Value *lhs_value =
      Builder->CreateLoad(A->getAllocatedType(), A, sym1.short_name);
  switch (op.kind) {
  case donsus_token_kind::DONSUS_PLUS_EQUAL: {
    res = Builder->CreateAdd(lhs_value, compile(ast->children[0], table));
    break;
  }
  case donsus_token_kind::DONSUS_MINUS_EQUAL: {
    res = Builder->CreateSub(lhs_value, compile(ast->children[0], table));
    break;
  }
  case donsus_token_kind::DONSUS_STAR_EQUAL: {
    res = Builder->CreateMul(lhs_value, compile(ast->children[0], table));
    break;
  }
  case donsus_token_kind::DONSUS_SLASH_EQUAL: {
    res = Builder->CreateSDiv(lhs_value, compile(ast->children[0], table));
    break;
  }
  case donsus_token_kind::DONSUS_EQUAL: {
    res = compile(ast->children[0], table);
    break;
  }
  default: {
  }
  }

  Builder->CreateStore(res, sym1.inst);
  return Builder->CreateLoad(A->getAllocatedType(), sym1.inst, identifier_name);
}

/*
 Returns back a vector with llvm TYPE based on DONSUS_TYPE for parameters
 * */
/*// Todo: Avoid unnecessary copies
std::vector<llvm::Type *> DonsusCodeGenerator::parameters_for_function(
    std::vector<NAME_DATA_PAIR> parameters) {
  std::vector<llvm::Type *> parameters_vector;
  for (auto n : parameters) {
    parameters_vector.push_back(map_type(n.type));
  }
  return parameters_vector;
}*/
llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::number_expr &ac_ast,
                           utility::handle<DonsusSymTable> &table) {
  return llvm::ConstantInt::get(
      *TheContext,
      llvm::APInt(32,
                  std::stoi(ast->get<donsus_ast::number_expr>().value.value),
                  false));
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::identifier &ast,
                           utility::handle<DonsusSymTable> &table) {
  DonsusSymTable::sym symbol = table->get(ast.identifier_name);
  return Builder->CreateLoad(map_type(symbol.type), symbol.inst,
                             ast.identifier_name);
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_decl &ast,
                           utility::handle<DonsusSymTable> &table) {
  llvm::FunctionType *FT;
  if (ast.return_type.size() > 1) {
    // handle multiple parameters here
    // setup the return type to a struct here
    FT = llvm::FunctionType::get(map_type(ast.return_type[0]), false);
  } else {
    FT = llvm::FunctionType::get(map_type(ast.return_type[0]), false);
  }

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, ast.func_name, *TheModule);

  table->setInst(ast.func_name, F);
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_def &ast,
                           utility::handle<DonsusSymTable> &table) {
  llvm::FunctionType *FT;
  if (ast.return_type.size() > 1) {
    // handle multiple parameters here
    // setup teh return type to a struct here
    FT = llvm::FunctionType::get(map_type(ast.return_type[0]), false);
  } else {
    FT = llvm::FunctionType::get(map_type(ast.return_type[0]), false);
  }

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, ast.func_name, *TheModule);

  table->setInst(ast.func_name, F);

  // construct a basic block before adding body
  llvm::BasicBlock *block =
      llvm::BasicBlock::Create(*TheContext, ast.func_name + "_block", F);
  Builder->SetInsertPoint(block);

  // setup the struct members
  for (auto node : ast.body) {
    compile(node, table);
  }
  /*  // setup parameters
    for (auto node : ast.parameters) {
      // compile them down
    }*/
  // use main block again
  Builder->SetInsertPoint(main_block);
  return F;
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_call &ast,
                           utility::handle<DonsusSymTable> &table) {
  DonsusSymTable::sym sym = table->get(ast.func_name);
  std::vector<llvm::Value *> args;

  // just call the function and save it
  // llvm::CallInst *call =
  // Builder->CreateCall(llvm::dyn_cast<llvm::FunctionType>(sym.inst), args);
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::if_statement &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator ::visit(donsus_ast::else_statement &ast,
                            utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::return_kw &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  // handle multiple parameters here
  // if its a multiple type just return back the structure
  return Builder->CreateRet(compile(ast->children[0], table));
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::string_expr &ast,
                           utility::handle<DonsusSymTable> &table) {

  return Builder->CreateGlobalStringPtr(llvm::StringRef(ast.value.value));
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::expression &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  switch (ca_ast.value.kind) {
  case DONSUS_PLUS: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      return Builder->CreateAdd(compile(*it, table), compile(*(it + 1), table));
    }
  }
  case DONSUS_MINUS: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      return Builder->CreateSub(compile(*it, table), compile(*(it + 1), table));
    }
  }
  case DONSUS_SLASH: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      return Builder->CreateSDiv(compile(*it, table),
                                 compile(*(it + 1), table));
    }
  }
  case DONSUS_STAR: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      return Builder->CreateMul(compile(*it, table), compile(*(it + 1), table));
    }
  }
  default: {
    return nullptr;
  }
  }
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::print_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  // figure out whether its lvalue, or rvalue
  // we would check for the string representation of an object
  // use external functions ,eg extern printf
  // if its lvalue, look it up in symbol table, otherwise just codegen
  // the expression
  std::vector<llvm::Type *> printf_arg_types;
  printf_arg_types.push_back(Builder->getPtrTy());

  llvm::FunctionType *FT1 = llvm::FunctionType::get(
      llvm::Type::getInt32Ty(*TheContext), printf_arg_types, true);

  llvm::Function *func;
  if (TheModule->getFunction("printf") == nullptr) {
    func = llvm::Function::Create(FT1, llvm::Function::ExternalLinkage,
                                  llvm::Twine("printf"), TheModule.get());
  } else {
    func = TheModule->getFunction("printf");
  }

  // pass in args
  std::vector<llvm::Value *> Argsv;
  // format

  for (auto node : ast->children) {
    DonsusSymTable::sym sym = sym_from_node(node, table);
    Argsv.push_back(printf_format(node, sym.key));
    Argsv.push_back(Builder->CreateLoad(map_type(sym.type), sym.inst));
  }

  return Builder->CreateCall(func, Argsv, "printfCall");
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::bool_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  if (ca_ast.value.value == "true")
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(8, 1, false));

  return llvm::ConstantInt::get(*TheContext, llvm::APInt(8, 0, false));
}

// for now it only supports one kind of type, e.g
// %d, %s
llvm::Value *
DonsusCodeGenerator::printf_format(utility::handle<donsus_ast::node> node,
                                   std::string name) {
  switch (node->real_type.type_un) {
  case DONSUS_TYPE::TYPE_BASIC_INT:
  case DONSUS_TYPE::TYPE_I32:
  case DONSUS_TYPE::TYPE_U64:
  case DONSUS_TYPE::TYPE_I8:
  case DONSUS_TYPE::TYPE_I64:
  case DONSUS_TYPE::TYPE_I16:
  case DONSUS_TYPE::TYPE_U32: {
    auto format_name = name + "_for_printf_string";
    return Builder->CreateGlobalString("%d", format_name);
  }
  case DONSUS_TYPE::TYPE_STRING: {
    auto format_name = name + "_for_printf_string";
    return Builder->CreateGlobalString("%s", format_name);
  }
  default: {
  }
  }
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::unary_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  /*  // negative number expression
    if (ast->children[0]->type.type ==
        donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION) {
      return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, -value, true));
    }*/
}
/*Maps DONSUS_TYPE to llvm TYPE.
 **/
llvm::Type *DonsusCodegen::DonsusCodeGenerator::map_type(DONSUS_TYPE type) {
  switch (type.type_un) {
  case DONSUS_TYPE::TYPE_I8: {
    return Builder->getInt8Ty();
  }

  case DONSUS_TYPE::TYPE_I16: {
    return Builder->getInt16Ty();
  }

  case DONSUS_TYPE::TYPE_BASIC_INT: {
    return Builder->getInt32Ty();
  }

  case DONSUS_TYPE::TYPE_I32: {
    return Builder->getInt32Ty();
  }

  case DONSUS_TYPE::TYPE_I64: {
    return Builder->getInt64Ty();
  }

  case DONSUS_TYPE::TYPE_U32: {
    return Builder->getInt32Ty();
  }

  case DONSUS_TYPE::TYPE_U64: {
    return Builder->getInt64Ty();
  }
  case DONSUS_TYPE::TYPE_STRING: {
    return Builder->getPtrTy();
  }
  case DONSUS_TYPE::TYPE_BOOL: {
    return Builder->getInt8Ty();
  }
  case DONSUS_TYPE::TYPE_VOID: {
    return Builder->getVoidTy();
  }
  default: {
  }
  }
  return nullptr;
}
} // namespace DonsusCodegen
