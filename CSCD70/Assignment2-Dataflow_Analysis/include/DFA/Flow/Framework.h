#pragma once // NOLINT(llvm-header-guard)

#include <llvm/IR/Function.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/PassManager.h>
#include <llvm/Support/raw_ostream.h>

#include <tuple>
namespace dfa {

template <typename TValue> struct ValuePrinter {
  static std::string print(const TValue &V) { return ""; }
};


template <typename TDomainElem, typename TValue, typename TMeetOp,
          typename TMeetBBConstRange, typename TBBConstRange,
          typename TInstConstRange>
class Framework {
protected:
  using DomainIdMap_t = typename TDomainElem::DomainIdMap_t;
  using DomainVector_t = typename TDomainElem::DomainVector_t;
  using DomainVal_t = typename TMeetOp::DomainVal_t;
  using MeetOperands_t = std::vector<DomainVal_t>;
  using MeetBBConstRange_t = TMeetBBConstRange;
  using BBConstRange_t = TBBConstRange;
  using InstConstRange_t = TInstConstRange;
  using AnalysisResult_t =
      std::tuple<DomainIdMap_t, DomainVector_t,
                 std::unordered_map<const llvm::BasicBlock *, DomainVal_t>,
                 std::unordered_map<const llvm::Instruction *, DomainVal_t>>;

  DomainIdMap_t DomainIdMap;
  DomainVector_t DomainVector;
  std::unordered_map<const llvm::BasicBlock *, DomainVal_t> BVs;
  std::unordered_map<const llvm::Instruction *, DomainVal_t> InstDomainValMap;

  /// @name Print utility functions
  /// @{

  std::string stringifyDomainWithMask(const DomainVal_t &Mask) const {
    std::string StringBuf;
    llvm::raw_string_ostream Strout(StringBuf);
    Strout << "{";
    CHECK(Mask.size() == DomainIdMap.size() &&
          Mask.size() == DomainVector.size())
        << "The size of mask must be equal to the size of domain, but got "
        << Mask.size() << " vs. " << DomainIdMap.size() << " vs. "
        << DomainVector.size() << " instead";
    for (size_t DomainId = 0; DomainId < DomainIdMap.size(); ++DomainId) {
      if (!static_cast<bool>(Mask[DomainId])) {
        continue;
      }
      Strout << DomainVector.at(DomainId)
             << ValuePrinter<TValue>::print(Mask[DomainId]) << ", ";
    } // for (MaskIdx : [0, Mask.size()))
    Strout << "}";
    return StringBuf;
  }
  virtual void printInstDomainValMap(const llvm::Instruction &Inst) const = 0;
  void printInstDomainValMap(const llvm::Function &F) const {
    for (const llvm::Instruction &Inst : llvm::instructions(&F)) {
      printInstDomainValMap(Inst);
    }
  }
  virtual std::string getName() const = 0;

  /// @}
  /// @name Boundary values
  /// @{

  DomainVal_t getBoundaryVal(const llvm::BasicBlock &BB) const {
    MeetOperands_t MeetOperands = getMeetOperands(BB);
  
    /// @todo(CSCD70) Please complete this method.

    //对于Boudary Condition,没有前驱块
    if(MeetOperands.empty())
      MeetOperands.push_back(bc());
    return meet(MeetOperands);
  }
  /// @brief Get the list of basic blocks to which the meet operator will be
  ///        applied.
  /// @param BB
  /// @return
  virtual MeetBBConstRange_t
  getMeetBBConstRange(const llvm::BasicBlock &BB) const = 0;
  /// @brief Get the list of domain values to which the meet operator will be
  ///        applied.
  /// @param BB
  /// @return
  /// @sa @c getMeetBBConstRange
  virtual MeetOperands_t getMeetOperands(const llvm::BasicBlock &BB) const = 0;

  DomainVal_t bc() const { return DomainVal_t(DomainIdMap.size()); }
  DomainVal_t meet(const MeetOperands_t &MeetOperands) const {

    /// @todo(CSCD70) Please complete this method.
    DomainVal_t result = TMeetOp().top(DomainIdMap.size());
    for(DomainVal_t domainval : MeetOperands)
    {
      result = TMeetOp()(result,domainval);
    }
    return result;
  }

