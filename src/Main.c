#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "Generation.h"

static I evaluate(struct Node *descriptorPointer) {
    switch (descriptorPointer->Kind) {
    case PARSER_NODE_ADD:
        return evaluate(descriptorPointer->Left) + evaluate(descriptorPointer->Right);
    case PARSER_NODE_SUB:
        return evaluate(descriptorPointer->Left) - evaluate(descriptorPointer->Right);
    case PARSER_NODE_MUL:
        return evaluate(descriptorPointer->Left) * evaluate(descriptorPointer->Right);
    case PARSER_NODE_DIV:
        return evaluate(descriptorPointer->Left) / evaluate(descriptorPointer->Right);
    case PARSER_NODE_MOD:
        return evaluate(descriptorPointer->Left) % evaluate(descriptorPointer->Right);
    case PARSER_NODE_NEG:
        return -evaluate(descriptorPointer->Left);
    case PARSER_NODE_PAR:
        return evaluate(descriptorPointer->Left);
    case PARSER_NODE_LIT:
        return descriptorPointer->Token->Value.AsI32;
    }
}

static void printNode(struct Node *node, I ident) {
    for (I i = 0; i < ident; i++) {
        printf("\t");
    }
    printf("%d\n", node->Kind);
    if (node->Right) {
        printNode(node->Right, ++ident);
    }
    if (node->Left) {
        printNode(node->Left, ++ident);
    }
}

I main(I argc, U8 *argv[]) {
    struct Node *node = Parse("test0.mn");
    printf("%d\n", evaluate(node));
    printf("-----------------------\n");
    printNode(node, 0);
    return 0;
}