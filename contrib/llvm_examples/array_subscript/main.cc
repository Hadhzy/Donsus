#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
// for subscript array[index]
struct LoopData {
  llvm::Value *idx;
  llvm::BasicBlock *body;
  llvm::BasicBlock *done;
  llvm::BasicBlock *loop;
};

class CodeGen {
public:
  CodeGen() = default;
  CodeGen(std::unique_ptr<llvm::LLVMContext> context,
          std::unique_ptr<llvm::Module> module,
          std::unique_ptr<llvm::IRBuilder<>> builder)
      : TheContext(std::move(context)), TheModule(std::move(module)),
        Builder(std::move(builder)) {
    create_entry_point();
  }

  void create_array() {
    llvm::ArrayType *arrayType = llvm::ArrayType::get(Builder->getInt32Ty(), 4);
    llvm::AllocaInst *allocaInst =
        Builder->CreateAlloca(arrayType, 0, nullptr, "myarray");

    std::vector<int> v = {1, 2, 3, 4};

    llvm::Constant *array_initializer =
        llvm::ConstantDataArray::get(*TheContext, v);

    llvm::GlobalVariable *array = new llvm::GlobalVariable(
        *TheModule, arrayType, false, llvm::GlobalVariable::PrivateLinkage,
        array_initializer, "array");

    index_array(arrayType, array);
  }

  void index_array(llvm::ArrayType *ar_type, llvm::GlobalVariable *array) {
    std::vector<llvm::Value *> indices;
    indices.push_back(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
    indices.push_back(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1)));
    llvm::Value *res = Builder->CreateGEP(ar_type, array, indices);

    llvm::Value *store = Builder->CreateStore(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)), res);
    Builder->CreateLoad(Builder->getInt32Ty(), res);
  }

  void create_entry_point() {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    std::string name = "main";

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*TheContext, "entry", F);
    Builder->SetInsertPoint(block);

    create_array();
    Builder->SetInsertPoint(block);
    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
    llvm::verifyFunction(*F);
  }
  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::Module> TheModule;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
};
int main() {
  std::unique_ptr<llvm::LLVMContext> TheContext =
      std::make_unique<llvm::LLVMContext>();
  std::unique_ptr<llvm::Module> TheModule =
      std::make_unique<llvm::Module>("Donsus IR", *TheContext);
  std::unique_ptr<llvm::IRBuilder<>> TheBuilder =
      std::make_unique<llvm::IRBuilder<>>(*TheContext);

  CodeGen codegen(std::move(TheContext), std::move(TheModule),
                  std::move(TheBuilder));

  llvm::errs() << *codegen.TheModule;
  return 0;
}