
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

    //   %retval = alloca %struct.TYPES, align 4
    auto IntType = llvm::IntegerType::get(*TheContext, 32);
    auto CharType = llvm::IntegerType::get(*TheContext, 8);
    auto my_struct_type = llvm::StructType::create(*TheContext, "TYPES");
    auto my_struct_ptr_type = llvm::PointerType::get(my_struct_type, 0);
    my_struct_type->setBody({IntType, CharType});
    llvm::Function* func = create_function(my_struct_type);
    Builder->SetInsertPoint(block);

    llvm::AllocaInst *inst = Builder->CreateAlloca(my_struct_type);

    //   %call = call i64 @return_multiple()
    llvm::CallInst *call_inst = Builder->CreateCall(func);


    Builder->CreateStore(call_inst,inst);
    Builder->CreateRet(llvm::ConstantInt::get(*TheContext, llvm::APInt(32, 0)));

  }

  llvm::Function* create_function(llvm::StructType* typeStruct) {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt64Ty(*TheContext), false);

    std::string name = "return_multiple";

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);
    llvm::BasicBlock *block =
        llvm::BasicBlock::Create(*TheContext, "return_multiple", F);
    Builder->SetInsertPoint(block);


    llvm::AllocaInst *inst = Builder->CreateAlloca(typeStruct);

    //   %a = getelementptr inbounds %struct.TYPES, ptr %retval, i32 0, i32 0
    std::vector<llvm::Value *> indices;
    std::vector<llvm::Value *> indices2;

    llvm::Value *Zero =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 0);
    llvm::Value *One =
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 1);
    indices.push_back(Zero);
    indices.push_back(Zero);

    indices2.push_back(Zero);
    indices2.push_back(One);

    //   store i32 1, ptr %a, align 4
    llvm::Value *store_first =
        Builder->CreateGEP(typeStruct, inst, indices);
    Builder->CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt32Ty(*TheContext), 1),
        store_first);

    //  %c = getelementptr inbounds %struct.TYPES, ptr %retval, i32 0, i32 1
    llvm::Value *store_second =
        Builder->CreateGEP(typeStruct, inst, indices2);
    //   store i8 99, ptr %c, align 4
    Builder->CreateStore(
        llvm::ConstantInt::get(llvm::Type::getInt8Ty(*TheContext), 99),
        store_second);

    // store i8 99, ptr %c, align 4
    llvm::Type *i32_type = llvm::IntegerType::getInt32Ty(*TheContext);
    llvm::Type *i64_type = llvm::IntegerType::getInt64Ty(*TheContext);
    // %0 = load i64, ptr %retval, align 4

    llvm::Value *result = Builder->CreateLoad(i64_type, inst);

   /* Builder->CreateLoad(llvm::ConstantExpr::getSizeOf(my_struct_type), )*/
    Builder->CreateRet(result);
    return F;
  }

  void create_global_data_structure() {}

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