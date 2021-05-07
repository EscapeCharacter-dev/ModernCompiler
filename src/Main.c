#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"

static I evaluate(struct Node *descriptorPointer, I ident) {
    for (I i = 0; i < ident; i++) {
        printf("\t");
    }
    printf("%d\n", descriptorPointer->Kind);
    switch (descriptorPointer->Kind) {
    case PARSER_NODE_ADD:
        return evaluate(descriptorPointer->Left, ident++) + evaluate(descriptorPointer->Right, ident++);
    case PARSER_NODE_SUB:
        return evaluate(descriptorPointer->Left, ident++) - evaluate(descriptorPointer->Right, ident++);
    case PARSER_NODE_MUL:
        return evaluate(descriptorPointer->Left, ident++) * evaluate(descriptorPointer->Right, ident++);
    case PARSER_NODE_DIV:
        return evaluate(descriptorPointer->Left, ident++) / evaluate(descriptorPointer->Right, ident++);
    case PARSER_NODE_MOD:
        return evaluate(descriptorPointer->Left, ident++) % evaluate(descriptorPointer->Right, ident++);
    case PARSER_NODE_LIT:
        return descriptorPointer->Token->Value.AsI32;
    }
}

I main(I argc, U8 *argv[]) {
    struct Node *node = Parse("test0.mn");
    printf("%d\n", evaluate(node, 0));
    return 0;
}