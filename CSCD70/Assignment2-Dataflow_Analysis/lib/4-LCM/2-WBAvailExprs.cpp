#include "LCM.h"

/// @todo(CSCD70) Please complete this file.
using namespace llvm;


AnalysisKey WBAvailExprs::Key;
WBAvailExprs::Result* wbavailExprs_ret;

void WBAvailExprs::InitializeDomainFromInstruction(const llvm::Instruction& In)
{
      if(In.isBinaryOp())
      {
        if((DomainIdMap.emplace(std::pair(dfa::Expression::ExpressionFromIn(In),DomainIdMap.size()))).second)
        {
          DomainVector.push_back(dfa::Expression::ExpressionFromIn(In));
        }
      }
}


bool WBAvailExprs::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                             DomainVal_t &ODV) {

    /// @todo(CSCD70) Please complete this method.
    DomainVal_t NewODV = IDV;

    std::unordered_map<const llvm::Instruction *, DomainVal_t> antiexpr_InstValMap = std::get<3>(*antiexprs_ret);

    NewODV =  dfa::Unite<dfa::Bool>()(NewODV,antiexpr_InstValMap.at(&Inst));

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

    if(NewODV==ODV)
        return false;
    ODV = NewODV;
    return true;
}
