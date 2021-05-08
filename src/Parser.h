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
    PARSER_ASSO_LEFT_RIGHT,
    PARSER_ASSO_RIGHT_LEFT,
};

enum {
    PARSER_NODE_ADD,
    PARSER_NODE_SUB,
    PARSER_NODE_MUL,
    PARSER_NODE_DIV,
    PARSER_NODE_MOD,
    PARSER_NODE_LIT,
    PARSER_NODE_PAR,
    PARSER_NODE_XOR,
    PARSER_NODE_BAND,
    PARSER_NODE_BOR,
    PARSER_NODE_AND,
    PARSER_NODE_LOR,
    PARSER_NODE_NEG,
    PARSER_NODE_BNOT,
    PARSER_NODE_NOT,
};

struct Node {
    struct LexDescriptor *Token;
    struct Node *Left;
    struct Node *Right;
    I Kind;
    I Associativity;
};

struct Node *Parse(const char *filepath);
struct Node *ParseNoLexs();

#endif
