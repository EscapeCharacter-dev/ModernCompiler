#include <stdio.h>
#include <stdlib.h>
#include "Generation.h"

static struct GeneratorClass *Generator = 0;
static FILE *Outfile;

static void _Prepare() {
    fprintf(Outfile, ".section\t.text\n");
    fprintf(Outfile, ".globl\tmain\n");
    fprintf(Outfile, "main:\n");
}

static void _Afterwards() {
    fprintf(Outfile, "\txor\t%%rax, %%rax\n");
    fprintf(Outfile, "\tret\n");
}

static I _LoadVal(I64 value) {
    fprintf(Outfile, "\tmov\t$%lld, %%rax\n", value);
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static void PopBinary(I isRightLeft) {
    if (!isRightLeft) {
        fprintf(Outfile, "\tpop\t%%rax\n");
        fprintf(Outfile, "\tpop\t%%rdx\n");
    } else {
        fprintf(Outfile, "\tpop\t%%rdx\n");
        fprintf(Outfile, "\tpop\t%%rax\n");
    }
}

static I _Add(I left, I right) {
    PopBinary(0);
    fprintf(Outfile, "\tadd\t%%rdx, %%rax\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Sub(I left, I right) {
    PopBinary(0);
    fprintf(Outfile, "\tsub\t%%rax, %%rdx\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Mul(I left, I right) {
    PopBinary(0);
    fprintf(Outfile, "\timul\t%%rdx, %%rax\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Div(I left, I right) {
    PopBinary(1);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq\t%%rdx\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Mod(I left, I right) {
    PopBinary(1);
    fprintf(Outfile, "\tcqo\n");
    fprintf(Outfile, "\tidivq\t%%rdx\n");
    fprintf(Outfile, "\tpush\t%%rdx\n");
    return 1;
}

static I _And(I left, I right) {
    PopBinary(0);
    fprintf(Outfile, "\tand\t%%rax, %%rdx\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Or(I left, I right) {
    PopBinary(0);
    fprintf(Outfile, "\tor\t%%rax, %%rdx\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _XOr(I left, I right) {
    PopBinary(0);
    fprintf(Outfile, "\txor\t%%rax, %%rdx\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Not(I left) {
    fprintf(Outfile, "\tpop\t%%rax\n");
    fprintf(Outfile, "\tmov\t%%$0xFFFFFFFFFFFFFFFF, %%rdx\n");
    fprintf(Outfile, "\txor\t%%rdx, %%rax\n");
    fprintf(Outfile, "\tpush %%rax\n");
    return 1;
}

static I _BNot(I left) {
    fprintf(Outfile, "\tpop\t%%rax\n");
    fprintf(Outfile, "\tnot\t%%rax\n");
    fprintf(Outfile, "\tpush\t%%rax\n");
    return 1;
}

static I _Neg(I left) {
    fprintf(Outfile, "\tnegq\t%%rsp\n");
    return 1;
}

static void _EndStatement() {
    fprintf(Outfile, "\tpop\t%%rax\n");
    fprintf(Outfile, "\txor\t%%rax, %%rax\n");
}

struct GeneratorClass *Genx86_64(const char *filepath) {
    if (!Generator) {
        Generator = malloc(sizeof(struct GeneratorClass));
        Outfile = fopen(filepath, "w");
        Generator->Load = _LoadVal;
        Generator->Add = _Add;
        Generator->Prepare = _Prepare;
        Generator->Afterwards = _Afterwards;
        Generator->EndStatement = _EndStatement;
        Generator->Add = _Add;
        Generator->Sub = _Sub;
        Generator->Mul = _Mul;
        Generator->Div = _Div;
        Generator->Mod = _Mod;
        Generator->Not = _Not;
        Generator->BNot = _BNot;
        Generator->Neg = _Neg;
    }
    return Generator;
}

void Genx86_64_Close() {
    fclose(Outfile);
}