//===----------------------------------------------------------------------===//
//
//  This file implements code generation for the Donsus compiler.
//  The Codegen is done with LLVM.
//
//===----------------------------------------------------------------------===//

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
 Return whether the node is an r-value expression
 or a lvalue variable, for printf.
 * */
auto is_expression(utility::handle<donsus_ast::node> node) -> bool {
  switch (node->type.type) {
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION:
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DEFINITION:
  case donsus_ast::donsus_node_type::DONSUS_IDENTIFIER:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DECL:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF:
  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL:
  case donsus_ast::donsus_node_type::DONSUS_ELSE_STATEMENT:
  case donsus_ast::donsus_node_type::DONSUS_RETURN_STATEMENT:
    return false;
  case donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_STRING_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_ASSIGNMENT:
  case donsus_ast::donsus_node_type::DONSUS_BOOL_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_PRINT_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_UNARY_EXPRESSION:
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS:
    return true;
  default: {
  }
  }
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
  case donsus_ast::donsus_node_type::DONSUS_VARIABLE_DECLARATION:
    return table->get(node->get<donsus_ast::variable_decl>().identifier_name);
  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_ARG:
    return table->get(node->get<donsus_ast::variable_decl>().identifier_name);
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS: {
    return table->get(node->get<donsus_ast::array_access>().identifier_name);
  }
  default: {
    throw std::runtime_error("Does not work with expressions");
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
  Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
}

void DonsusCodeGenerator::create_entry_point() {
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
  std::string name = "main";

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, name, *TheModule);

  main_func = F;
}

