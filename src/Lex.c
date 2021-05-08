#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "Lex.h"

static FILE *input;

static I chrpos(const char *string, U8 character) {
    char *p;
    p = strchr(string, character);
    return p ? p - string : -1;
}

static I32 scanI32(U8 c) {
    I k, val = 0;
    
    while ((k = chrpos("0123456789", c)) >= 0) {
        val = val * 10 + k;
        c = fgetc(input);
    }
    return val;
}

struct LexDescriptor *LexPeekNext() {
    I pos = ftell(input);
    struct LexDescriptor *descriptor = LexNext();
    fseek(input, pos, SEEK_SET);
    return descriptor;
}

static U8 PeekChar() {
    U8 ret = fgetc(input);
    fseek(input, -1, SEEK_CUR);
    return ret;
}

void LexOpen(const char *filepath) {
    input = fopen(filepath, "r");
    if (!input) {
        fprintf(stderr, "Error using file '%s'.\n", filepath);
        exit(0);
    }
}

void LexNextDescriptor(struct LexDescriptor *descriptor) {
    __label__ ret;
    I c = fgetc(input);
    if (isspace(c)) {
        while (isspace((c = fgetc(input))));
    }
    switch (c) {
    case EOF:
        descriptor->Character = 0;
        descriptor->Kind = LEX_TOK_EOF;
        goto ret;
    case '+':
        if (PeekChar() == '+') {
            fseek(input, 1, SEEK_CUR);
            descriptor->Character = 0;
            descriptor->Kind = LEX_TOK_PLUSPLUS;
        } else {
            descriptor->Character = c;
            descriptor->Kind = LEX_TOK_PLUS;
        }
        goto ret;
    case '-':
        if (PeekChar() == '-') {
            fseek(input, 1, SEEK_CUR);
            descriptor->Character = 0;
            descriptor->Kind = LEX_TOK_MINUSMINUS;
        } else {
            descriptor->Character = c;
            descriptor->Kind = LEX_TOK_MINUS;
        }
        goto ret;
    case '*':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_STAR;
        goto ret;
    case '/':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_SLASH;
        goto ret;
    case '%':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_PERCENT;
        goto ret;
    case '^':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_HELM;
        goto ret;
    case '&':
        if (PeekChar() == '&') {
            fseek(input, 1, SEEK_CUR);
            descriptor->Character = 0;
            descriptor->Kind = LEX_TOK_AMPERSANDS;
        } else {
            descriptor->Character = c;
            descriptor->Kind = LEX_TOK_AMPERSAND;
        }
        goto ret;
    case '|':
        if (PeekChar() == '|') {
            fseek(input, 1, SEEK_CUR);
            descriptor->Character = 0;
            descriptor->Kind = LEX_TOK_PIPES;
        } else {
            descriptor->Character = c;
            descriptor->Kind = LEX_TOK_PIPE;
        }
        goto ret;
    case '(':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_OPARENT;
        goto ret;
    case ')':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_CPARENT;
        goto ret;
    case '!':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_BANG;
        goto ret;
    case '~':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_TILDA;
        goto ret;
    case ';':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_SEMI;
        goto ret;
    default:
        if (isdigit(c)) {
            I32 value = scanI32(c);
            descriptor->Character = 0;
            descriptor->Kind = LEX_TOK_LINT;
            descriptor->Value.AsI32 = value;
            fseek(input, -1, SEEK_CUR);
            goto ret;
        }
        fprintf(stderr, "Unknown character %c\n", c);
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_BAD;
        goto ret;
    }
ret:
    return;
}

struct LexDescriptor *LexNext() {
    struct LexDescriptor *descriptor = malloc(sizeof(struct LexDescriptor));
    LexNextDescriptor(descriptor);
    return descriptor;
}

void LexClose() {
    fclose(input);
}