@0 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
inside_main:
  %a = alloca i32, align 4
  store i32 11, ptr %a, align 4
  %0 = load i32, ptr %a, align 4
  %1 = icmp sgt i32 %0, 10
  br i1 %1, label %then, label %else

then:                                             ; preds = %inside_main
  store i32 0, ptr %a, align 4
  br label %ifcont

else:                                             ; preds = %inside_main
  store i32 1, ptr %a, align 4
  br label %ifcont

ifcont:                                           ; preds = %else, %then
  %2 = load i32, ptr %a, align 4
  %3 = call i32 (ptr, ...) @printf(ptr @0, i32 %2)
  ret i32 0
}

declare i32 @printf(ptr, ...)