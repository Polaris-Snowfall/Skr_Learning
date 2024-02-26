#include "LocalOpts.h"

using namespace llvm;

extern void delIns(std::vector<Instruction*> InsList);

PreservedAnalyses MultiInstOptPass::run([[maybe_unused]] Function &F,
                                        FunctionAnalysisManager &) {

  /// @todo(CSCD70) Please complete this method.
  int cnt = 0;
  std::vector<Instruction*> del_InsList;
  int bbs = 0;
  for(auto& BB : F)
  {
    for(auto& In : BB)
    {
      if(In.isBinaryOp())
      {
        int op = -1 ; //1 -> + ; 0 -> -;
        int Incnt = 0;
        auto oper1 = In.getOperand(0);
        auto oper2 = In.getOperand(1);
        switch (In.getOpcode())
        {
        case Instruction::Add:
          for(User* user:In.users())
          {
            if(Instruction* UserIn = dyn_cast<Instruction>(user))
            {
              Value* Val = NULL;
              if(In.isBinaryOp())
              {
                if(UserIn->getOpcode() == Instruction::Sub)
                {
                  // outs() << "debug: " << oper1 << " " << oper2 << " " <<  UserIn->getOperand(1) <<"\n";
                  if(UserIn->getOperand(1) == oper1)
                    Val = oper2;
                  else if(UserIn->getOperand(1) == oper2)
                    Val = oper1;

                  if(Val)
                  {
                    UserIn->replaceAllUsesWith(Val);  
                    del_InsList.push_back(UserIn);
                    ++cnt;
                  }
                }
              }
            }
          }
          break;
        
        default:
          break;
        }
      }
    }
  }
  delIns(del_InsList);
  outs() << "define dso_local void @MultiInstOpt(i32 noundef %0, i32 noundef %1) {" << "\n";
  outs() << "multi-inst-opt: " << cnt << "\n";
  return PreservedAnalyses::none();
}
