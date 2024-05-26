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

    literal:string = "string_literal"; # string literal
    a:int = 12; # int literal
    a:float = 12.1; # float literal

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

Types
~~~~~~~~~
Donsus offers a various range of types for specific purposes depending on the context. Here are the accepted types::
- integer
- void
- bool
- float
- string
