; ModuleID = 'TestCase.bc'
source_filename = "./test/TestCaseBasic.ll"

@.str = private unnamed_addr constant [19 x i8] c"%d,%d,%d,%d,%d,%d\0A\00", align 1
@.str.1 = private unnamed_addr constant [13 x i8] c"%d,%d,%d,%d\0A\00", align 1

define dso_local void @AlgebraicIdentity(i32 noundef %0) {
  %2 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %0, i32 noundef 0, i32 noundef %0, i32 noundef %0, i32 noundef %0, i32 noundef 1)
  ret void
}

define dso_local void @StrengthReduction(i32 noundef %0) {
  %2 = shl i32 %0, 1
  %3 = shl i32 %0, 6
  %4 = ashr i32 %0, 2
  %5 = ashr i32 %0, 7
  %6 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %2, i32 noundef %3, i32 noundef %4, i32 noundef %5)
  ret void
}

define dso_local void @MultiInstOpt(i32 noundef %0, i32 noundef %1) {
  %3 = add nsw i32 %0, 3
  %4 = add nsw i32 %0, %1
  %5 = call i32 (ptr, ...) @printf(ptr noundef @.str.1, i32 noundef %3, i32 noundef %0, i32 noundef %4, i32 noundef %0)
  ret void
}

declare i32 @printf(ptr noundef, ...)
