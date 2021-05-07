#ifndef MODERNC_PARSER_H
#define MODERNC_PARSER_H

#include "Lex.h"

struct ParseDescriptor {
    struct LexDescriptor *Operands;
    struct LexDescriptor *Operators;
    I OperandsLength;
    I OperatorsLength;
};

enum {
    PARSER_NODE_ADD,
    PARSER_NODE_SUB,
    PARSER_NODE_MUL,
    PARSER_NODE_DIV,
    PARSER_NODE_MOD,
    PARSER_NODE_LIT,
};

struct Node {
    struct LexDescriptor *Token;
    struct Node *Left;
    struct Node *Right;
    I Kind;
};

struct Node *Parse(const char *filepath);

#endif
