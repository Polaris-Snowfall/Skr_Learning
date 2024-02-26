#include "LCM.h"

/// @todo(CSCD70) Please complete this file.
using namespace llvm;


AnalysisKey AntiExprs::Key;
AntiExprs::Result* antiexprs_ret;

void AntiExprs::InitializeDomainFromInstruction(const llvm::Instruction& In)
{
      if(In.isBinaryOp())
      {
        if((DomainIdMap.emplace(std::pair(dfa::Expression::ExpressionFromIn(In),DomainIdMap.size()))).second)
        {
          DomainVector.push_back(dfa::Expression::ExpressionFromIn(In));
        }
      }
}


bool AntiExprs::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                             DomainVal_t &ODV) {

    /// @todo(CSCD70) Please complete this method.
    DomainVal_t NewODV = IDV;

    if(Inst.hasNUsesOrMore(1))
    {
        for(dfa::Expression& expr : DomainVector)
        {
            if(expr.LHS == &Inst || expr.RHS == &Inst)
            {
                int64_t id;
                if((id = getDomainId(expr))!=-1)
                {
                    NewODV.at(id).Value = false;
                }
            }
        }
    }

    if(Inst.isBinaryOp())
    {
        dfa::Expression expr(Inst.getOpcode(),Inst.getOperand(0),Inst.getOperand(1));

        int64_t id;
        if((id = getDomainId(expr))!= -1)
        {
        NewODV.at(id).Value = true;
        }    
    }


    if(NewODV==ODV)
        return false;
    ODV = NewODV;
    return true;
}