int DonsusCodeGenerator::create_object_file() {
#if !DEBUG
  default_optimisation();
#endif
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

  if (llvm::verifyModule(*TheModule, &llvm::errs())) {
    llvm::errs() << "Error: Module verification failed.\n";
    return 1;
  }

#if defined(DU_SYSTEMS_WINDOWS)
  auto Filename = "output.obj";
#else if defined(DU_SYSTEMS_UNIX)
  auto Filename = "output.o";
#endif
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
#if DEBUG
  llvm::outs() << "Wrote " << Filename << "\n";
#endif
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
  std::vector<std::filesystem::path> obj_paths = {"output"};
  std::filesystem::path exe_path = "test";

  std::string linker_cmd;
  linker_cmd = platform.GetLinkerCommand(obj_paths, exe_path, GetBitness());
#if DEBUG
  std::cout << "command: " << linker_cmd;
#endif
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

  case donsus_ast::donsus_node_type::DONSUS_FOR_LOOP: {
    return visit(n->get<donsus_ast::for_loop>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_DEF: {
    return visit(n->get<donsus_ast::function_def>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_IF_STATEMENT: {
    return visit(n->get<donsus_ast::if_statement>(), n, table);
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
  case donsus_ast::donsus_node_type::DONSUS_FLOAT_EXPRESSION:
    return visit(n, n->get<donsus_ast::float_expr>(), table);

  case donsus_ast::donsus_node_type::DONSUS_EXPRESSION: {
    return visit(n, n->get<donsus_ast::expression>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_FUNCTION_CALL: {
    return visit(n->get<donsus_ast::function_call>(), table);
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

  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DECLARATION: {
    return visit(n, n->get<donsus_ast::array_decl>(), table);
  }

  case donsus_ast::donsus_node_type::DONSUS_ARRAY_DEFINITION: {
    return visit(n, n->get<donsus_ast::array_def>(), table);
  }
  case donsus_ast::donsus_node_type::DONSUS_ARRAY_ACCESS: {
    return visit(n, n->get<donsus_ast::array_access>(), table);
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
    llvm::Constant *initial_value;
    if (is_definition) {
      initial_value = llvm::Constant::getNullValue(map_type(make_type(type)));
    } else {
      initial_value = llvm::Constant::getNullValue(map_type(make_type(type)));
    }

    auto *c = new llvm::GlobalVariable(
        *TheModule, map_type(make_type(type)), false,
        llvm::GlobalValue::LinkageTypes::ExternalLinkage, initial_value, name);
    if (is_definition) {
      // FROM UNKNOWN TO DESIRED TYPE
      auto result = compile(ast->children[0], table);
      if (result->getType() != map_type(make_type(type))) {
        // if cast is needed, as of now its always needed if the
        // type is not one of the integer types
        // convert from f32 to f64
        if (make_type(type).type_un == DONSUS_TYPE::TYPE_F64) {
          llvm::Value *f64_g_v =
              Builder->CreateFPExt(result, Builder->getDoubleTy());
          Builder->CreateStore(f64_g_v, c);
        }
      } else {
        Builder->CreateStore(result, c);
      }
    }

    table->setInst(name, c);
    Builder->SetInsertPoint(main_block);
    return c;
  }

  // variable definition
  if (is_definition) {
    // FROM UNKNOWN TO DESIRED TYPE
    llvm::AllocaInst *Allocadef =
        Builder->CreateAlloca(map_type(make_type(type)), nullptr, name);
    table->setInst(name, Allocadef);
    llvm::Value *def_value = compile(ast->children[0], table);
    Builder->CreateStore(def_value, Allocadef);
    llvm::Value *CurVardef =
        Builder->CreateLoad(Allocadef->getAllocatedType(), Allocadef);
    return CurVardef;
  }

  llvm::AllocaInst *Alloca =
      Builder->CreateAlloca(map_type(make_type(type)), nullptr, name);

  table->setInst(name, Alloca);
  llvm::Value *decl_value = llvm::ConstantInt::get(
      *TheContext, llvm::APInt(map_bitwidth(make_type(type)), 0));
  Builder->CreateStore(decl_value, Alloca);
  llvm::Value *CurVardef =
      Builder->CreateLoad(Alloca->getAllocatedType(), Alloca);
  return CurVardef;
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::assignment &ac_ast,
                           utility::handle<DonsusSymTable> &table) {
  // array[1] = 3;

  llvm::Value *res;
  auto identifier_name = ac_ast.identifier_name;
  auto op = ac_ast.op;

  if (is_global_sym(identifier_name, table)) {
    Builder->SetInsertPoint(main_block);
  }

  DonsusSymTable::sym sym1 = table->get(identifier_name);
  llvm::Value *lhs_value;
  // should not be true
  if (sym1.array.array_type) {
    lhs_value = Builder->CreateLoad(
        sym1.array.array_type, compile(ac_ast.lvalue, table), sym1.short_name);
  } else {
    lhs_value =
        Builder->CreateLoad(sym1.inst->getType(), sym1.inst, sym1.short_name);
  }

  switch (op.kind) {
  case donsus_token_kind::DONSUS_PLUS_EQUAL: {
    res = Builder->CreateAdd(lhs_value, compile(ac_ast.rvalue, table));
    break;
  }
  case donsus_token_kind::DONSUS_MINUS_EQUAL: {
    res = Builder->CreateSub(lhs_value, compile(ac_ast.rvalue, table));
    break;
  }
  case donsus_token_kind::DONSUS_STAR_EQUAL: {
    res = Builder->CreateMul(lhs_value, compile(ac_ast.rvalue, table));
    break;
  }
  case donsus_token_kind::DONSUS_SLASH_EQUAL: {
    res = Builder->CreateSDiv(lhs_value, compile(ac_ast.rvalue, table));
    break;
  }
  case donsus_token_kind::DONSUS_EQUAL: {
    res = compile(ac_ast.rvalue, table);
    break;
  }
  default: {
  }
  }

  Builder->CreateStore(res, sym1.inst);
  return Builder->CreateLoad(sym1.inst->getType(), sym1.inst, identifier_name);
}

/*
 Returns back a vector with llvm TYPE based on DONSUS_TYPE for parameters
*/
/* Todo: Avoid unnecessary copies
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
  // here we would need to cast
  return llvm::ConstantInt::get(
      *TheContext,
      llvm::APInt(map_bitwidth(ast->real_type),
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
  std::vector<llvm::Type *> arg_types;
  for (auto node : ast.parameters) {
    arg_types.push_back(map_type(node->real_type));
  }

  if (ast.return_type.size() > 1) {
    // handle multiple parameters here
    std::vector<llvm::Type *> types_in_struct;
    for (DONSUS_TYPE type : ast.return_type) {
      types_in_struct.push_back(map_type(type));
    }
    auto struct_for_func = llvm::StructType::create(*TheContext);
    struct_for_func->setBody(types_in_struct);
    FT = llvm::FunctionType::get(struct_for_func, arg_types, false);
  } else {
    FT =
        llvm::FunctionType::get(map_type(ast.return_type[0]), arg_types, false);
  }

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, ast.func_name, *TheModule);
  llvm::verifyFunction(*F);
  table->setInst(ast.func_name, F);
  return F;
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_def &ast,
                           utility::handle<DonsusSymTable> &table) {
  llvm::FunctionType *FT;
  std::vector<llvm::Type *> arg_types;
  std::vector<llvm::Value *> allocated_insts; // allocadef(s) of local vars
  for (auto node : ast.parameters) {
    arg_types.push_back(map_type(node->real_type));
  }

  if (ast.return_type.size() > 1) {
    // handle multiple parameters here
    auto struct_for_func = llvm::StructType::create(*TheContext);
    llvm::AllocaInst *inst = Builder->CreateAlloca(struct_for_func);

    table->multiple_return.type_of_struct = struct_for_func;
    table->multiple_return.inst_of_struct = inst;

    // This is not correct, since arguments have zero correlation with what
    // we return
    struct_for_func->setBody(arg_types);
    FT = llvm::FunctionType::get(struct_for_func, arg_types, false);
  } else {
    FT =
        llvm::FunctionType::get(map_type(ast.return_type[0]), arg_types, false);
  }

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, ast.func_name, *TheModule);
  table->inst = F;

  // construct a basic block before adding body
  llvm::BasicBlock *block =
      llvm::BasicBlock::Create(*TheContext, ast.func_name + "_block", F);
  Builder->SetInsertPoint(block);

  for (auto node : ast.parameters) {
    llvm::AllocaInst *allocaInst =
        Builder->CreateAlloca(map_type(node->real_type));

    // we could just simply assume its variable declaration all the time
    DonsusSymTable::sym symbol = sym_from_node(node, table);
    table->setInst(symbol.short_name, allocaInst);

    allocated_insts.push_back(allocaInst);
  }
  // store values into local stack
  auto argIterator = F->arg_begin();
  for (auto &allocaInst : allocated_insts) {
    llvm::Value *argValue = &(*argIterator);
    Builder->CreateStore(argValue, allocaInst);
    // load maybe?
    Builder->CreateLoad(allocaInst->getType(), allocaInst);
    ++argIterator;
  }

  // setup the struct members

  // body of the function
  for (auto node : ast.body) {
    compile(node, table);
  }
  // insert unreachable instruction
  // if the merge block is empty
  // if the merge block does not have end terminator
  for (llvm::BasicBlock &BB : *F) {
    if ((BB.getName() == "merge" && BB.empty()) ||
        (BB.getName() == "merge" && !BB.getTerminator())) {
      Builder->SetInsertPoint(&BB);
      Builder->CreateUnreachable();
    }
  }
  // handle void here
  if (ast.return_type[0].type_un == DONSUS_TYPE::TYPE_VOID) {

    Builder->CreateRetVoid();
  }
  // use main block again
  Builder->SetInsertPoint(main_block);
  return F;
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_call &ast,
                           utility::handle<DonsusSymTable> &table) {

  utility::handle<DonsusSymTable> sym_table =
      table->get_sym_table_from_unqualified(
          ast.func_name); // the funcion's table

  std::vector<llvm::Value *> args;
  // llvm::CallInst *call =
  for (auto node : ast.arguments) {
    if (is_expression(node)) {
      args.push_back(compile(node, table));
    } else {
      DonsusSymTable::sym sym_local = sym_from_node(node, table);
      args.push_back(sym_local.inst);
    }
  }
  llvm::CallInst *call = Builder->CreateCall(
      llvm::dyn_cast<llvm::Function>(sym_table->inst), args);
  return call;
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::if_statement &ac_ast,
                           utility::handle<donsus_ast::node> &ast,
                           utility::handle<DonsusSymTable> &table) {
  // Visit the condition expression
  llvm::Value *conditionValue = compile(ast->children[0], table);

  // for true and false
  if (conditionValue->getType()->isIntegerTy(8)) {
    conditionValue = Builder->CreateICmpNE(
        conditionValue, llvm::ConstantInt::get(*TheContext, llvm::APInt(8, 0)),
        "conditionAsI1");
  }

  // gets teh current function object that is being built.
  // It gets this by asking the builder for the current BasicBlock, anda
  // asking that block for its parent(the function it is currently embedded
  // into).
  llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();

  llvm::BasicBlock *if_block =
      llvm::BasicBlock::Create(*TheContext, "then", TheFunction);
  llvm::BasicBlock *elseBlock =
      llvm::BasicBlock::Create(*TheContext, "else", TheFunction);
  llvm::BasicBlock *mergeBlock =
      llvm::BasicBlock::Create(*TheContext, "merge", TheFunction);

  // conditional branch
  Builder->CreateCondBr(conditionValue, if_block, elseBlock);

  // Generate code for the if block
  Builder->SetInsertPoint(if_block);
  llvm::Value *last_if = nullptr;
  for (auto node : ac_ast.body) {
    last_if = compile(node, table);
  }

  if (last_if != nullptr && llvm::isa<llvm::ReturnInst>(last_if)) {

  } else {
    // if it doesn't return
    Builder->CreateBr(mergeBlock);
  }

  llvm::Value *last_else = nullptr;
  Builder->SetInsertPoint(elseBlock);
  if (!ac_ast.alternate.empty()) {
    for (auto node :
         ac_ast.alternate[0]->get<donsus_ast::else_statement>().body) {
      last_else = compile(node, table);
    }
  }

  // here if the instruction does not exist, it's wrong because llvm::isa
  // fails
  if (last_else != nullptr && llvm::isa<llvm::ReturnInst>(last_else)) {

  } else {
    // if it doesn't return
    Builder->CreateBr(mergeBlock);
  }
  /*  if (!((last_if != nullptr && llvm::isa<llvm::ReturnInst>(last_if)) &&
          (last_else != nullptr && llvm::isa<llvm::ReturnInst>(last_else)))) {
      Builder->SetInsertPoint(mergeBlock);
    }*/
  // if its global go back to main
  Builder->SetInsertPoint(mergeBlock);
  /*  Builder->CreateUnreachable();*/
  return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0));
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::for_loop &ac_ast,
                           utility::handle<DonsusSymTable> &table) {
  // llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();

  // // Create blocks for the loop
  // llvm::BasicBlock *PreheaderBB = Builder->GetInsertBlock();
  // llvm::BasicBlock *LoopCondBB =
  //     llvm::BasicBlock::Create(*TheContext, "for.cond", TheFunction);
  // llvm::BasicBlock *LoopBodyBB =
  //     llvm::BasicBlock::Create(*TheContext, "for.body", TheFunction);
  // llvm::BasicBlock *LoopIncBB =
  //     llvm::BasicBlock::Create(*TheContext, "for.inc", TheFunction);
  // llvm::BasicBlock *AfterBB =
  //     llvm::BasicBlock::Create(*TheContext, "for.end", TheFunction);

  // // Allocate and initialize the loop variable
  // llvm::AllocaInst *Alloca = Builder->CreateAlloca(
  //     llvm::Type::getInt32Ty(*TheContext), nullptr, ac_ast.loop_variable);
  // llvm::Value *StartVal = compile(ac_ast.start, table);
  // Builder->CreateStore(StartVal, Alloca);

  // // Jump to the loop condition block
  // Builder->CreateBr(LoopCondBB);
  // Builder->SetInsertPoint(LoopCondBB);

  // // Load the current value of the loop variable
  // llvm::Value *CurVal = Builder->CreateLoad(Alloca->getAllocatedType(),
  // Alloca);

  // // Compare the loop variable with the end value
  // llvm::Value *EndVal = compile(ac_ast.end, table);
  // llvm::Value *Cond = Builder->CreateICmpSLT(CurVal, EndVal);

  // // Branch based on the comparison
  // Builder->CreateCondBr(Cond, LoopBodyBB, AfterBB);

  // // Generate the loop body
  // Builder->SetInsertPoint(LoopBodyBB);

  // // Reload the current value of the loop variable
  // table->setInst(ac_ast.loop_variable, Alloca);

  // for (auto &bodyNode : ac_ast.body) {
  //   compile(bodyNode, table);
  // }

  // // After the body, jump to the increment block
  // Builder->CreateBr(LoopIncBB);
  // Builder->SetInsertPoint(LoopIncBB);

  // // Increment the loop variable
  // llvm::Value *StepVal =
  //     llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1));
  // llvm::Value *NextVal = Builder->CreateAdd(CurVal, StepVal);
  // Builder->CreateStore(NextVal, Alloca);

  // // Jump back to the loop condition block
  // Builder->CreateBr(LoopCondBB);

  // // Set the insertion point to the after block
  // Builder->SetInsertPoint(AfterBB);

  // Return value after the loop
  return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0));
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::return_kw &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  /*  if (ca_ast.types.size() > 1) {
      // if its a multiple type just return back the structure
      // with the loaded elements
      // make indices
      std::vector<std::vector<llvm::Value *>> indices;
      llvm::Value *Zero =
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0);
      for (int i = 0; i < ca_ast.types.size(); ++i) {
        indices[i].push_back(Zero);
        // don't have to be LLVM value - can be regular vector with int
        llvm::Value *tmp =
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), i);
        indices[i].push_back(tmp);
      }

      auto type = table->multiple_return.type_of_struct;
      auto inst = table->multiple_return.inst_of_struct;
      // obtain struct
      int counter = 0;
      for (auto in : indices) {

        llvm::Value *store_into = Builder->CreateGEP(type, inst, in);
        Builder->CreateStore(compile(ast->children[counter], table),
    store_into);
        ++counter;
      }
      llvm::Value *result = Builder->CreateLoad(type, inst);
      return Builder->CreateRet(result);
    }*/
  return Builder->CreateRet(compile(ast->children[0], table));
}

// Todo: do not call this twice
llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::string_expr &ast,
                           utility::handle<DonsusSymTable> &table) {
  std::string PreprocessedString;
  for (size_t i = 0; i < ast.value.value.size(); ++i) {
    if (ast.value.value[i] == '\\' && i + 1 < ast.value.value.size()) {
      switch (ast.value.value[i + 1]) {
      case 'n':
        PreprocessedString.push_back(0x0a);
        ++i;
        break;
      case 'a':
        PreprocessedString.push_back(0x07);
        ++i;
        break;
      case 'b':
        PreprocessedString.push_back(0x08);
        ++i;
        break;
      case 'e':
        PreprocessedString.push_back(0x1b);
        ++i;
        break;
      case 'f':
        PreprocessedString.push_back(0x0c);
        ++i;
        break;
      case 'r':
        PreprocessedString.push_back(0x0d);
        ++i;
        break;
      case 't':
        PreprocessedString.push_back(0x09);
        ++i;
        break;
      case 'v':
        PreprocessedString.push_back(0x0b);
        ++i;
        break;
      default:
        PreprocessedString.push_back(ast.value.value[i]);
        break;
      }
    } else {
      PreprocessedString.push_back(ast.value.value[i]);
    }
  }
  return Builder->CreateGlobalStringPtr(llvm::StringRef(PreprocessedString));
}
llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::float_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  return llvm::ConstantFP::get(
      *TheContext,
      llvm::APFloat(std::stof(ast->get<donsus_ast::float_expr>().value.value)));
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::expression &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  switch (ca_ast.value.kind) {
  case DONSUS_PLUS: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      if (ast->real_type.is_float()) {
        return Builder->CreateFAdd(compile(*it, table),
                                   compile(*(it + 1), table));
      }
      return Builder->CreateAdd(compile(*it, table), compile(*(it + 1), table));
    }
  }
  case DONSUS_MINUS: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      if (ast->real_type.is_float()) {
        return Builder->CreateFSub(compile(*it, table),
                                   compile(*(it + 1), table));
      }
      return Builder->CreateSub(compile(*it, table), compile(*(it + 1), table));
    }
  }
  case DONSUS_SLASH: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      if (ast->real_type.is_float()) {
        return Builder->CreateFDiv(compile(*it, table),
                                   compile(*(it + 1), table));
      }
      return Builder->CreateSDiv(compile(*it, table),
                                 compile(*(it + 1), table));
    }
  }
  case DONSUS_STAR: {
    for (auto it = ast->children.begin(); it != ast->children.end(); ++it) {
      if (ast->real_type.is_float()) {
        return Builder->CreateFMul(compile(*it, table),
                                   compile(*(it + 1), table));
      }
      return Builder->CreateMul(compile(*it, table), compile(*(it + 1), table));
    }
  }
    // comparisons
  case DONSUS_DOUBLE_EQUAL: {
    // Handle ==
    llvm::Value *lhs = compile(ast->children[0], table);
    llvm::Value *rhs = compile(ast->children[1], table);
    return Builder->CreateICmpEQ(lhs, rhs, "cmp_eq");
  }
  case DONSUS_NOT_EQUAL: {
    // Handle !=
    llvm::Value *lhs = compile(ast->children[0], table);
    llvm::Value *rhs = compile(ast->children[1], table);
    return Builder->CreateICmpNE(lhs, rhs, "cmp_ne");
  }
  case DONSUS_GREATER: {
    // Handle >
    llvm::Value *lhs = compile(ast->children[0], table);
    llvm::Value *rhs = compile(ast->children[1], table);
    return Builder->CreateICmpSGT(lhs, rhs, "cmp_gt");
  }
  case DONSUS_GREATER_EQUAL: {
    // Handle >=
    llvm::Value *lhs = compile(ast->children[0], table);
    llvm::Value *rhs = compile(ast->children[1], table);
    return Builder->CreateICmpSGE(lhs, rhs, "cmp_ge");
  }
  case DONSUS_LESS: {
    // Handle <
    llvm::Value *lhs = compile(ast->children[0], table);
    llvm::Value *rhs = compile(ast->children[1], table);
    return Builder->CreateICmpSLT(lhs, rhs, "cmp_lt");
  }
  case DONSUS_LESS_EQUAL: {
    // Handle <=
    llvm::Value *lhs = compile(ast->children[0], table);
    llvm::Value *rhs = compile(ast->children[1], table);
    return Builder->CreateICmpSLE(lhs, rhs, "cmp_le");
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

  std::vector<llvm::Value *> Argsv;

  std::string format_string{};

  for (auto node : ast->children) {
    if (!is_expression(node)) {
      DonsusSymTable::sym sym = sym_from_node(node, table);
      if (sym.type.type_un == DONSUS_TYPE::TYPE_STATIC_ARRAY ||
          sym.type.type_un == DONSUS_TYPE::TYPE_DYNAMIC_ARRAY ||
          sym.type.type_un == DONSUS_TYPE::TYPE_FIXED_ARRAY) {

        for (size_t i = 0; i < sym.array.insts.size(); ++i) {
          format_string.append(printf_format(sym.array.type));
        }
        continue;
      }
      // not nice: I know
      // not expression but not an array might remove this
      format_string.append(printf_format(node->real_type));

    } else {
      format_string.append(printf_format(node->assume_type()));
    }
  }
  Argsv.push_back(Builder->CreateGlobalString(format_string));

  for (auto node : ast->children) {

    // just for access_array and literal float
    if (is_expression(node)) {
      llvm::Value *cur_value = compile(node, table);
      if (node->real_type.type_un == DONSUS_TYPE::TYPE_F32) {
        llvm::Value *Alloca_FLliteral =
            Builder->CreateAlloca(map_type(node->real_type));
        Builder->CreateStore(cur_value, Alloca_FLliteral);
        llvm::Value *loadedFloatValue =
            Builder->CreateLoad(map_type(node->real_type), Alloca_FLliteral);
        llvm::Value *new_value =
            Builder->CreateFPExt(loadedFloatValue, Builder->getDoubleTy());
        Argsv.push_back(new_value);
        continue;
      }
      if (cur_value->getType()->isPointerTy() &&
          node->type.type ==
              donsus_ast::donsus_node_type::underlying::DONSUS_ARRAY_ACCESS) {
        Argsv.push_back(
            Builder->CreateLoad(map_type(node->real_type), cur_value));
      } else {
        Argsv.push_back(cur_value);
      }
      continue;
    }

    DonsusSymTable::sym sym = sym_from_node(node, table);
    if (sym.type.type_un == DONSUS_TYPE::TYPE_STATIC_ARRAY ||
        sym.type.type_un == DONSUS_TYPE::TYPE_DYNAMIC_ARRAY ||
        sym.type.type_un == DONSUS_TYPE::TYPE_FIXED_ARRAY) {
      for (auto i : sym.array.insts) {
        if (!i->getType()->isPointerTy()) {
          Argsv.push_back(i);
          continue;
        }
        Argsv.push_back(Builder->CreateLoad(map_type(sym.array.type), i));
      }
      continue;
    }
    if (sym.type.type_un == DONSUS_TYPE::TYPE_F32 ||
        sym.type.type_un == DONSUS_TYPE::TYPE_F64) {
      // https://stackoverflow.com/questions/63144506/printf-doesnt-work-for-floats-in-llvm-ir#comment111685194_63156309
      llvm::Value *loadedFloatValue =
          Builder->CreateLoad(map_type(sym.type), sym.inst);
      llvm::Value *new_value =
          Builder->CreateFPExt(loadedFloatValue, Builder->getDoubleTy());
      Argsv.push_back(new_value);
      continue;
    }

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

std::string DonsusCodeGenerator::printf_format(DONSUS_TYPE type) const {
  switch (type.type_un) {
  case DONSUS_TYPE::TYPE_BASIC_INT:
  case DONSUS_TYPE::TYPE_I32:
  case DONSUS_TYPE::TYPE_U64:
  case DONSUS_TYPE::TYPE_I8:
  case DONSUS_TYPE::TYPE_I64:
  case DONSUS_TYPE::TYPE_I16:
  case DONSUS_TYPE::TYPE_U32: {
    return "%d";
  }
  case DONSUS_TYPE::TYPE_STRING: {
    return "%s";
  }
  case DONSUS_TYPE::TYPE_F32:
  case DONSUS_TYPE::TYPE_F64:
    return "%f";
  default: {
  }
  }
  return {};
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::unary_expr &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  // negative number expression
  if (ast->children[0]->type.type ==
      donsus_ast::donsus_node_type::DONSUS_NUMBER_EXPRESSION) {
    return llvm::ConstantInt::get(
        *TheContext,
        llvm::APInt(
            map_bitwidth(ast->real_type),
            -std::stoi(
                ast->children[0]->get<donsus_ast::number_expr>().value.value),
            true));
  }
}

/*Maps DONSUS_TYPE to llvm TYPE.
 **/
llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::array_decl &ca_ast,
                           utility::handle<DonsusSymTable> &table) {

  /*llvm::ArrayType *arrayType =
   * llvm::ArrayType::get(map_type(make_type(ca_ast.type)), ca_ast.);*/
  return nullptr;
}

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::array_def &ca_ast,
                           utility::handle<DonsusSymTable> &table) {
  auto type = ca_ast.type;
  auto name = ca_ast.identifier_name;
  if (is_global_sym(name, table)) {
    DonsusSymTable::sym symbol = table->get(name);

    /*
     * llvm::ArrayType::get(map_type(make_type(type))
     * */
    llvm::ArrayType *arrayType =
        llvm::ArrayType::get(map_type(make_type(type)), ca_ast.size);

    // create array
    DonsusSymTable::sym::donsus_array don_a;
    don_a.num_of_elems = ca_ast.size;
    don_a.array_type = arrayType;
    std::vector<llvm::Constant *> v;
    std::vector<llvm::Value *> dynamic;

    for (auto node : ca_ast.elements) {
      llvm::Value *local = compile(node, table);
      auto *constant = llvm::dyn_cast<llvm::Constant>(local);
      if (constant) {
        // make it nicer
        don_a.insts.push_back(local);
        dynamic.push_back(local);
        v.push_back(constant);
      } else {
        // dynamic initialisation is needed
        don_a.insts.push_back(local);
        dynamic.push_back(local); // might create a different iterator for this
      }
    }
    don_a.type = make_type(type);
    // set array(local copy again)
    symbol.array = don_a;
    // both can share a constant initializer
    llvm::Constant *array_initializer = llvm::ConstantArray::get(arrayType, v);

    if (ca_ast.array_type == donsus_ast::ArrayType::FIXED) {
      auto *array = new llvm::GlobalVariable(
          *TheModule, arrayType, false, llvm::GlobalVariable::PrivateLinkage,
          array_initializer);

      symbol.inst = array;
      table->setSym(symbol.key, symbol);
      return array;
    } else if (ca_ast.array_type == donsus_ast::ArrayType::STATIC) {
      // isConstant - true -> makes it immutable
      auto *array = new llvm::GlobalVariable(
          *TheModule, arrayType, true, llvm::GlobalVariable::PrivateLinkage,
          array_initializer);
      symbol.inst = array; // re_load
      table->setSym(symbol.key, symbol);
      return array;
    } else if (ca_ast.array_type == donsus_ast::ArrayType::DYNAMIC) {

      auto *zero = llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0));

      auto index = 0;

      auto *array = new llvm::GlobalVariable(
          *TheModule, arrayType, false, llvm::GlobalVariable::PrivateLinkage,
          llvm::Constant::getNullValue(arrayType), "array");
      symbol.inst = array;
      table->setSym(symbol.key, symbol);
      for (auto dy_value : dynamic) {
        std::vector<llvm::Value *> idx_list;
        idx_list.push_back(zero);
        idx_list.push_back(
            llvm::ConstantInt::get(*TheContext, llvm::APInt(32, index)));
        index++;
        auto *gep = Builder->CreateGEP(arrayType, array, idx_list);
        Builder->CreateStore(dy_value, gep);
      }
      return array;
    }

  } else {
    // we dont support dynamic array codegen yet
    // local on the stack or smt
    // global variable is not needed here
  }

  return nullptr;
}

