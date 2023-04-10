#include "semantic.h"

struct Stack stackstab;
void pass1(ASTNode * asttree);
void pass2(ASTNode * asttree);
void pass3(ASTNode * asttree);
void pass4(ASTNode * asttree);

void semantic(ASTNode * asttree) { // should annotate tree passed by ref
    pass1(asttree);    
    pass2(asttree);    
    pass3(asttree);    
    pass4(asttree);    
}


// TODO
void pass1(ASTNode * asttree) {

}
void pass2(ASTNode * asttree) {

}
void pass3(ASTNode * asttree) {

}
void pass4(ASTNode * asttree) {

}
