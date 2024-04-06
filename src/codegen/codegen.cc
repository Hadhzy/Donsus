/*
 Codegen for the DONSUS COMPILER
Todo:
- build vtable: https://itanium-cxx-abi.github.io/cxx-abi/abi.html#vtable
- visit each of the ast nodes
- optimisation is not needed as the IR builder already constant one
- IRBuilder
- assign them in symbol table
 // signed support
 // unsigned support
 */
#include "../../Include/codegen/codegen.h"

namespace DonsusCodegen {
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
    return visit(n->get<donsus_ast::expression>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
    return visit(n->get<donsus_ast::function_call>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT: {
    return visit(n->get<donsus_ast::else_statement>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT: {
    return visit(n->get<donsus_ast::return_kw>(), table);
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

  Builder->SetInsertPoint(entry);
}

llvm::Value *DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                                        donsus_ast::variable_decl &ca_ast,
                                        utility::handle<DonsusSymTable> &table,
                                        bool is_definition) {
  auto type = ca_ast.identifier_type;
  auto name = ca_ast.identifier_name;

  // variable definition
  if (is_definition) {
    // CreateAlloca (Type *Ty, Value *ArraySize=nullptr, const Twine &Name="")
    llvm::AllocaInst *Allocadef =
        Builder->CreateAlloca(map_type(make_type(type)), 0, name);
    DonsusSymTable::sym sym_def = table->get(name);
    sym_def.inst = Allocadef;
    llvm::Value *def_value = compile(ast->children[0], table);
    Builder->CreateStore(def_value, Allocadef);
    llvm::Value *CurVardef =
        Builder->CreateLoad(Allocadef->getAllocatedType(), Allocadef, name);
    return CurVardef;
  }

  llvm::AllocaInst *Alloca =
      Builder->CreateAlloca(map_type(make_type(type)), 0, name);

  DonsusSymTable::sym sym1 = table->get(name);
  sym1.inst = Alloca;
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

  DonsusSymTable::sym sym1 = table->get(identifier_name);
  llvm::AllocaInst *A = sym1.inst;
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
    res = Builder->CreateUDiv(
        lhs_value, compile(ast->children[0], table)); // unsigned division?
    break;
  }
  default: {
  }
  }

  Builder->CreateStore(res, sym1.inst);
  return Builder->CreateLoad(A->getAllocatedType(), A, sym1.inst);
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
  // arbitrary precision integer, typically common unsigned ints
  // context_, llvm:PInt(32, int_expr.GetValAsInt(), true)));
  // only works with scalar integers, constant propagation is needed
  // constant propagate the operands into an unsigned int
  int value = ast->constant_propagation(ast);

  return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, value, false));
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::identifier &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_decl &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_def &ast,
                           utility::handle<DonsusSymTable> &table) {
  /*
    std::vector<llvm::Type *> parameters =
        parameters_for_function(ast.parameters);
    auto function_name = ast.func_name;
    // multiple return type support here

    if (!ast.return_type.size()) {
      using return_type = map_type(make_type(ast.return_type));
    } else {
      // multiple return types
      llvm::StructType type = multiple_return_types(ast.return_type);
      using return_type = type;
    }

    llvm::FunctionType *FT =
        llvm::FunctionType::get(return_type), parameters, false);

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, function_name, *TheModule);

    // process body here before returning
    // multiple return types
    Builder->CreateRet(map_type(make_type(ast.return_type)));
  */
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_call &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::if_statement &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator ::visit(donsus_ast::else_statement &ast,
                            utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::return_kw &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::string_expr &ast,
                           utility::handle<DonsusSymTable> &table) {
  // first immutable
  // make a new global variable, i8*

  return Builder->CreateGlobalStringPtr(llvm::StringRef(ast.value.value));
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::expression &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::print_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  // figure out whether its lvalue, or rvalue
  // we would check for the string representation of an object
  // use external functions ,eg extern printf
  // if its lvalue, look it up in symbol table, otherwise just codegen
  // the expression
  auto CalleeF = TheModule->getOrInsertFunction(
      "printf",
      llvm::FunctionType::get(
          llvm::IntegerType::getInt32Ty(*TheContext),
          llvm::PointerType::get(llvm::Type::getInt8Ty(*TheContext), 0),
          true /* this is var arg func type*/));

  auto func = llvm::dyn_cast<llvm::Constant>(CalleeF.getCallee());

  // pass in args
  std::vector<llvm::Value *> Argsv;

  /*  return Builder->CreateCall(func, Argsv, "printfCall");*/
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::bool_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  if (ca_ast.value.value == "true")
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1, false));

  return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0, false));
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::unary_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  // negative number expression
  int value = ast->constant_propagation(ast->children[0]);

  if (ast->children[0]->type.type ==
      donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION) {
    return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, -value, true));
  }
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
