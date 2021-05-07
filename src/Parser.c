#include "Parser.h"
#include <stdlib.h>
#include <string.h>

static struct Node *CreateNode(struct Node *left, struct Node *right, struct LexDescriptor *descriptor, I kind) {
    struct Node *node = malloc(sizeof(struct Node));
    node->Left = left;
    node->Right = right;
    node->Token = descriptor;
    node->Kind = kind;
    return node;
}

static struct Node *ParsePrimary() {
    struct LexDescriptor *descriptor = LexNext();
    return CreateNode(0, 0, descriptor, PARSER_NODE_LIT);
}

static struct Node *ParseNode() {
    struct Node *right, *left;
    struct LexDescriptor *operator;
    left = ParsePrimary();
    operator = LexNext();
    right = ParsePrimary();
    I kind;
    switch (operator->Kind) {
    case LEX_TOK_PLUS:
        kind = PARSER_NODE_ADD;
        break;
    case LEX_TOK_MINUS:
        kind = PARSER_NODE_SUB;
        break;
    case LEX_TOK_STAR:
        kind = PARSER_NODE_MUL;
        break;
    case LEX_TOK_SLASH:
        kind = PARSER_NODE_DIV;
        break;
    case LEX_TOK_PERCENT:
        kind = PARSER_NODE_MOD;
        break;
    }
    return CreateNode(left, right, 0, kind);
}

struct Node *Parse(const char *filepath) {
    LexOpen(filepath);
    struct Node *node = ParseNode();
    LexClose();
    return node;
}

/*struct ParseDescriptor Parse(const char *filepath) {
    LexOpen(filepath);
    struct LexDescriptor *operandsStack = calloc(256, sizeof(struct LexDescriptor)),
                         *operatorsStack = calloc(256, sizeof(struct LexDescriptor));
    U operandsIndex = 0, operatorsIndex = 0;
    
    struct LexDescriptor current;
    while (1) {
        LexNextDescriptor(&current);
        
        if (current.Kind == LEX_TOK_EOF) {
            break;
        }
        
        switch (current.Kind) {
        case LEX_TOK_LINT:
            memcpy(&(operandsStack[operandsIndex++]), &current, sizeof(struct LexDescriptor));
            break;
        case LEX_TOK_PLUS:
        case LEX_TOK_MINUS:
        case LEX_TOK_STAR:
        case LEX_TOK_SLASH:
            memcpy(&(operatorsStack[operatorsIndex++]), &current, sizeof(struct LexDescriptor));
            break;
        }
    }
    LexClose();
    struct ParseDescriptor descriptor;
    descriptor.Operands = operandsStack;
    descriptor.Operators = operatorsStack;
    descriptor.OperandsLength = operandsIndex + 1;
    descriptor.OperatorsLength = operatorsIndex + 1;
    return descriptor;
}*/