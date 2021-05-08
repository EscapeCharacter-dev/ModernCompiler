#ifndef GENERATION_H
#define GENERATION_H

#include "Types.h"

struct GeneratorClass {
    void (*Prepare)();
    void (*Afterwards)();
    I (*Add)(I left, I right);
    I (*Sub)(I left, I right);
    I (*Mul)(I left, I right);
    I (*Div)(I left, I right);
    I (*Mod)(I left, I right);
    I (*And)(I left, I right);
    I (*XOr)(I left, I right);
    I (*Or)(I left, I right);
    I (*LAnd)(I left, I right);
    I (*LOr)(I left, I right);
    I (*Load)(I64 value);
    I (*Neg)(I left);
    I (*Not)(I left);
    I (*BNot)(I left);
    void (*EndStatement)();
};

#endif
