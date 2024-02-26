#include "DFA.h"

using namespace llvm;


AnalysisKey AvailExprs::Key;

void AvailExprs::InitializeDomainFromInstruction(const llvm::Instruction& In)
{
      if(In.isBinaryOp())
      {
        if((DomainIdMap.emplace(std::pair(dfa::Expression::ExpressionFromIn(In),DomainIdMap.size()))).second)
        {
          DomainVector.push_back(dfa::Expression::ExpressionFromIn(In));
        }
      }
}


bool AvailExprs::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                             DomainVal_t &ODV) {

  /// @todo(CSCD70) Please complete this method.
  DomainVal_t NewODV = IDV;

  for(dfa::Expression expr : DomainVector)
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

  

  if(NewODV==ODV)
    return false;
  ODV = NewODV;
  return true;
}
