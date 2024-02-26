define dso_local void @foo(i32 noundef %0, i32 noundef %1) #0 {
  %3 = add nsw i32 %0, %1
  %4 = icmp ne i32 %0, 0
  br i1 %4, label %5, label %7

5:                                                ; preds = %2
  %6 = add nsw i32 %0, 1
  br label %7

7:                                                ; preds = %5, %2
  %.0 = phi i32 [ %6, %5 ], [ %0, %2 ]
  %8 = add nsw i32 %.0, %1
  ret void
}



