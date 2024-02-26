#include "DFA.h"

using namespace llvm;
/// @todo(CSCD70) Please complete this file.
AnalysisKey LiveNess::Key;

void LiveNess::InitializeDomainFromInstruction(const llvm::Instruction& In)
{
  for(auto& operand : In.operands())
  {
    if(isa<Instruction>(operand)||isa<Argument>(operand))
    {
      if((DomainIdMap.emplace(std::pair(operand.get(),DomainIdMap.size()))).second)
      {
        DomainVector.push_back(operand.get());
      }
    }
  }
}


bool LiveNess::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                             DomainVal_t &ODV) {

  /// @todo(CSCD70) Please complete this method.
  DomainVal_t NewODV = IDV;

  if(Inst.hasNUsesOrMore(1))
  {
    int64_t id;
    if((id = getDomainId(dfa::Variable::VariableFromIn(Inst)))!=-1)
    {
      NewODV.at(id).Value = false;
    }
  }

  for(auto Iter  = Inst.op_begin();Iter != Inst.op_end(); ++Iter)
  {
    Value *V = *Iter;

    if(isa<Instruction>(V)||isa<Argument>(V))
    {
      int64_t id;
      if((id = getDomainId(V))!=-1)
      {
        NewODV.at(id).Value = true;
      }
    }
  }


  if(NewODV==ODV)
    return false;
  ODV = NewODV;
  return true;
}
