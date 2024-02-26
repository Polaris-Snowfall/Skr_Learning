#include "DFA.h"

/// @todo(CSCD70) Please complete this file.
using namespace llvm;


AnalysisKey SCCP::Key;

void SCCP::InitializeDomainFromInstruction(const llvm::Instruction& In)
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

void calc(int64_t& result,const std::vector<int64_t>& val,const Instruction& In)
{
  switch (In.getOpcode())
  {
  case Instruction::Add:
    result += val.at(0);
    return;
  case Instruction::ICmp:
    if(auto IcmpIn = dyn_cast<ICmpInst>(&In))
    {
      switch (IcmpIn->getPredicate())
      {
      case ICmpInst::ICMP_SLT:
        result = (val[0] < val[1]);
        return;
      default:
        return;
        
      }
    }

  default:
    return;
  }
}

int64_t SCCP::getConstantValue(const Instruction& In,const DomainVal_t &IDV)
{
  int64_t result;
  std::vector<int64_t> val_list;
  for(auto& operand : In.operands())
  {

    if(!isa<ConstantInt>(operand))
    {
      int64_t id;
      if((id = getDomainId(operand.get()))!=-1)
      {
        val_list.push_back(IDV.at(id).Value);
      }
    }
    else
    {
      val_list.push_back(dyn_cast<ConstantInt>(operand)->getSExtValue());
    }
  }
  calc(result,val_list,In);
  return result;
}




bool SCCP::transferFunc(const Instruction &Inst, const DomainVal_t &IDV,
                             DomainVal_t &ODV) {

  /// @todo(CSCD70) Please complete this method.
  DomainVal_t NewODV = IDV;
  static std::set<BasicBlock*>NeverReachBBs;

  //对NeverReachBB不做处理
  for(auto& NeverReachBB : NeverReachBBs)
  {
    if(Inst.getParent()==NeverReachBB)
      return false;
  }

  //根据跳转条件是否为常量来
  //修改要操作的基本块集合
  if(Inst.getOpcode()==Instruction::Br) 
  {
    if(Inst.getNumOperands()==3) //只处理条件跳转
    {
      auto condition = Inst.getOperand(0);
      bool ifconst = false;
      int64_t truth;
      if(!isa<ConstantInt>(condition))
      {
        int64_t id;
        if((id = getDomainId(condition))!= -1)
        {
          if(IDV.at(id).Status == dfa::CONST)
          {
            ifconst = true;
            truth = IDV.at(id).Value;
          }
        }
      }
      else
      {
        ifconst = true;
        truth = dyn_cast<ConstantInt>(condition)->getSExtValue();
      }    
      if(ifconst)
      {
        if(truth)
        {
          NeverReachBBs.insert(dyn_cast<BasicBlock>(Inst.getOperand(1)));
        }
        else
        {
          NeverReachBBs.insert(dyn_cast<BasicBlock>(Inst.getOperand(2)));
        }
      }
      else
      {
        NeverReachBBs.erase(dyn_cast<BasicBlock>(Inst.getOperand(1)));
        NeverReachBBs.erase(dyn_cast<BasicBlock>(Inst.getOperand(2)));
      }
    }
  }
  else
  {
    //如果该指令是一次定义,设置该指令代表的变量为NAC
    if(isa<Instruction>(Inst)||isa<Argument>(Inst))
    {
      for(auto& V : DomainVector)
      {
        if(const Instruction* InofV = dyn_cast<Instruction>(V.Var))
        {
          for(auto& operand : InofV->operands())
          {
            if(operand.get() == &Inst)
            {
              int64_t id;
              if((id = getDomainId(dfa::Variable::VariableFromIn(Inst)))!=-1)
              {
                NewODV.at(id).Status = dfa::NAC;
                break;
              }
            }
          }
        }
      }

      int64_t id1,id2;
      bool ifnac = false;
      bool ifundef = false;
      bool ifphi = (Inst.getOpcode() == Instruction::PHI);
      if((id1 = getDomainId(dfa::Variable::VariableFromIn(Inst)))!= -1)
      {
        if(ifphi)
        {
          //特化phi,与其他的操作符有不同的规则
          dfa::Constant r = {.Status=dfa::UNDEF};
          for(auto& operand : Inst.operands())
          {
            bool flag = false;
            if(const PHINode* phiNode = dyn_cast<PHINode>(&Inst))
            {
              //如果phi的某个值来自NeverReachBB,不与其进行Meet操作.
              if((NeverReachBBs.find(phiNode->getIncomingBlock(operand)))!=NeverReachBBs.end())
              {
                flag = true;
              }

              if(!isa<ConstantInt>(operand))
              {
                if(!flag)
                {
                  if((id2 = getDomainId(operand.get()))!= -1)
                  {
                    r = r & (IDV.at(id2));
                  }
                }
              }
              else
              {
                if(!flag)
                {
                  r = r & dfa::Constant({.Value=dyn_cast<ConstantInt>(operand)->getSExtValue(),.Status=dfa::CONST});
                }
              }    
            }
          }
          NewODV.at(id1) = r;
        }
        else
        {
              for(auto& operand : Inst.operands())
              {
                if(!isa<ConstantInt>(operand))
                {
                  if((id2 = getDomainId(operand.get()))!= -1)
                  {
                    if(IDV[id2].Status==dfa::NAC)
                    {
                      NewODV[id1].Status = dfa::NAC;
                      ifnac = true;
                      break;
                    }
                    else if(IDV[id2].Status == dfa::UNDEF)
                    {
                      NewODV[id1].Status = dfa::UNDEF;
                      ifundef = true;
                      break;
                    }
                  }
                }    
              }
              if(!ifnac)
              {
                if(!ifundef)
                {
                  NewODV[id1].Status = dfa::CONST;
                  NewODV[id1].Value = getConstantValue(Inst,IDV);
                }
              }
          }
      }
    }
  }


    if(NewODV==ODV)
        return false;
    ODV = NewODV;
    return true;
}

