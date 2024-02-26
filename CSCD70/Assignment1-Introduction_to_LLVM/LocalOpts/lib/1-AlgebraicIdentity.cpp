#include "LocalOpts.h"

using namespace llvm;

void delIns(std::vector<Instruction*> InsList)
{
  for(auto& ins : InsList)
  {
    if(ins->isSafeToRemove())
    {
      ins->eraseFromParent();
    }
  }
}

PreservedAnalyses AlgebraicIdentityPass::run([[maybe_unused]] Function &F,
                                             FunctionAnalysisManager &) {

  /// @todo(CSCD70) Please complete this method.
  int cnt = 0;
  std::vector<Instruction*> del_InsList;
  for(auto& BB : F)
  {
    for(auto& In : BB)
    {
      if(In.isBinaryOp())
      {
        Value* AlgebraicIdentity = 0;
        bool flag = false;
        auto oper1 =In.getOperand(0);
        auto oper2 =In.getOperand(1);
        int64_t ConstVal1,ConstVal2 = 0;
        if(isa<ConstantInt>(oper1))
          ConstVal1 = dyn_cast<ConstantInt>(oper1)->getSExtValue();
        if(isa<ConstantInt>(oper2))
          ConstVal2 = dyn_cast<ConstantInt>(oper2)->getSExtValue();

        switch (In.getOpcode())
        {
          //x+0 x-0 --> x
        case Instruction::Add:
        case Instruction::Sub:
          if((isa<ConstantInt>(oper1)&&!ConstVal1)||(isa<ConstantInt>(oper2)&&!ConstVal2))
          {
            AlgebraicIdentity = oper1 ? oper1 : oper2;
            flag = true;
          }
          break;
        
        case Instruction::Mul:
        case Instruction::SDiv:
          if((isa<ConstantInt>(oper1)&&ConstVal1==1)||(isa<ConstantInt>(oper2)&&ConstVal2==1))
          {
            AlgebraicIdentity = ConstVal1==1? oper2 : oper1;
            flag = true; 
          }
        break;
        default:
          break;
        }
        if(flag)
        {
          In.replaceAllUsesWith(AlgebraicIdentity);
          del_InsList.push_back(&In);
          ++cnt;
        }
      }
    }
  }
  delIns(del_InsList);
  outs() << "define dso_local void @AlgebraicIdentity(i32 noundef %0) {" << "\n";
  outs() << "Algebraic Identity: " << cnt << "\n";
  return PreservedAnalyses::none();
}
