# CPSC411-GoLF



## Visit
 - **scanner**: `ms1` folder
   - using _flex_ for lexical analyzer
 - **parser**: `ms2` folder
   - using _bison_ for LALR parser
 - **semantic analyzer**: `ms3` folder
   - tree traversal to perform define and lookup of var/functions declarations (allows to detect any undefined or redefined declarations)
   - type checking not done at the moment
 - **code generator**: `codegen_mips` folder
   - tree traversal to produce mips assembly code
   - strategy: is structured as follows
     - the code uses `$t0` register as a return for evalExpression (which evaluates an expression)
     - the code uses stack memory and at most 2 register for a naive approach. This allows to handle any amount of arguments passed in function call without issues, and allows simple but effective code generation
     - the code involves mainly 3 functions
       - **trav**: which traverses through top level declarations (gloval variables and function declarations)
       - **applyBlock**: which generates code for a Block statement
       - **evalExpression**: which generates code for an Expression. Note that it may also branch to a passed true or false label, which is necessary for applyBlock in statements such as `if`, `for` statements.
    - `TEMPLATE.s` file includes predefined code for functions that are in universe block/scope and error checking utilities functions
      - since, all variables and function declarations in user input **GoLF** file are nested in file block/scope or after it (thus universe block is not the outermost scope), the declarations will not conflict with TEMPLATE.s as all of them contain the suffix `_file`.
      - **getProvenienceStack** may be visited in `semantic.h` and `semantic.c` to understand how unique labels are generates for all variable and function declarations.
