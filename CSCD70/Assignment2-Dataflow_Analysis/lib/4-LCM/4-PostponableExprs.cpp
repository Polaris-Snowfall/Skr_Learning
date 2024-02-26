#include "LCM.h"

/// @todo(CSCD70) Please complete this file.
using namespace llvm;


AnalysisKey PostponExprs::Key;
PostponExprs::Result* Postponexprs_ret;


void PostponExprs::InitializeDomainFromInstruction(const llvm::Instruction& In)
{
      if(In.isBinaryOp())
      {
        if((DomainIdMap.emplace(std::pair(dfa::Expression::ExpressionFromIn(In),DomainIdMap.size()))).second)
        {
          DomainVector.push_back(dfa::Expression::ExpressionFromIn(In));
        }
      }
}


bool PostponExprs::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                             DomainVal_t &ODV) {

    /// @todo(CSCD70) Please complete this method.
    DomainVal_t NewODV = IDV;

    auto& wbavailexpr_DomainIdMap = std::get<0>(*wbavailExprs_ret);
    dfa::Expression expr(Inst.getOpcode(),Inst.getOperand(0),Inst.getOperand(1));

    int64_t id;
    if(id = (::getDomainId(expr,wbavailexpr_DomainIdMap)) != -1)
    {
        if(Earliest.at(&Inst).at(id) == true)
            NewODV.at(id).Value = true; 
    }

    if(Inst.isBinaryOp())
    {
        int64_t id;
        if((id = getDomainId(expr))!= -1)
        {
        NewODV.at(id).Value = false;
        }    
    }


    if(NewODV==ODV)
        return false;
    ODV = NewODV;
    return true;
}
