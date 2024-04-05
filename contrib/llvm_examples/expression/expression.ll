; use an expression for function call

@a = global i32 19, align 4 ; the address will be multiple of 4
@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define noundef i32 @add_some_value(){
    ret i32 10;
}

define i32 @main() {
    %c = alloca i32, align 4
    %d = alloca i32, align 4
    %call = call noundef i32 @add_some_value() ; ensure the pointer is not null or otherwise its undefined
    store i32 %call, ptr %c, align 4 ; store the function value to c
    %1 = load i32, ptr @a, align 4 ; load the global value
    %2 = load i32, ptr %c, align 4 ; load the value from call
    %add = add nsw i32 %1, %2 ; add them together, nsw to produce error when overflow happens
    store i32 %add, ptr %d, align 4 ;
    ; print it
    %call1 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %add)

    ret i32 0
}

declare i32 @printf(ptr noundef, ...) #2
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }