# Question 2
There are various possible ways to apply this logic.

1. `sol.py` is the python version
2. `sol1.l` `sol2.l` and `sol3.l` involves flex scanner
  - sol1.l is simply the same grammar as given in the question
  - however if you replace the EBNF grammar with a single line grammar for `decimal_lit` and `binary_litl`, you would notice that the first one has **zero or more** occurences of **[ _ ]decimal_digit** whereas the second would have **one or more** occurences of **[ _ ]binary_digit**
  - sol3.l is simply the version using buffer instead of file descriptor when lexing with yylex()