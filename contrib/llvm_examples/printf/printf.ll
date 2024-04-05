@.str = private unnamed_addr constant [3 x i8]  c"%d\00", align 1

define i32 @main(){
    %add = alloca i32, align 4
    store i32 10, ptr %add, align 4
    %1 = load i32, ptr %add, align 4
    %call = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %1)
    ret i32 0
}

declare i32 @printf(ptr noundef, ...) #2