#include <stdio.h>
#include <stdlib.h>
#include "Lex.h"
#include "Parser.h"
#include "Gen_x86_64.h"

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
    case PARSER_NODE_BAND:
        return evaluate(descriptorPointer->Left) & evaluate(descriptorPointer->Right);
    case PARSER_NODE_XOR:
        return evaluate(descriptorPointer->Left) ^ evaluate(descriptorPointer->Right);
    case PARSER_NODE_BOR:
        return evaluate(descriptorPointer->Left) | evaluate(descriptorPointer->Right);
    case PARSER_NODE_AND:
        return evaluate(descriptorPointer->Left) && evaluate(descriptorPointer->Right);
    case PARSER_NODE_LOR:
        return evaluate(descriptorPointer->Left) || evaluate(descriptorPointer->Right);
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

static void gen(struct GeneratorClass *class, struct Node *node) {
    switch (node->Kind) {
    case PARSER_NODE_ADD:
        gen(class, node->Right);
        gen(class, node->Left);
        class->Add(0, 0);
        break;
    case PARSER_NODE_SUB:
        gen(class, node->Right);
        gen(class, node->Left);
        class->Sub(0, 0);
        break;
    case PARSER_NODE_MUL:
        gen(class, node->Right);
        gen(class, node->Left);
        class->Mul(0, 0);
        break;
    case PARSER_NODE_DIV:
        gen(class, node->Right);
        gen(class, node->Left);
        class->Div(0, 0);
        break;
    case PARSER_NODE_MOD:
        gen(class, node->Right);
        gen(class, node->Left);
        class->Mod(0, 0);
        break;
    case PARSER_NODE_NEG:
        gen(class, node->Left);
        class->Neg(0);
        break;
    case PARSER_NODE_LIT:
        class->Load(node->Token->Value.AsI32);
        break;
    }
}

static void compile(const char *filepath, const char *outfile) {
    struct GeneratorClass *class = Genx86_64(outfile);
    class->Prepare();
    struct Node *node = 0;
    LexOpen(filepath);
    while (1) {
        node = ParseNoLexs();
        if (!node) {
            break;
        }
        gen(class, node);
        class->EndStatement();
    }
    class->Afterwards();
    Genx86_64_Close();
    LexClose();
}

I main(I argc, U8 *argv[]) {
    compile("test0.mn", "test0.s");
    return 0;
}