#pragma once // NOLINT(llvm-header-guard)

#include <DFA/Domain/Expression.h>
#include <DFA/Domain/Variable.h>
#include <DFA/Flow/ForwardAnalysis.h>
#include <DFA/Flow/BackwardAnalysis.h>
#include <DFA/MeetOp.h>

#include <llvm/IR/PassManager.h>

using DomainVal_t = std::vector<dfa::Bool>;


class AntiExprs final : public dfa::BackwardAnalysis<dfa::Expression, dfa::Bool,
                                                     dfa::Intersect<dfa::Bool>>,
                         public llvm::AnalysisInfoMixin<AntiExprs> {
private:
  using BackwardAnalysis_t = dfa::BackwardAnalysis<dfa::Expression, dfa::Bool,
                                                 dfa::Intersect<dfa::Bool>>;

  friend llvm::AnalysisInfoMixin<AntiExprs>;
  static llvm::AnalysisKey Key;

  std::string getName() const final { return "AntiExprs"; }
  bool transferFunc(const llvm::Instruction &, const DomainVal_t &,
                    DomainVal_t &) final;
  


virtual void InitializeDomainFromInstruction(const llvm::Instruction& In);

public:
  using Result = typename BackwardAnalysis_t::AnalysisResult_t;
  using BackwardAnalysis_t::run;
  
};



class WBAvailExprs final : public dfa::ForwardAnalysis<dfa::Expression, dfa::Bool,
                                                     dfa::Intersect<dfa::Bool>>,
                         public llvm::AnalysisInfoMixin<WBAvailExprs> {
private:
  using ForwardAnalysis_t = dfa::ForwardAnalysis<dfa::Expression, dfa::Bool,
                                                 dfa::Intersect<dfa::Bool>>;

  friend llvm::AnalysisInfoMixin<WBAvailExprs>;
  static llvm::AnalysisKey Key;

  std::string getName() const final { return "WBAvailExprs"; }
  bool transferFunc(const llvm::Instruction &, const DomainVal_t &,
                    DomainVal_t &) final;
  


virtual void InitializeDomainFromInstruction(const llvm::Instruction& In);

public:
  using Result = typename ForwardAnalysis_t::AnalysisResult_t;
  using ForwardAnalysis_t::run;
  
};


class PostponExprs final : public dfa::BackwardAnalysis<dfa::Expression, dfa::Bool,
                                                     dfa::Intersect<dfa::Bool>>,
                         public llvm::AnalysisInfoMixin<PostponExprs> {
private:
  using BackwardAnalysis_t = dfa::BackwardAnalysis<dfa::Expression, dfa::Bool,
                                                 dfa::Intersect<dfa::Bool>>;

  friend llvm::AnalysisInfoMixin<PostponExprs>;
  static llvm::AnalysisKey Key;

  std::string getName() const final { return "PostponExprs"; }
  bool transferFunc(const llvm::Instruction &, const DomainVal_t &,
                    DomainVal_t &) final;
  


virtual void InitializeDomainFromInstruction(const llvm::Instruction& In);

public:
  using Result = typename BackwardAnalysis_t::AnalysisResult_t;
  using BackwardAnalysis_t::run;
  
};


extern AntiExprs::Result* antiexprs_ret;
extern WBAvailExprs::Result* wbavailExprs_ret;
extern std::unordered_map<const llvm::Instruction *, DomainVal_t> Earliest;

extern int64_t getDomainId(dfa::Expression& expr,std::unordered_map<dfa::Expression,size_t>);

void EarliestPlacement(llvm::Function& F);

class LCMWrapperPass : public llvm::PassInfoMixin<LCMWrapperPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &FAM) {

    /// @todo(CSCD70) Get the result from the main body.
    antiexprs_ret = &FAM.getResult<AntiExprs>(F);
    // wbavailExprs_ret = &FAM.getResult<WBAvailExprs>(F);
    // EarliestPlacement(F);
    return llvm::PreservedAnalyses::all();
  }
};
