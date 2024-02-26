#include "LocalOpts.h"

using namespace llvm;

extern void delIns(std::vector<Instruction*> InsList);

int getshift(int x)
{
  if(!((x > 0) && !(x & (x - 1))))
    return -1;
  int i = -1;
  while(x)
  {
    x = x>>1;
    ++i;
  }
  return i;

}

PreservedAnalyses StrengthReductionPass::run([[maybe_unused]] Function &F,
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
        int shift = 0;
        Value* oper;
        auto oper1 =In.getOperand(0);
        auto oper2 =In.getOperand(1);
        int shift1 = -1,shift2 = -1;
        if(isa<ConstantInt>(oper1))
        {
          // outs() <<"debug1: " <<dyn_cast<ConstantInt>(oper1)->getSExtValue() << "\n";
          shift1 = getshift(dyn_cast<ConstantInt>(oper1)->getSExtValue());
        }
        if(isa<ConstantInt>(oper2))
        {
          // outs() << "debug2: " << dyn_cast<ConstantInt>(oper2)->getSExtValue() << "\n";
          shift2 = getshift(dyn_cast<ConstantInt>(oper2)->getSExtValue());
        }

        switch (In.getOpcode())
        {
        case Instruction::Mul:
        case Instruction::SDiv:
        case Instruction::UDiv:
          if((isa<ConstantInt>(oper1)&&shift1!=-1)||(isa<ConstantInt>(oper2)&&shift2!=-1))
          {
            shift = shift1==-1 ? shift2 : shift1;
            // outs() << "debug: " << shift1 << " " << shift2 << " " << shift << "\n";
            oper = shift1==-1? oper1 : oper2;
            
            IRBuilder<> builder(&In);
            
            Value* NewIns;
            switch (In.getOpcode())
            {
            case Instruction::Mul:
              NewIns = builder.CreateShl(oper,shift);
              break;
            case Instruction::SDiv:
            case Instruction::UDiv:
              NewIns =builder.CreateAShr(oper,shift);
              break;            
            }

            In.replaceAllUsesWith(NewIns);
            del_InsList.push_back(&In);
            cnt++;
          }
          break;
        default:
          break;
        }
      }
    }
  }
  delIns(del_InsList);
  outs() << "define dso_local void @StrengthReduction(i32 noundef %0) {" << "\n";
  outs() << "strength-reduction: " << cnt << "\n";
  return PreservedAnalyses::none();
}
