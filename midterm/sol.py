import re

def is_intlit(txt: str):
    if "\n" in txt: return False
    if "\r" in txt: return False
    if "\v" in txt: return False
    if "\f" in txt: return False
    # 0 or a non-zero digit followed by any number of (underscore digit) or digit
    decimal_lit = "0|[1-9](_?[0-9])*"  
    # 0b or 0B followed by at least one occurence of (underscore digit) or digit
    binary_lit = "0(b|B)(_?[0-1])+"
    # note ^ and $ must be used to not allow whitespaces or substring matches
    return bool(re.search(f"^({decimal_lit}|{binary_lit})$", txt))
    # the below one also works: in fact 
    # return bool(re.search("^(0|[1-9](_?[0-9])*|0(b|B)(_?[0-1])+)$", txt))

testcases = [
    "1_",
    "0123",
    "1234",
    "0",
    "1_2_3",
    "0b",
    "0B",
    "0b_",
    "0B_",
    "0b_1",
    "0B_1",
    "0B_111_1",
    "0b_1234_2",
    " 1234",
    "1234\n",
    "\t0b110"

]

for test in testcases:
    print(f"{is_intlit(test)}\t{test}")