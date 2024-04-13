// Running each step in the compiler
#include "llvm/ADT/StringRef.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Value.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <llvm/IR/Verifier.h>

// implements everything, except printf
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
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*TheContext, "main", F);
    Builder->SetInsertPoint(block);

    // For formatting %d
    llvm::Value *global_var = Builder->CreateGlobalString("%d", ".str");

    //  %add = alloca i32, align 4
    llvm::AllocaInst *Allocadef =
        Builder->CreateAlloca(Builder->getInt32Ty(), 0, "c");

    // store i32 10, ptr %add, align 4
    llvm::Type *i32_type = llvm::IntegerType::getInt32Ty(*TheContext);
    Builder->CreateStore(llvm::ConstantInt::get(i32_type, 10, true), Allocadef);

    // %1 = load i32, ptr %add, align 4
    llvm::Value *result = Builder->CreateLoad(i32_type, Allocadef);

    // declare printf function
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(Builder->getPtrTy());

    llvm::FunctionType *FT1 = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*TheContext), printf_arg_types, true);

    llvm::Function *func =
        llvm::Function::Create(FT1, llvm::Function::ExternalLinkage,
                               llvm::Twine("printf"), TheModule.get());
    func->setCallingConv(llvm::CallingConv::C);
    // load global

    // attribute group

    std::vector<llvm::Value *> values;
    values.push_back(global_var);
    values.push_back(result);
    llvm::CallInst *call = Builder->CreateCall(func, values);
    // ret i32 0
    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));

    llvm::verifyFunction(*F);
  }

  std::unique_ptr<llvm::LLVMContext> TheContext;
  std::unique_ptr<llvm::IRBuilder<>> Builder;
  std::unique_ptr<llvm::Module> TheModule;
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