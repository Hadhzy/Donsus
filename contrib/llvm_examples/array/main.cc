#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

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

  void create_global_array() {

    llvm::ArrayType *arrayType = llvm::ArrayType::get(Builder->getInt32Ty(), 4);
    llvm::AllocaInst *allocaInst =
        Builder->CreateAlloca(arrayType, 0, nullptr, "myarray");

    // initialiser
    std::vector<int> v = {1, 2, 3, 4};

    llvm::Constant *array_initializer =
        llvm::ConstantDataArray::get(*TheContext, v);

    llvm::GlobalVariable *array = new llvm::GlobalVariable(
        *TheModule, arrayType, false, llvm::GlobalVariable::PrivateLinkage,
        array_initializer, "array");

    // here it must be  ArrayRef<Value *>
    std::vector<llvm::Value *> indices;
    indices.push_back(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
    indices.push_back(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1)));
    llvm::Value *res = Builder->CreateGEP(arrayType, array, indices);
    llvm::Value *store = Builder->CreateStore(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)), res);

    // now we can use this value everywhere
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

    create_global_array();

    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
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