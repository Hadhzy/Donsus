
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
    create_function();

    llvm::Function *Func = TheModule->getFunction(
        "entry"); // This function should be main or whatever the convention is

    // // assert here

    /*    llvm::BasicBlock *entry =
            llvm::BasicBlock::Create(*TheContext, "entry_point", TheFunction);
  // create global variable integer
 Builder->SetInsertPoint(entry);*/
    llvm::Constant *int1 =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 19);
    llvm::Constant *initConst1 = new llvm::GlobalVariable(
        *TheModule, int1->getType(), false,
        llvm::GlobalVariable::ExternalLinkage, int1, "a");

    Builder->CreateGlobalString("%d", ".str");

    llvm::Value *global_var = TheModule->getGlobalVariable("a");
    // fill up main
    llvm::Function *my_func = TheModule->getFunction("main");
    llvm::Function *add_value_func = TheModule->getFunction("add_some_value");
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(*TheContext, "inside_main", my_func);

    Builder->SetInsertPoint(block);
    llvm::AllocaInst *Allocadef =
        Builder->CreateAlloca(Builder->getInt32Ty(), 0, "c");
    llvm::AllocaInst *Allocadef2 =
        Builder->CreateAlloca(Builder->getInt32Ty(), 0, "d");
    // call instruction
    llvm::CallInst *inst = Builder->CreateCall(add_value_func);
    // store into c
    Builder->CreateStore(inst, Allocadef);
    auto first_name = '1';
    auto second_name = '2';
    llvm::Value* load_a =
        Builder->CreateLoad(Builder->getInt32Ty(), global_var);
    llvm::Value* load_b =
        Builder->CreateLoad(Builder->getInt32Ty(), Allocadef);
    // get the value from the load instruction, then save it to d
    // print it out afterwards
    llvm::Value *add = Builder->CreateAdd(load_a, load_b);
    Builder->CreateStore(add, Allocadef2);

  Builder->CreateRet(
      llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 10)));
  }


  void add_variable() {}

  void create_function() {
    // i32
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    std::string name = "add_some_value";
    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);

    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(*TheContext, "add_some_value", F);
    Builder->SetInsertPoint(block);
    Builder->CreateRet(
        llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 10)));
    llvm::verifyFunction(*F);
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