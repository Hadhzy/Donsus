.. _reference:


Reference
~~~~~~~~~~~~

---

.. _lexical_analysis:

Logical lines
~~~~~~~~~~~~~

A Donsus program is divided into a number of logical lines.

The end of a logical line is represented by the token SEMI_COLON.
Donsus is not a new line sensitive language thus indentations do not apply.

Comments
~~~~~~~~
There are 2 possible ways of commenting in Donsus. A comment is solely informing the
compiler the line should be ignored - and it serves as an informative documentation.
For Example::

    # single line comment
    /*
    THIS IS IGNORED
    AS WELL AS THIS
    */

.. _identifiers-keywords:

Identifiers and keywords
~~~~~~~~~~~~~~~~~~~~~~~~~
The following identifiers are used as reserved keywords of donsus, and cannot be used as ordinary identifiers. They must be spelled exactly as here::

    if        elif     else
    return    printf   true
    false     while    for

These keywords are used to define the structure of the program and cannot be used as identifiers. 
We are going to go through each of them in the following sections.

.. _types_in_donsus:

Types
~~~~~
Donsus offers various types, the following are the types supported by donsus::

- integer
- void
- bool
- float
- string
- unary

Here are the keywords used to represent these types in donsus::

    int    int8    int16
    int32  int64   u32
    u64    f32     f64
    bool   void    string

Let's discuss each of them seperately.

Integer types
-------------
Integers are used to represent whole numbers. In donsus there are multiple types of integers supported. The following are the integer types supported by donsus::

    int    int8    int16
    int32  int64   u32
    u64

- **int**  is used to represent a 32 bit signed integer. 
- **int8** is used to represent an 8 bit signed integer. 
- **int16** type is used to represent a 16 bit signed integer. 
- **int32** type is used to represent a 32 bit signed integer.
- **int64** type is used to represent a 64 bit signed integer. 
- **u32** type is used to represent a 32 bit unsigned integer. 
- **u64** type is used to represent a 64 bit unsigned integer.

Float types
-----------
Floats are used to represent decimal numbers. In donsus there are multiple types of floats supported. The following are the float types supported by donsus::

    f32    f64

- **f32** is used to represent a 32 bit float.
- **f64** is used to represent a 64 bit float.

Boolean types
-------------

Booleans are used to represent true or false values. In donsus there is only one type of boolean supported. The following is the boolean type supported by donsus::

    bool

- **bool** is used to represent a boolean value. It can either be true or false.

String types
------------

Strings are used to represent text. In donsus there is only one type of string supported. The following is the string type supported by donsus::

    string

- **string** is used to represent a string value e.g "hello world".

Void types
----------

Void is used to represent the absence of a value. In donsus there is only one type of void supported. The following is the void type supported by donsus::

    void

- **void** is used to represent the absence of a value e.g return type of a function is void if the function does not return a value.

.. _escaped-sequences:

Escape sequences
~~~~~~~~~~~~~~~~

Escape sequences in string(s) are interpreted according to rules similar to those used by Standard C. The recognised escape sequences are


>>> Here the printf function will flush  out an extra newline after displaying the input of the function.

.. _literals:

Literals
~~~~~~~~~
Literals are explicitly provided constant values assigned to variables. They
represent fixed values that cannot be modified.
There are multiple types of literal each of which corresponding to a **DONSUS_TYPE**::

    literal:type = value;

String literals
----------------
String literals are sequences of characters enclosed in double quotes. They represent string data.
The following are examples of string literals::

    a:string = "Hello World"; # string literal


Integer literals
----------------
Integer literals are whole numbers without a decimal point. They represent integer data.
The following are examples of integer literals::

    a:int = 12; # int literal
    b:u32 = 12; # u32 literal

Float literals
--------------
Float literals are numbers with a decimal point. They represent floating point data.
The following are examples of float literals::

    a:float = 12.1; # float literal

Boolean literals
----------------
Boolean literals are either true or false. They represent boolean data.
The following are examples of boolean literals::

    a:bool = true; # bool literal

As of now Donsus doesn't support character literals and null literals.


.. _operators:

