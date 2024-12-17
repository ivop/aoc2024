#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint64_t A, B, C, IP = 1, tmp, memtop;
int mem[1000];

int main(int argc, char **argv) {
    fscanf(stdin, "Register A: %ld\n", &A);
    fscanf(stdin, "Register B: %ld\n", &B);
    fscanf(stdin, "Register C: %ld\n", &C);
    fscanf(stdin, "Program: %d", &mem[0]);
    while (fscanf(stdin, ",%d", &mem[IP++])) memtop = IP;
    if (argc != 1) A = strtoll(argv[1],0,10);
    IP = 0;
    while (IP < memtop) {
        int instr = mem[IP++];
        uint64_t combo[8] = { 0, 1, 2, 3, A, B, C, -1 };
        switch (instr) {
        case 0: A = A >> combo[mem[IP++]]; break;
        case 1: B = B ^ mem[IP++]; break;
        case 2: B = combo[mem[IP++]] & 7; break;
        case 3: tmp = mem[IP++]; if (A) IP = tmp; break;
        case 4: tmp = mem[IP++]; B = B ^ C; break;
        case 5: printf("%ld,", combo[mem[IP++]] & 7); break;
        case 6: B = A >> combo[mem[IP++]]; break;
        case 7: C = A >> combo[mem[IP++]]; break;
        }
    }
    printf("\b \n");
}