    int64_t getDomainId(const TDomainElem& expr)
    {
      typename DomainIdMap_t::iterator iter = DomainIdMap.find(expr);
      if(iter == DomainIdMap.end())
        return -1;
      return (*iter).second;
    }


  /// @}
  /// @name CFG traversal
  /// @{

  /// @brief Get the list of basic blocks from the function.
  /// @param F
  /// @return
  virtual BBConstRange_t getBBConstRange(const llvm::Function &F) const = 0;
  /// @brief Get the list of instructions from the basic block.
  /// @param BB
  /// @return
  virtual InstConstRange_t
  getInstConstRange(const llvm::BasicBlock &BB) const = 0;
  /// @brief Traverse through the CFG of the function.
  /// @param F
  /// @return True if either BasicBlock-DomainValue mapping or
  ///         Instruction-DomainValue mapping has been modified, false
  ///         otherwise.
  bool traverseCFG(const llvm::Function &F) {
    bool Changed = false;

    /// @todo(CSCD70) Please complete this method.
    //在Froward和Backward的分析中,IDV,ODV的意义相反
    // static int round = 1;
    // llvm::outs() << "---------------Round " << round++<< "---------------\n";
    DomainVal_t IDV,ODV;

    for(auto& BB : getBBConstRange(F))
    {
      IDV = getBoundaryVal(BB);
      BVs[&BB] = IDV; 
      for(auto& In : getInstConstRange(BB))
      {
        ODV = InstDomainValMap[&In];
        Changed |= transferFunc(In,IDV,ODV);
        InstDomainValMap[&In] = ODV;
        IDV = ODV;
      }
      
    }
    return Changed;
  }

  /// @}

  virtual ~Framework() {}

  virtual void InitializeDomainFromInstruction(const llvm::Instruction& In) = 0;

  /// @brief Apply the transfer function to the input domain value at
  ///        instruction @p inst .
  /// @param Inst
  /// @param IDV
  /// @param ODV
  /// @return Whether the output domain value is to be changed.
  virtual bool transferFunc(const llvm::Instruction &Inst,
                            const DomainVal_t &IDV, DomainVal_t &ODV) = 0;

  virtual AnalysisResult_t run(llvm::Function &F,
                               llvm::FunctionAnalysisManager &FAM) {

    /// @todo(CSCD70) Please complete this method.

    for(auto& In : instructions(F))
    {
      InitializeDomainFromInstruction(In);
    }

    for(auto& BB : getBBConstRange(F))
    {
      BVs[&BB] = TMeetOp().top(DomainIdMap.size());

      for(auto& In : getInstConstRange(BB))
      {
        InstDomainValMap[&In] = TMeetOp().top(DomainIdMap.size());
      }
    }


    int64_t round = 1;
    while(traverseCFG(F))
    {
      // llvm::outs() << "----------------Round " << round++ << "----------------" << "\n";
      // printInstDomainValMap(F);
    }
    // llvm::outs() << "----------------Round " << round++ << "----------------" << "\n";
    printInstDomainValMap(F);


    return std::make_tuple(DomainIdMap, DomainVector, BVs, InstDomainValMap);
  }

}; // class Framework

/// @brief For each domain element type, we have to define:
///        - The default constructor
///        - The meet operators (for intersect/union)
///        - The top element
///        - Conversion to bool (for logging)
struct Bool {
  bool Value = false;
  Bool operator&(const Bool &Other) const {
    return {.Value = Value && Other.Value};
  }
  Bool operator|(const Bool &Other) const {
    return {.Value = Value || Other.Value};
  }

  static Bool top() { return {.Value = true}; }
  operator bool() const { return Value; }
};

enum ConstantStatus{
  UNDEF = 2,
  CONST = 1,
  NAC = 0
};

struct Constant{
  int64_t Value;
  enum ConstantStatus Status = NAC;
  Constant operator&(const Constant& Other) const {
    Constant result;
    result.Status = std::min(Status,Other.Status);
    if(result.Status==CONST)
    {
      if(Status == CONST && Other.Status==CONST)
      {
        if(Value != Other.Value)
        {
          result.Status = NAC;
        }
        else
          result.Value = Value;
      }
      else if(Status == CONST)
        result.Value = Value;
      else if(Other.Status == CONST)
        result.Value = Other.Value;        
    }
    return result;
  }

  static Constant top() {return {.Status = UNDEF};}
  operator bool() const {return Status==CONST;}
};

} // namespace dfa
