define void @test(){
entry:
    ret void
}
define i32 @main() {
entry:
    call void @test();
    ret i32 0
}