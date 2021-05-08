#include "Parser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static struct Node *CreateNode(struct Node *left, struct Node *right, struct LexDescriptor *descriptor, I kind, I associativity) {
    struct Node *node = malloc(sizeof(struct Node));
    node->Left = left;
    node->Right = right;
    node->Token = descriptor;
    node->Kind = kind;
    node->Associativity = associativity;
    return node;
}

static struct Node *ParsePrimary();
static struct Node *ParsePrecedence(I ptp);

static I Arithm(I operator) {
    switch (operator) {
    case LEX_TOK_PLUS:
        return PARSER_NODE_ADD;
    case LEX_TOK_MINUS:
        return PARSER_NODE_SUB;
    case LEX_TOK_STAR:
        return PARSER_NODE_MUL;
    case LEX_TOK_SLASH:
        return PARSER_NODE_DIV;
    case LEX_TOK_PERCENT:
        return PARSER_NODE_MOD;
    case LEX_TOK_PIPE:
        return PARSER_NODE_BOR;
    case LEX_TOK_AMPERSAND:
        return PARSER_NODE_BAND;
    case LEX_TOK_HELM:
        return PARSER_NODE_XOR;
    case LEX_TOK_AMPERSANDS:
        return PARSER_NODE_AND;
    case LEX_TOK_PIPES:
        return PARSER_NODE_LOR;
    }
}

static I ArithmUPrefix(I operator) {
    switch (operator) {
    case LEX_TOK_MINUS:
        return PARSER_NODE_NEG;
    case LEX_TOK_BANG:
        return PARSER_NODE_NOT;
    case LEX_TOK_TILDA:
        return PARSER_NODE_BNOT;
    }
}

static struct Node *ParsePrefixUnaryPrecedence(I ptp) {
    struct Node *left;
    struct LexDescriptor *descriptor;
    I precedence;
    descriptor = LexNext();
    switch (descriptor->Kind) {
    case LEX_TOK_MINUS:
    case LEX_TOK_BANG:
    case LEX_TOK_TILDA:
        precedence = 7;
        break;
    default:
        fprintf(stderr, "Error when parsing: Invalid unary prefix operator %d (ID)\n", descriptor->Kind);
        exit(0);
    }
    
    if (descriptor->Kind == LEX_TOK_SEMI || descriptor->Kind == LEX_TOK_EOF) {
        fprintf(stderr, "Error when parsing: Premature end of file or semicolon\n");
        exit(0);
    }
    
    while (precedence > ptp) {
        left = ParsePrecedence(precedence);
        if (!left) {
            fprintf(stderr, "Error when parsing: Premature end of file in node operand (unary prefix)\n");
            exit(0);
        }
        left = CreateNode(left, 0, descriptor, ArithmUPrefix(descriptor->Kind), PARSER_ASSO_RIGHT_LEFT);
        struct LexDescriptor *peeking = LexPeekNext();
        if (peeking->Kind == LEX_TOK_SEMI) {
            return left;
        }
        free(peeking);
    }
    return left;
}

static struct Node *ParsePrecedence(I ptp) {
    struct Node *left, *right;
    struct LexDescriptor *descriptor;
    descriptor = LexPeekNext();
    if (descriptor->Kind == LEX_TOK_EOF) {
        free(descriptor);
        descriptor = LexNext();
        free(descriptor);
        return 0;
    }
    free(descriptor);
    left = ParsePrimary();
    descriptor = LexNext();
    if (descriptor->Kind == LEX_TOK_SEMI) {
        return left;
    }
    I precedence;
    switch (descriptor->Kind) {
    case LEX_TOK_PIPES:
        precedence = 1;
        break;
    case LEX_TOK_AMPERSANDS:
        precedence = 2;
        break;
    case LEX_TOK_PIPE:
        precedence = 3;
        break;
    case LEX_TOK_AMPERSAND:
        precedence = 4;
        break;
    case LEX_TOK_PLUS:
    case LEX_TOK_MINUS:
        precedence = 5;
        break;
    case LEX_TOK_STAR:
    case LEX_TOK_SLASH:
    case LEX_TOK_PERCENT:
        precedence = 6;
        break;
    case LEX_TOK_EOF:
        fprintf(stderr, "Error when parsing: Premature end of file as operator\n");
        exit(0);
    default:
        fprintf(stderr, "Error when parsing: Unexpected operator %d (ID)\n", descriptor->Kind);
        exit(0);
    }
    while (precedence > ptp) {
        right = ParsePrecedence(precedence);
        if (!right) {
            return left;
        }
        left = CreateNode(left, right, descriptor, Arithm(descriptor->Kind), PARSER_ASSO_LEFT_RIGHT);
        struct LexDescriptor *peeking = LexPeekNext();
        if (peeking->Kind == LEX_TOK_SEMI) {
            return left;
        }
        free(peeking);
    }
    return left;
}

static struct Node *ParsePrimary() {
    struct LexDescriptor *descriptor = LexPeekNext();
    if (descriptor->Kind == LEX_TOK_MINUS || descriptor->Kind == LEX_TOK_BANG
        || descriptor->Kind == LEX_TOK_TILDA) {
        free(descriptor);
        return ParsePrefixUnaryPrecedence(0);
    }
    free(descriptor);
    descriptor = LexNext();
    return CreateNode(0, 0, descriptor, PARSER_NODE_LIT, PARSER_ASSO_RIGHT_LEFT);
}

struct Node *ParseNoLexs() {
    return ParsePrecedence(0);
}

struct Node *Parse(const char *filepath) {
    LexOpen(filepath);
    struct Node *node = ParsePrecedence(0);
    LexClose();
    return node;
}