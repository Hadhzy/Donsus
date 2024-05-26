#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
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

  void create_loop() {
    LoopData data;
    // index number
    llvm::AllocaInst *index =
        Builder->CreateAlloca(llvm::Type::getInt32Ty(*TheContext));
    Builder->CreateStore(llvm::ConstantInt::get(Builder->getInt32Ty(), 0),
                         index);
    llvm::Value *max = llvm::ConstantInt::get(Builder->getInt32Ty(), 5);

    llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();
    // Create for loop
    llvm::BasicBlock *ForCondBB =
        llvm::BasicBlock::Create(*TheContext, "for.cond", TheFunction);
    llvm::BasicBlock *ForBodyBB = llvm::BasicBlock::Create(
        *TheContext, "for.body", TheFunction); // create blocks for the loop
    llvm::BasicBlock *ForIncBB = llvm::BasicBlock::Create(
        *TheContext, "for.inc", TheFunction); // increment
    llvm::BasicBlock *ForEndBB = llvm::BasicBlock::Create(
        *TheContext, "for.end", TheFunction); // create blocks for the loop

    // handle condition here
    // make sure the loop is still valid
    Builder->CreateBr(ForCondBB); // br label %for.cond
    Builder->SetInsertPoint(ForCondBB);

    llvm::Value *lhs = Builder->CreateLoad(Builder->getInt32Ty(), index);
    llvm::Value *cmp1 = Builder->CreateICmpSLT(lhs, max);
    Builder->CreateCondBr(cmp1, ForBodyBB, ForEndBB);

    // printf stuff
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(Builder->getPtrTy());

    llvm::FunctionType *FT1 = llvm::FunctionType::get(
        llvm::Type::getInt32Ty(*TheContext), printf_arg_types, true);

    llvm::Function *func =
        llvm::Function::Create(FT1, llvm::Function::ExternalLinkage,
                               llvm::Twine("printf"), TheModule.get());

    std::vector<llvm::Value* > values;
    values.push_back(index);


    // handle body (do nothing)
    Builder->SetInsertPoint(ForBodyBB);
    llvm::Value *loaded = Builder->CreateLoad(Builder->getInt32Ty(), index);
    llvm::Value *inc_ed = Builder->CreateAdd(
        loaded, llvm::ConstantInt::get(Builder->getInt32Ty(), 1));
    Builder->CreateStore(inc_ed, index);
    Builder->CreateBr(ForIncBB); // br label %for.inc

    Builder->SetInsertPoint(ForIncBB);
    llvm::Value *loaded_inc = Builder->CreateLoad(
        Builder->getInt32Ty(), index); // Load the incremented value
    Builder->CreateStore(loaded_inc,
                         index);  // Store the incremented value back to index
    Builder->CreateBr(ForCondBB); // br label %for.cond


    Builder->SetInsertPoint(ForEndBB);
    Builder->CreateUnreachable();
  }

  void create_entry_point() {
    llvm::FunctionType *FT =
        llvm::FunctionType::get(llvm::Type::getInt32Ty(*TheContext), false);
    std::string name = "main";

    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, name, *TheModule);
    llvm::BasicBlock *block = llvm::BasicBlock::Create(*TheContext, "entry", F);
    Builder->SetInsertPoint(block);
    create_loop();
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