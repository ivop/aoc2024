#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

uint64_t A, B, C, IP = 1, tmp, memtop, Ainit;
int mem[1000], L[8];

void disasm(int pc, int ccode) {
    char combo[8] = "0123ABCX";
    int lit[8] = { 0, 1, 0, 1, 1, 0, 0, 0 };
    if (!ccode) {
        char *opc[8] = { "adv","bxl","bst","jnz","bxc","out","bdv","cdv" };
        printf("%04x:\t%02x %02x\t%s %c\n", pc, mem[pc], mem[pc+1],
                opc[mem[pc]], lit[mem[pc]] ? '0'+mem[pc+1] : combo[mem[pc+1]]);
    } else {
        char *opc[8] = { "A=A>>%c;\n", "B=B^%c;\n", "B=%c&7;\n",
            "if (A) goto L%c;\n", "B=B^C;\n", "out(%c);\n", "B=A>>%c;\n",
            "C=A>>%c;\n" };
        printf(pc<8 && L[pc&7] ? "L%d:\n    " : "    ", pc);
        printf(opc[mem[pc]], lit[mem[pc]] ? '0'+mem[pc+1] : combo[mem[pc+1]]);
    }
}

int main(int argc, char **argv) {
    fscanf(stdin, "Register A: %ld\n", &A);
    fscanf(stdin, "Register B: %ld\n", &B);
    fscanf(stdin, "Register C: %ld\n", &C);
    fscanf(stdin, "Program: %d", &mem[0]);
    while (fscanf(stdin, ",%d", &mem[IP++])) memtop = IP;
    printf("program = \"");
    for (IP=0; IP<memtop; IP++) putchar('0'+mem[IP]);
    printf("\"\n");
    for (IP=0; IP<memtop; IP+=2) if (mem[IP] == 3) L[mem[IP+1]] = 1;
    for (IP=0; IP<memtop; IP+=2) disasm(IP,argc==1 ? 0 : atoi(argv[1]));
}