Operators
~~~~~~~~~
These are the following operators in Donsus::


    +     +=     -
    -=    *     *=
    /     /=    >
    >=    <=    =
    ==    !=    --
    ++


These include both binary and assignment operators.



.. _arrays:

Arrays
~~~~~~

Arrays are a collection of elements of the same type. In donsus there are three types of arrays supported:

    - static array(read only array with fixed size) 
    - dynamic array
    - fixed sized array

We are going to discuss each of them with examples. 


Static Arrays
-------------

In donsus, static arrays are read only arrays with fixed size. The size of the array is determined at compile time. The syntax for defining a static array is as follows::

    a:int[5]. = [1,2,3,4,5];

It is very important to notice the dot after the size of the array. This is used to differentiate between **static** and **dynamic** arrays. 
Static arrays are read only arrays. This means that you cannot change the size of the array or the elements of the array. Some other examples include::

    b:float[5]. = [1.0,2.0,3.0,4.0,5.0];
    c:string[5]. = ["a","b","c","d","e"];
    d:bool[5]. = [true,false,true,false,true];

Dynamic Arrays
--------------
Dynamic arrays are arrays whose size can be changed at runtime. Meaning you can add or remove elements from the array and the size does not need to be provided explicitly. The syntax for defining a dynamic array is as follows::

    a:int[] = [1,2,3,4,5];

The above code declares a dynamic array of integers with 5 elements. Some other examples include::

    b:float[] = [1.0,2.0,3.0,4.0,5.0];
    c:string[] = ["a","b","c","d","e"];
    d:bool[] = [true,false,true,false,true];

Fixed Sized Arrays
------------------
Fixed sized arrays are arrays whose size is determined at runtime but cannot be changed. Unlike static arrays elements of fixed sized arrays can be changed only the size cannot. The syntax for defining a fixed sized array is as follows::

    a:int[5] = [1,2,3,4,5];

The above code declares a fixed sized array of integers with 5 elements. It is important to notice that there is no dot after the size of the array. Some other examples include::

    b:float[5] = [1.0,2.0,3.0,4.0,5.0];
    c:string[5] = ["a","b","c","d","e"];
    d:bool[5] = [true,false,true,false,true];

.. _functions_in_donsus:

Functions
~~~~~~~~~

Functions are a block of code that performs a specific task that are used to break down the code into smaller and modular pieces. In donsus functions are defined using the keyword **def** followed by the function name and the parameters in parentheses. The syntax for defining a function is as follows::

    def function_name(parameter1:DONSUS_TYPE,parameter2:DONSUS_TYPE,...) -> DONSUS_TYPE {
        # code block
    }

    def function_name_declare(parameter1:DONSUS_TYPE,parameter2:DONSUS_TYPE,...) -> DONSUS_TYPE;

The return type of the function is specified after the arrow **->** and the types for the parameters have to be seperated by a colo(aka **DONSUS_COLO**). Accepted return types are: :ref:`types`. The function can return a value of the specified type. If the function does not return a value the **void** return type should be used. The function can be called by using the function name followed by the parameters in parentheses. The syntax for calling a function is as follows::

    function_name(argument1,argument2,...);

Both the arguments and the parameters are seperated by commas.
As of now donsus doesn't support function overloading however it supports nested functions. This means that you cannot have two functions with the same name but different parameters in donsus.


In regards of the function's body, donsus supports the following statements::

    - if
    - elif
    - else
    - while
    - for
    - return
    - printf

We are going to talk about all of them in the following sections.

Some examples for the usage of functions in donsus are as follows::

    def add(a:int,b:int) -> int {
        return a+b;
    }

    def nested_function() -> void {
        def nested_function2() -> void {
            printf("This is a nested function");
        }
        nested_function2();
    }

    def if_else(a:int) -> void {
        if(a>0) {
            printf("a is greater than 0");
        } elif (a==0) {
            printf("a is less than or equal to 0");
        } else {
            printf("a is less than 0");
        }
    }

    def while_loop(a:int) -> void {
        while(a>0) {
            printf("a is greater than 0");
            a = a - 1;
        }
    }

    def for_loop(a:int) -> void {
        for number: 2..10 {
            printf(number);
        }   
    }

.. _compound_statements_in_donsus:

