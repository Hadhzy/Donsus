#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>

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

  void create_entry_point() {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    std::string name = "main";

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*TheContext, "entry", F);
    Builder->SetInsertPoint(block);

    // Actual instructions for main
    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));

    // Creating "just_void" function
    llvm::FunctionType* FT2 = llvm::FunctionType::get(llvm::Type::getVoidTy(*TheContext), false);
    std::string name2 = "just_void";

    llvm::Function *F1 = llvm::Function::Create(
        FT2, llvm::Function::ExternalLinkage, name2, *TheModule
    );

    llvm::BasicBlock* block2 = llvm::BasicBlock::Create(*TheContext, "just_void", F1);

    // Actual instructions for just_void
    // call it
    Builder->SetInsertPoint(block2);
    Builder->CreateRetVoid();
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