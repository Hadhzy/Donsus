@array = dso_local global [4 x i32] [i32 1, i32 2, i32 3, i32 4], align 16
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define dso_local noundef i32 @main() #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 34, ptr getelementptr inbounds ([4 x i32], ptr @array, i64 0, i64 3), align 4
  %2 = load i32, ptr getelementptr inbounds ([4 x i32], ptr @array, i64 0, i64 3), align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2)
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }