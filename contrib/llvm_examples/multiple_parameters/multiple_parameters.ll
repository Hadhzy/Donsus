@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

define i32 @func(i32, i32){
    %3 = alloca i32, align 4
    %4 = alloca i32, align 4
    %5 = alloca i32, align 4 ; for return value

    ; load the parameters into local stack
    store i32 %0, ptr %3
    store i32 %1, ptr %4

    ; load first 2 local vars
    %6 = load i32, ptr %3, align 4
    %7 = load i32, ptr %4, align 4
    ; add them together
    %8 = add nsw i32 %6, %7

    store i32 %8, ptr %5, align 4 ; once we stored it we need to load it again

    %9 = load i32, ptr %5, align 4
    ret i32 %9
}

define i32 @main() {
    %1 = alloca i32, align 4
    %2 = alloca i32, align 4

    ; store value into local one
    store i32 0, ptr %1, align 4

    %3 = call noundef i32 @func(i32 noundef 12, i32 noundef 12)
    store i32 %3, ptr %2, align 4

    %4 = load i32, ptr %2, align 4 ; load it before print
    %5 = call i32(ptr, ...) @printf(ptr noundef @.str, i32 noundef %4)
    ret i32 0
}

declare i32 @printf(ptr noundef, ...)