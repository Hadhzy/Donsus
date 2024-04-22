The goal of this page is to get the **terminologies** right before reading the language reference.
As you might be aware - there are a lot of standards and documents describing programming languages - as one might
except one term
might correspond to lot others or even means something totally different in other contexts. **This glossary presents how
Donsus interprets**
these **definitions** and gives an **introduction to the language**.

Using the correct terminologies often has a huge significance in being able to describe
problems properly and most likely leads to the best efficient answer possible when asking questions or discussing ideas.

### Table of contents:

1. [Statements](#statements)
2. [Functions](#functions)
3. [Built-in](#built-in)
4. [Expressions](#expressions)
5. [Objects](#objects)
6. [Types](#types)

---

### Donsus

Donsus is the name of the project and the language itself - also representing the name of the compiler which implements
the idea itself.

---

### Statements

As in most programming languages statements are fragments of the program that are executed
in sequence.

Donsus includes the following types of statements:

#### expression statements:

This category includes:

- assignments
- function calls

```
# ...
a += 12;  # expression statement - simple assignment
call_me(); # expression statement - function call without required arguments
```

An expression statement is an expression followed by a semicolon.
They do not introduce identifiers into a block.

*Assignment statement*:
An assignment statement sets and/or resets teh value stored in the storage location(s)
denoted by a variable name.

```
a += 12;
```

*function_call:*
In Donsus function calls invoke/execute functions.

```
test();
```

#### compound statements:

A compound statement, or block, is a brace-enclosed sequence of statements and declarations.

```
if (value > 1){
    
}
```

Here,

```
{
...
}
```

the brackets denote a compound statement.
They do not require semicolons to be finished off with.

#### selection statements:

Selection statement, e.g `if` provide a means to conditionally execute sections of code.

```
if(condition) {
}
```

`Condition` and `guard` can be used interchangeably to refer to the same definition.
An `invariant` is a specific guard which is always true.

#### declaration statements:

Declarations are how names are introduced or re-introduced into the program.
Definitions are declarations that fully define the entity introduced by the declaration.
This category includes:

- Function definitions/declarations
- Variable definitions/declarations

```
a() ->int; # function declaration
b: string; # variable declaration

# function definition
def c() -> int{
    return 3;
}

d:string = "hello world"; # variable definition
```

Notice, how each of these lines except `compund statements` require semicolons at the end of their definitions.

[Declaration statements](#declaration-statements) share the same behaviour
with [expression statements](#expression-statements) in their semantic appearance.


---

### Functions

#### parameters

Parameters determine the arguments that can be specified when the function is called.

```
# function parameters: a:int, b:int, c:int
def name(a:int, b:int, c:int) -> int{
    ...
 }
```

Notice how the parameters are following the same syntax as [variable-declarations](#declaration-statements).

#### arguments

Arguments are the values that are passed into functions.

```
# function arguments: 1, 2, 3
func_call(1, 2, 3);
```

Functions are entities that associate a sequence of [statements](#statements) with a name and a list
of [parameters](#parameters).

```
def name() -> int {
    printf("hello from function");
}
``` 

The `name` function associates/binds an [expression statement](#expression-statements), namely printf to its function
body.
Printf is a specific entity and [built-in](#built-in) - inside the language.
--- 

### Built-in

#### Built-in functions:

refer to those pre-defined functions that come along with a programming language.

In Donsus, at least as of now - it means that it is part of the language syntax and has a specific meaning.
e.g *printf*.
---

### Expressions

#### r-value:

An r-value expression is not an l-value expression. It does not introduce a
new symbol into the program

```
 34 + 2; # here 34+2 is an r-value expression.
```

#### l-value:

A lvalue does introduce a new symbol into the programming as well as always defines
a region of storage, so you can take its address.

```
a: int = 2; # here "a" is an lvalue, introducing a new symbol to the language.
```

---

### Objects

---

### Types

### implicit

Implicit is often used to refer to something that's done by the compiler
and the user has no control over.

#### implicit conversion:

In Donsus, we highly against implicit conversion and try to avoid it almost everywhere.
Basically implicit type conversion is an automatic type conversion done by the compiler.
This often results in unsafe code and produces unexpected results.

### explicit

Explicit is the manual approach to accomplishing the change
you wish to have by writing out hte instructions to be done explicitly.

