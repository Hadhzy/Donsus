%struct.TYPES = type {i32, i8}

define i64 @return_multiple() {
entry:
  %retval = alloca %struct.TYPES, align 4
  %a = getelementptr inbounds %struct.TYPES, ptr %retval, i32 0, i32 0
  store i32 1, ptr %a, align 4
  %c = getelementptr inbounds %struct.TYPES, ptr %retval, i32 0, i32 1
  store i8 99, ptr %c, align 4
  %0 = load i64, ptr %retval, align 4
  ret i64 %0
}

define i32 @main(){
entry:
  %value = alloca %struct.TYPES, align 4
  %call = call i64 @return_multiple()
  store i64 %call, ptr %value, align 4
  ret i32 0
}