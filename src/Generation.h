#ifndef GENERATION_H
#define GENERATION_H

typedef int WI; // walking "info"

typedef void (*OnPrepare)();
typedef void (*OnAfterwards)();
typedef int (*OnAdd)(int left, int right);
typedef int (*OnSub)(int left, int right);
typedef int (*OnMul)(int left, int right);
typedef int (*OnDiv)(int left, int right);
typedef int (*OnMod)(int left, int right);

struct GeneratorClass {
    OnPrepare Prepare;
    OnAfterwards Afterwards;
    OnAdd Add;
    OnSub Sub;
    OnMul Mul;
    OnDiv Div;
    OnMod Mod;
};

#endif
