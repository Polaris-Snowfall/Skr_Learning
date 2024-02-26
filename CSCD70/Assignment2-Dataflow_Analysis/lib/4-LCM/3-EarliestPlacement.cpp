#include "LCM.h"

/// @todo(CSCD70) Please complete this file.
using namespace llvm;
std::unordered_map<const llvm::Instruction *, DomainVal_t> Earliest;

int64_t getDomainId(dfa::Expression& expr,std::unordered_map<dfa::Expression , size_t> DomainIdMap)
{
    auto iter = DomainIdMap.find(expr);
    if(iter == DomainIdMap.end())
    return -1;
    return (*iter).second;
}

Instruction* getLastInstruction(Instruction* Inst)
{
    BasicBlock* BB  = Inst->getParent();
    Instruction* lastIn = NULL;
    for(auto& In : *BB)
    {
        if(&In == Inst)
            return lastIn;
        lastIn = &In;
    }
}

void EarliestPlacement(Function& F)
{
    auto& antiexpr_InstValMap =  std::get<3>(*antiexprs_ret);
    auto& wbavailexpr_InstValMap = std::get<3>(*wbavailExprs_ret);
    auto& wbavailexpr_BVs = std::get<2>(*wbavailExprs_ret);

    auto& antiexpr_DomainIdMap = std::get<0>(*antiexprs_ret);
    auto& wbavailexpr_DomainIdMap = std::get<0>(*wbavailExprs_ret);

    auto& antiexpr_DomainVector = std::get<1>(*antiexprs_ret);
    auto& wbavailexpr_DomainVector = std::get<1>(*wbavailExprs_ret);

    assert(antiexpr_DomainVector.size()==wbavailexpr_DomainVector.size());

    for(auto& BB : F)
    {
        for(auto& In : BB)
        {
            for(auto& expr : antiexpr_DomainVector)
            {
                int64_t id;
                if((id = getDomainId(expr,wbavailexpr_DomainIdMap))!=-1)
                {
                    Earliest.at(&In) = antiexpr_InstValMap.at(&In); 
                }
                Instruction* LastIn;
                if((LastIn = getLastInstruction(&In))!=NULL)
                {
                    if(wbavailexpr_InstValMap.at(LastIn).at(id).Value == true)
                        Earliest.at(&In).at(id).Value = false;
                }
                else
                {
                    if(wbavailexpr_BVs.at(In.getParent()).at(id).Value == true)
                        Earliest.at(&In).at(id).Value = false;
                }

            }
        }
    }
}