#include <stdio.h>

int A, B, C, IP = 1, tmp, memtop;
char *line;
size_t n;
int mem[1000];

int main(void) {
    fscanf(stdin, "Register A: %d\n", &A);
    fscanf(stdin, "Register B: %d\n", &B);
    fscanf(stdin, "Register C: %d\n", &C);
    fscanf(stdin, "Program: %d", &mem[0]);
    while (fscanf(stdin, ",%d", &mem[IP++])) ;
    memtop = IP-1;
    IP = 0;
    while (IP < memtop) {
        int instr = mem[IP++];
        int combo[8] = { 0, 1, 2, 3, A, B, C, -1 };
        switch (instr) {
        case 0: A = A >> combo[mem[IP++]]; break;
        case 1: B = B ^ mem[IP++]; break;
        case 2: B = combo[mem[IP++]] & 7; break;
        case 3: tmp = mem[IP++]; if (A) IP = tmp; break;
        case 4: tmp = mem[IP++]; B = B ^ C; break;
        case 5: printf("%d,", combo[mem[IP++]] & 7); break;
        case 6: B = A >> combo[mem[IP++]]; break;
        case 7: C = A >> combo[mem[IP++]]; break;
        }
    }
    putchar('\n');
}
