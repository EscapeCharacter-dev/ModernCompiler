#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Lex.h"

static FILE *input;

void LexOpen(const char *filepath) {
    input = fopen(filepath, "r");
    if (!input) {
        fprintf(stderr, "Error using file '%s'.\n", filepath);
        exit(0);
    }
}

void LexNextDescriptor(struct LexDescriptor *descriptor) {
    I c = fgetc(input);
    if (isspace(c)) {
        while (isspace((c = fgetc(input))));
    }
    switch (c) {
    case EOF:
        descriptor->Character = 0;
        descriptor->Kind = LEX_TOK_EOF;
        return;
    case '+':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_PLUS;
        return;
    case '-':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_MINUS;
        return;
    case '*':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_STAR;
        return;
    case '/':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_SLASH;
        return;
    case '%':
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_PERCENT;
        return;
    default:
        if (isdigit(c)) {
            I count = 0;
            I where = ftell(input);
            while (isdigit((c = fgetc(input)))) {
                count++;
            }
            fseek(input, where, SEEK_SET);
            char *str = malloc(count + 1);
            fread(str, 1, count, input);
            str[count] = '\0';
            I32 value = strtol(str, 0, 10);
            free(str);
            descriptor->Character = 0;
            descriptor->Kind = LEX_TOK_LINT;
            descriptor->Value.AsI32 = value;
            return;
        }
        fprintf(stderr, "Unknown character %c\n", c);
        descriptor->Character = c;
        descriptor->Kind = LEX_TOK_BAD;
        return;
    }
}

struct LexDescriptor *LexNext() {
    struct LexDescriptor *descriptor = malloc(sizeof(struct LexDescriptor));
    LexNextDescriptor(descriptor);
    return descriptor;
}

void LexClose() {
    fclose(input);
}