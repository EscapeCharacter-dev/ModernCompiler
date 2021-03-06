#ifndef MODERNC_LEX_H
#define MODERNC_LEX_H

#include "Types.h"

enum {
    LEX_TOK_PLUS,
    LEX_TOK_MINUS,
    LEX_TOK_STAR,
    LEX_TOK_SLASH,
    LEX_TOK_PERCENT,
    LEX_TOK_EOF,
    LEX_TOK_LINT,
    LEX_TOK_BAD,
    LEX_TOK_OPARENT,
    LEX_TOK_CPARENT,
    LEX_TOK_PLUSPLUS,
    LEX_TOK_MINUSMINUS,
    LEX_TOK_HELM,
    LEX_TOK_PIPE,
    LEX_TOK_AMPERSAND,
    LEX_TOK_AMPERSANDS,
    LEX_TOK_PIPES,
    LEX_TOK_BANG,
    LEX_TOK_TILDA,
    LEX_TOK_SEMI,
};

union LexValue {
    I8 	AsI8;
    U8 	AsU8;
    I16	AsI16;
    U16 AsU16;
    I32	AsI32;
    U32	AsU32;
    I	AsI;
    U	AsU;
    I64	AsI64;
    U64 AsU64;
};

struct LexDescriptor {
        U8 		Character;	// 0 if it is a string
        I  		Kind;
        union LexValue 	Value;
};

void LexOpen(const char *filepath);
void LexClose();
void LexNextDescriptor(struct LexDescriptor *descriptor);
struct LexDescriptor *LexNext();
struct LexDescriptor *LexPeekNext();

#endif
