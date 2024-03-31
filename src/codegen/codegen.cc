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
void DonsusCodeGenerator::create_entry_point() {
  // create an entry function which can be used in the first block
  llvm::FunctionType *FT =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
  std::string name = "entry";

  llvm::Function *F = llvm::Function::Create(
      FT, llvm::Function::ExternalLinkage, name, *TheModule);
}
// traverse
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
  }
}

DonsusCodeGenerator::DonsusCodeGenerator(
    std::unique_ptr<llvm::LLVMContext> context,
    std::unique_ptr<llvm::Module> module,
    std::unique_ptr<llvm::IRBuilder<>> builder)
    : TheContext(std::move(context)), TheModule(std::move(module)),
      Builder(std::move(builder)) {

  create_entry_point();
  llvm::Function *TheFunction = TheModule->getFunction("entry");

  // assert here

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
  // so first load from the instruction and get its value
  // based on the operator use CreateAdd, CreateMul on the value that we want to
  // add.
  auto identifier_name = ac_ast.identifier_name;
  auto op = ac_ast.op;

  DonsusSymTable::sym sym1 = table->get(identifier_name);
  llvm::AllocaInst *A = sym1.inst;
  // op here, CreateAdd, CreateMul,
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

llvm::Value *
DonsusCodeGenerator::visit(utility::handle<donsus_ast::node> &ast,
                           donsus_ast::number_expr &ac_ast,
                           utility::handle<DonsusSymTable> &table) {
  // arbitrary precision integer, typically common unsigned ints
  // context_, llvm:PInt(32, int_expr.GetValAsInt(), true)));
  // only works with scalar integers, constant propagation is needed
  // constant propagate the operands into an unsigned int
  int value = ast->constant_propagation(ast);

  bool is_signed = false;
  if (value < 0) {
    is_signed = true;
  }

  return llvm::ConstantInt::get(*TheContext, llvm::APInt(32, value, is_signed));
}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::identifier &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_decl &ast,
                           utility::handle<DonsusSymTable> &table) {}

llvm::Value *
DonsusCodeGenerator::visit(donsus_ast::function_def &ast,
                           utility::handle<DonsusSymTable> &table) {}

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

  Builder->CreateGlobalStringPtr(llvm::StringRef(ast.value.value));
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

/*Maps DONSUS_TYPE to llvm TYPE.
 **/
llvm::Type *DonsusCodegen::DonsusCodeGenerator::map_type(DONSUS_TYPE type) {
  switch (type.type_un) {
  case DONSUS_TYPE::TYPE_VOID: {
    return nullptr;
  }

  // integers
  case DONSUS_TYPE::TYPE_I8: {
    return Builder->getInt8Ty();
  }

  case DONSUS_TYPE::TYPE_I16: {
    return Builder->getInt16Ty();
  }

  case DONSUS_TYPE::TYPE_BASIC_INT: {
    // arbitrary precision integer, like BIGINT
    // consider using APInt here, but we don't know the value
    return Builder->getInt32Ty();
  }

  case DONSUS_TYPE::TYPE_I32: {
    return Builder->getInt32Ty();
  }

  case DONSUS_TYPE::TYPE_I64: {
    return Builder->getInt64Ty();
  }
    /*  case DONSUS_TYPE::TYPE_U32: {
        return Builder->get
      }*/

    /*  case DONSUS_TYPE::TYPE_U64: {
        break;
      }

      case DONSUS_TYPE::TYPE_CHAR: {
        break;
      }*/
  default: {
  }
  }
  return nullptr;
}
} // namespace DonsusCodegen
