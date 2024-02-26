#pragma once // NOLINT(llvm-header-guard)

#include "Framework.h"

/// @todo(CSCD70) Please instantiate for the backward pass, similar to the
///               Backward one.
/// @sa @c BackwardAnalysis
namespace dfa {

/// @todo(CSCD70) Please modify the traversal ranges.

typedef llvm::iterator_range<llvm::const_succ_iterator>
    BackwardMeetBBConstRange_t;
typedef llvm::iterator_range<llvm::Function::BasicBlockListType::const_reverse_iterator>
    BackwardBBConstRange_t;
typedef llvm::iterator_range<llvm::BasicBlock::const_reverse_iterator>
    BackwardInstConstRange_t;

template <typename TDomainElem, typename TValue, typename TMeetOp>
class BackwardAnalysis
    : public Framework<TDomainElem, TValue, TMeetOp, BackwardMeetBBConstRange_t,
                       BackwardBBConstRange_t, BackwardInstConstRange_t> {
protected:
  using Framework_t =
      Framework<TDomainElem, TValue, TMeetOp, BackwardMeetBBConstRange_t,
                BackwardBBConstRange_t, BackwardInstConstRange_t>;
  using typename Framework_t::AnalysisResult_t;
  using typename Framework_t::BBConstRange_t;
  using typename Framework_t::InstConstRange_t;
  using typename Framework_t::MeetBBConstRange_t;
  using typename Framework_t::MeetOperands_t;
  using typename Framework_t::DomainVal_t;


  using Framework_t::BVs;
  using Framework_t::DomainIdMap;
  using Framework_t::DomainVector;
  using Framework_t::InstDomainValMap;

  using Framework_t::getName;
  using Framework_t::run;
  using Framework_t::stringifyDomainWithMask;

  void printInstDomainValMap(const llvm::Instruction &Inst) const final {
    using llvm::errs;
    using llvm::outs;
    const llvm::BasicBlock *const ParentBB = Inst.getParent();


    LOG_ANALYSIS_INFO << "\t"
                      << stringifyDomainWithMask(InstDomainValMap.at(&Inst));
    outs() << Inst << "\n";
    if (&Inst == &(ParentBB->back())) {
      LOG_ANALYSIS_INFO << "\t" << stringifyDomainWithMask(BVs.at(ParentBB));
      errs() << "\n";
    } // if (&Inst == &(*ParentBB->begin()))
  }

  virtual MeetOperands_t getMeetOperands(const llvm::BasicBlock &BB) const {
    MeetOperands_t Operands;
    /// @todo(CSCD70) Please complete this method.
    for(const llvm::BasicBlock* preBB : getMeetBBConstRange(BB))
    {
      DomainVal_t domain = InstDomainValMap.at(&(preBB->front()));
      Operands.push_back(domain);
    }
    return Operands;
  }

  MeetBBConstRange_t
  getMeetBBConstRange(const llvm::BasicBlock &BB) const final {
    return llvm::successors(&BB);
  }
  InstConstRange_t getInstConstRange(const llvm::BasicBlock &BB) const final {
    return make_range(BB.rbegin(), BB.rend());
  }
  BBConstRange_t getBBConstRange(const llvm::Function &F) const final {
    return make_range(F.rbegin(), F.rend());
  }
};

} // namespace dfa
