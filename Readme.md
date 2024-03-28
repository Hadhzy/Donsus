# Donsus
Donsus is a compiled, statically typed, objected oriented langauge focusing on low level programming and AI acceleration.

The ideas that make Donsus: [Ideas](https://github.com/Hels15/Donsus/blob/main/ideas.txt)

Donsus is currently under heavy development and yet not ready to be used. 
> Donsus is implemented in c++

## Dependencies:

- LLVM
- GoogleTest(for developers)

**This tutorial doesn't mean to be by any means a complete documentation of the language, it solely
serves a small introduction.**

## Variables
```
a: int = 12; # comment here
a: int;  # comment here
```
When creating variables, providing types is *mandatory* as Donsus `does not support 
any form of type inference yet`, except from identifying expression types. The types of the variables must be known at *compile time.*

In Donsus you can select from a variety of different types(not fully completed).
These types can be used together to form a well design infrastructure:

- INT(signed integer)
- I32(32 bit signed integer)
- U64(64 bit unsigned integer)
- CHAR
- I8 (8 bit integer)
- I64
- BOOL
- I16 (16 bit integer)
- U32(32 bit unsigned integer)
- VOID

The type system of Donsus can be found [here](https://github.com/Hels15/Donsus/blob/main/src/type.cc).

**Donsus doesn't allow redefinition of variables(ODR)**

One can declare a variable in donsus by using the following syntax:
```
a:int;
```

Assigning a value to a variable:
```
# assign value after defition or declaration
a:int = (12 + 4 / 3) + (a / b) + func_calL();` 
```
Donsus is not a whitespace sensitive language, 
thus end of the statements must be followed by a semicolon.

Extra information can be added by the use of comments which are ignored by the language.

Multiline comments can be performed by:

```
/* 
I am a multi line comment
*/

# I'm a single comment
```

## Function declarations:
```
# 1 - with parameters
a(int: a) ->int;
```

## Function definition:
To denote function definition one must use the `def` keyword as part of donsus followed by the following:
```
    def a() -> int {}
```

**Note: Compound statement do not require semicolon at the end of their definition.**
Similarly to declarations there must be a return type provided, and to denote the scope of the function one must use curly brackets. 
After the user have denoted the end of the parameter list, it must be followed by a `->` (`DONSUS_ARROW`) for the return type. 
Once the brackets have been initiated the function body can be filled up like the following:

```
 def a() -> int {
    # Function body
 }
```
This syntax is similar to javascript and c++. 

To denote the return statement within a function one must use the `return` keyword as part of donsus keywords followed by an expression. 

Consider this:
```
def a() -> int {
    return 2+3;
}
```
Consequently the return type of the function and the returned expression must match.

Additionally donsus supports nested functions. 

Consider this:
```
def a() -> int {
    def b() -> void {
        # function body
    }   
    return 2+3;
}
```

## Function calls

Function calls are quite simple and straightforward in Donsus: 
```
    function_call()
```
A function call must match with the desired function to call followed by parenthesis to denote the argument list. 

Arguments can be passed into the function in the following manner: 

```
    function_call(a,b,"c", 2+3, another_function());
```


## Statements
Statements are fragments of the C++ program that are executed in sequence.
First we'll cover `selection statements`:

Let's consider the following code below:
```
if (condition | guard){
     # some code here
} elif (condition | guard){
    # some code here
} else {
 # some code here
}
```
Similar to other languages these constructs can be used to bring decisions to the program
whether it is a guard(invariant) or an arbitrary given condition.

The condition clause must hold an expression that can be converted into a boolean, Donsus
is strictly disregards/against implicit conversion. An expression either can be converted, thus 
understood as a boolean or not.

## Printing
```
print("hello world");
```
The print feature can be thought as a function with arguments passed in for
the output stream. The print function can flush the buffer to the output stream if the 
given argument has a string representation, if this case doesn't hold, an error occurs.


## Exceptions
In Donsus, exceptions are a way of responding to the occurrence of exceptions or 
conditions requiring special processing during the execution of a program. Generally 
speaking exceptions occur when a Donsus program can't terminate successfully due to the presence
of a user defined feature the language can't understand.

Ex:
```
a: int = 32;
a: int = 22;

-> RedefinitionException("can't redefine an already existing object")
```


Test framework:

To reassure the validity of donsus, we carry out a number of tests to make sure that everything is working as intended. Donsus uses the Google test framework,
and its tests are listed within the `donsus_test` folder. Donsus tests the language both synthetically and semantically. 







