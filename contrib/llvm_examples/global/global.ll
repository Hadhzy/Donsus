@c = global i32 5, align 4

define i32 @main() {

%1 = alloca i32, align 4
store i32 0, ptr %1, align 4

%2 = load i32, ptr @c, align 4 ; load global so that we can do something with it
; do whatever

ret i32 0
}