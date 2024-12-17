#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

uint32_t A, B, C, IP = 1, tmp, memtop, Ainit;
char *line;
size_t n;
int mem[1000];

void disasm(int pc) {
    char combo[8] = "0123ABCX";
    int lit[8] = { 0, 1, 0, 1, 1, 0, 0, 0 };
    //char *opc[8] = { "adv", "bxl", "bst", "jnz", "bxc", "out", "bdv", "cdv" };
    //printf("%04x:\t%02x %02x\t%s %c\n", pc, mem[pc], mem[pc+1], opc[mem[pc]], lit[mem[pc]] ? '0'+mem[pc+1] : combo[mem[pc+1]]);
    char *opc[8] = { "A=A>>%c", "B=B^%c", "B=%c&7", "if A!=0 jmp %c", "B=B^C", "out %c&7", "B=A>>%c", "C=A>>%c" };
    printf("%04x:\t%02x %02x\t", pc, mem[pc], mem[pc+1]);
    printf(opc[mem[pc]], lit[mem[pc]] ? '0'+mem[pc+1] : combo[mem[pc+1]]);
    putchar('\n');
}

int main(void) {
    fscanf(stdin, "Register A: %d\n", &A);
    fscanf(stdin, "Register B: %d\n", &B);
    fscanf(stdin, "Register C: %d\n", &C);
    fscanf(stdin, "Program: %d", &mem[0]);
    while (fscanf(stdin, ",%d", &mem[IP++])) ;
    memtop = IP-1;
    for (IP=0; IP<memtop; IP+=2) disasm(IP);
}
