#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
// initialises an array dynamically

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

  // create array and initialise member dynamically
  void create_array() {
    llvm::ArrayType *arrayType =
        llvm::ArrayType::get(llvm::Type::getInt32Ty(*TheContext), 5);
    llvm::AllocaInst *allocaInst = Builder->CreateAlloca(arrayType, 0, nullptr);

    llvm::Constant *initial_value =
        llvm::Constant::getNullValue(Builder->getInt32Ty());

    // the type of the initializer must match the type of the array
    //  llvm::Constant::getNullValue(arrayType) for an arbitrary type.

    auto *array = new llvm::GlobalVariable(
        *TheModule, arrayType, false, llvm::GlobalVariable::PrivateLinkage,
        llvm::Constant::getNullValue(arrayType), "array");

    std::vector<llvm::Value *> indxList1{};
    std::vector<llvm::Value *> indxList2{};

    indxList1.push_back(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
    indxList1.push_back(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));

    indxList2.push_back(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));
    indxList2.push_back(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1)));

    auto *value = Builder->CreateGEP(arrayType, array, indxList1);
    auto *value2 = Builder->CreateGEP(arrayType, array, indxList2);

    Builder->CreateStore(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)), value);
    Builder->CreateStore(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 1)), value2);
  }

  void create_entry_point() {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    std::string name = "main";

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*TheContext, "entry", F);
    Builder->SetInsertPoint(block);
    Builder->SetInsertPoint(block);
    create_array();
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