Simple and Compound Statements
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If-elif-else Statements
-----------------------
In general the if-elif-else statement is used to make decisions in the code. The syntax for the if-elif-else statement is as follows::

    if(DONSUS_EXPRESSION) {
        # code block
    } elif(DONSUS_EXPRESSION) {
        # code block
    } else {
        # code block
    }

**DONSUS_EXPRESSION** is a combination of operands and operators within donsus. This is a core element of donsus which is out of the context of this reference, however here it is a placeholder for a boolean expression. 
The condition has to be provided in parentheses. The body of the if-elif-else statement accepts the same statements as the function body.

The code block is executed if the expression evaluates to true. If the expression evaluates to false the code block is skipped. The elif and else blocks are optional. The elif block is executed if the if block evaluates to false and the elif expression evaluates to true. The else block is executed if the if and elif blocks evaluate to false.


Some examples for the usage of if-elif-else statements in donsus are as follows::
    
    def if_else(a:int) -> void {
        if(a>0) {
            printf("a is greater than 0");
        } elif (a==0) {
            printf("a is less than or equal to 0");
        } else {
            printf("a is less than 0");
        }
    }

    if_else(5); # prints "a is greater than 0"
    if_else(0); # prints "a is less than or equal to 0"
    if_else(-5); # prints "a is less than 0"

.. _while:

While Loops 
-----------
The while loop is used to execute a block of code repeatedly as long as the condition specified in the loop is true. The syntax for the while loop is as follows::

    while(DONSUS_EXPRESSION) {
        # code block
    }

**DONSUS_EXPRESSION** Appears in the same context as in the if-elif-else statement. The code block is executed as long as the expression evaluates to true. If the expression evaluates to false the loop is exited and the code block is skipped.
The condition has to be provided in parentheses. The body of the while loop accepts the same statements as the function body.

Some examples for the usage of while loops in donsus are as follows::

    def while_loop(a:int) -> void {
        # a must be defined prior to the execution
        while(a>0) {
            printf("a is greater than 0");
            a = a - 1;
        }
    }

    while_loop(5); # prints "a is greater than 0" 5 times

.. _for:

For Loops
---------
The for loop is used to execute a block of code repeatedly. In donsus there are two types of for loops. The first type is the for loop that iterates over a range of numbers. The second type is the for loop that iterates over an array. 
The init statement(counter variable) is automatically created prior to the execution of the loop.identifiers
Let's discuss each of them separately.

For Loops Over a Range of Numbers
==================================

The syntax for the for loop that iterates over a range of numbers is as follows::

    for variable: start..end {
        # code block
    }

First of all after the **for** keyword the variable that is used to iterate over the range of numbers **can** be provided, if it is not provided donsus will set the variable **it** as default. The range of numbers is specified by the **start** and **end** values seperated by two dots. 

Both the **start** and **end** values have to be integers. The code block is executed for each number in the range of numbers. 

The body of the for loop accepts the same statements as the function body.

Some examples for the usage of for loops over a range of numbers in donsus are as follows::

    def for_loop(a:int) -> void {
        for number: 2..10 {
            printf(number);
        }   
    }

    for_loop(5); # prints 2 3 4 5 6 7 8 9 10

For Loops Over an Array
========================
The syntax for the for loop that iterates over an array is as follows::

    for variable: array {
        # code block
    }

First of all after the **for** keyword the variable that is used to iterate over the array **can** be provided, if it is not provided donsus will set the variable **it** as default. The array is specified by the **array** value which has to refer to a valid array. 

The code block is executed for each element in the array. 

The body of the for loop accepts the same statements as the function body.

Some examples for the usage of for loops over an array in donsus are as follows::

    def for_loop_array() -> void {
        a:int[] = [1,2,3,4,5];
        for number: a {
            printf(number);
        }   
    }

    for_loop_array(); # prints 1 2 3 4 5

For Loops Without Iterator variable
===================================
As mentioned above, the iterator variable can be omitted and it will be inserted
automatically in the form of a temporary variable - called: *it*.::

    def for_loop_array() ->void {
        a:int[] = [1, 2, 3, 4, 5];
        for a {
            printf(it);
        }
    }

    for_loop_array(); # prints 1 2 3 4 5

.. _return:
    
Return Statement
----------------
The return statement is used to return a value from a function. The syntax for the return statement is as follows::

    return DONSUS_EXPRESSION;

**DONSUS_EXPRESSION** Appears in the same context as in the if-elif-else statement. The return statement is used to return a value from a function. The return type of the function has to match the type of the expression. If the return statement is not provided the function have return **void**. 
There must be a semicolon at the end of the return statement.

Some examples for the usage of return statements in donsus are as follows::

    def add(a:int,b:int) -> int {
        return a+b;
    }

    def subtract(a:int,b:int) -> int {
        return a-b;
    }

    def multiply(a:int,b:int) -> int {
        return a*b;
    }

    def divide(a:int,b:int) -> int {
        return a/b;
    }

.. _printf:

Printf Statement
----------------
The printf statement is used to print a value to the terminal. The syntax for the printf statement is as follows::

    printf(DONSUS_EXPRESSION);

**DONSUS_EXPRESSION** Appears in the same context as in the if-elif-else statement. The printf statement is used to print a value to the terminal. The value can be a anything within the boundaries of a donsus expression.

Some examples for the usage of printf statements in donsus are as follows::

    def print_hello_world() -> void {
        printf("Hello World");
    }

    def print_number(a:int) -> void {
        printf(a);
    }

    def print_float(a:float) -> void {
        printf(a);
    }

    def print_string(a:string) -> void {
        printf(a);
    }

    def print_bool(a:bool) -> void {
        printf(a);
    }



.. _expressions_in_donsus:

Expressions
~~~~~~~~~~~

Expressions are the building blocks of donsus. They are used to perform operations on variables and values. There are multiple types of expressions in donsus, they all have the type **DONSUS_EXPRESSION**. 
We are going to talk about the presence of expressions in donsus and what are the accepted types of expressions. 

Number Expressions
------------------

Number expressions are used to perform operations on numbers. The following operators are supported for number expressions and any combination of expressions in general::

    +     +=     -
    -=    *     *=
    /     /=    >
    >=    <    <=
    ==    !=    

This list also includes comparison operators. The comparison operators are used to compare two expressions. The comparison operators return a boolean value and can be used in statements::

    >     >=    <    <=
    ==    !=

Donsus also supports **float expressions** and **unary expressions**. Some examples for the usage of number expressions in donsus are as follows::

    def add(a:int,b:int) -> int {
        return a+b;
    }

    def subtract(a:int,b:int) -> int {
        return a-b;
    }

    def multiply(a:int,b:int) -> int {
        return a*b;
    }

    def divide(a:int,b:int) -> int {
        return a/b;
    }

    def compare(a:int,b:int) -> bool {
        return a>b;
    }

    def compare_float(a:float,b:float) -> bool {
        return a>b;
    }

    def unary(a:int) -> int {
        return -a;
    }

    def unary_float(a:float) -> float {
        return -a;
    }

String Expressions
------------------

String expressions are used to perform operations on strings. In donsus here is how to define a string expression::

    a:string = "Hello";
    b:string = "World";
    c:string = a+b;

It's important to note that the operators mentioned before apply to all expressions and the combination of expressions as long as they used correctly.
Some examples for the usage of string expressions in donsus are as follows::

    def concat(a:string,b:string) -> string {
        return a+b;
    }

    def compare_string(a:string,b:string) -> bool {
        return a==b;
    }

    def compare_string_not_equal(a:string,b:string) -> bool {
        return a!=b;
    }

Boolean Expressions
-------------------

In donsus a bool expression can either be **true** or **false**. These are keywords in donsus specifically reserved for boolean expressions. 
Boolean expressions must be used with the **bool** type. Here is how to use them in donsus::

    a:bool = true;
    b:bool = false;


Operator precedence in Expressions
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The following table lists the precedence of operators in donsus, from highest to lowest. Operators in the same box have the same precedence.

.. list-table:: Title
   :widths: 100 100
   :header-rows: 1

   * - Operators
     - Precedence   
   * - **>=**, **>**, **<=**, **<**, **!=**, **==**
     - 1
   * - **+**, **-**
     - 10
   * - * , **/**
     - 20
