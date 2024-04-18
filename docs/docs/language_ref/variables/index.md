## Variables

Donsus supports the creating of both mutable and immutable objects. 

### immutable:
Immutable is an object whose state cannot be modified after it is created.
This is often denoted by the `const` keyword in other programming languages.

### mutable:
Mutable is an object whose state can be modifier after it has been created. In Donsus
mutable variables can be easily created - as the code represents below:
```
a: int = 20;
```
```
var_decl: DONSUS_NAME DONSUS_COLON donsus_type
var_def:  DONSUS_NAME DONSUS_COLON donsus_type DONSUS_EQUAL expression
```

`DONSUS_NAME:` The name of the variable must be a valid DONSUS identifier - meaning it can't start
with an underscore