llvm::Value *DonsusCodegen::DonsusCodeGenerator::visit(
    utility::handle<donsus_ast::node> &ast, donsus_ast::array_access &ca_ast,
    utility::handle<DonsusSymTable> &table) {

  DonsusSymTable::sym symbol = table->get(ca_ast.identifier_name);
  llvm::Value *value;
  /*  if (!ast->children.empty()) {*/
  // arr[i] = smt;
  std::vector<llvm::Value *> indxList1{};
  indxList1.push_back(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
  indxList1.push_back(compile(ca_ast.index, table));

  value = Builder->CreateGEP(symbol.array.array_type, symbol.inst, indxList1);

  /*} */ /*else {
     // arr[i]
     return nullptr;
   }*/
  return value;
}
unsigned int
DonsusCodegen::DonsusCodeGenerator::map_bitwidth(DONSUS_TYPE type) {
  switch (type.type_un) {
  case DONSUS_TYPE::TYPE_I8: {
    return 8;
  }

  case DONSUS_TYPE::TYPE_I16: {
    return 16;
  }

  // i32 and literals
  case DONSUS_TYPE::TYPE_BASIC_INT:
  case DONSUS_TYPE::TYPE_I32:
  case DONSUS_TYPE::TYPE_UNSPECIFIED_INTEGER:
  case DONSUS_TYPE::TYPE_U32: {
    return 32;
  }

  case DONSUS_TYPE::TYPE_I64:
  case DONSUS_TYPE::TYPE_U64: {
    return 64;
  }
  default: {
    return 32;
  }
  }
}
llvm::Type *DonsusCodegen::DonsusCodeGenerator::map_type(DONSUS_TYPE type) {
  switch (type.type_un) {
  case DONSUS_TYPE::TYPE_I8: {
    return Builder->getInt8Ty();
  }

  case DONSUS_TYPE::TYPE_I16: {
    return Builder->getInt16Ty();
  }

  case DONSUS_TYPE::TYPE_BASIC_INT:
  case DONSUS_TYPE::TYPE_I32:
  case DONSUS_TYPE::TYPE_U32: {
    return Builder->getInt32Ty();
  }

  case DONSUS_TYPE::TYPE_I64: {
    return Builder->getInt64Ty();
  }

  case DONSUS_TYPE::TYPE_F32: {
    return Builder->getFloatTy();
  }
  case DONSUS_TYPE::TYPE_F64: {
    return Builder->getDoubleTy();
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
  case DONSUS_TYPE::TYPE_FIXED_ARRAY: {
  }
  case DONSUS_TYPE::TYPE_STATIC_ARRAY: {
  }
  case DONSUS_TYPE::TYPE_DYNAMIC_ARRAY: {
  }
  default: {
  }
  }
  return nullptr;
}
} // namespace DonsusCodegen