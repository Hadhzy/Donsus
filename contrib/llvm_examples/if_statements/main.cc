
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

class CodeGen {
public:
  CodeGen() = default;
  CodeGen(std::unique_ptr<llvm::LLVMContext> context,
          std::unique_ptr<llvm::Module> module,
          std::unique_ptr<llvm::IRBuilder<>> builder)
      : TheContext(std::move(context)), TheModule(std::move(module)),
        Builder(std::move(builder)) {
    // main wrapper function
    create_entry_point();
    llvm::Function *main_function = TheModule->getFunction("main");

    llvm::BasicBlock *entry =
        llvm::BasicBlock::Create(*TheContext, "inside_main", main_function);

    Builder->SetInsertPoint(entry);

    llvm::AllocaInst *allocate_a =
        Builder->CreateAlloca(Builder->getInt32Ty(), 0, "a");
    Builder->CreateStore(llvm::ConstantInt::get(
                             llvm::Type::getInt32Ty(*TheContext), 11), // a = 11
                         allocate_a);
    // Create comparison instruction (a > 10)
    llvm::Value *cmp = Builder->CreateICmpSGT(
        Builder->CreateLoad(llvm::IntegerType::getInt32Ty(*TheContext),
                            allocate_a),
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 10));

    llvm::BasicBlock *thenBlock = llvm::BasicBlock::Create(
        *TheContext, "then", main_function);                // then block
    llvm::BasicBlock *elseBlock = llvm::BasicBlock::Create( // else block
        *TheContext, "else", main_function);
    llvm::BasicBlock *mergeBlock = llvm::BasicBlock::Create( // merge block
        *TheContext, "ifcont", main_function);

    // conditional branch
    Builder->CreateCondBr(cmp, thenBlock, elseBlock);

    Builder->SetInsertPoint(thenBlock);
    Builder->CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0),
        allocate_a);
    Builder->CreateBr(mergeBlock);

    Builder->SetInsertPoint(elseBlock);
    Builder->CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 1),
        allocate_a);
    Builder->CreateBr(mergeBlock);

    Builder->SetInsertPoint(mergeBlock);

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

    llvm::Value *formatStr = Builder->CreateGlobalStringPtr("%d\n");
    std::vector<llvm::Value *> printfArgs;
    printfArgs.push_back(formatStr);
    printfArgs.push_back(Builder->CreateLoad(
        llvm::IntegerType::getInt32Ty(*TheContext), allocate_a));
    llvm::CallInst *call = Builder->CreateCall(func, printfArgs);

    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));

    llvm::verifyFunction(*main_function);
  }

  void create_entry_point() {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    std::string name = "main";

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);
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