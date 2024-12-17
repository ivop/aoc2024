#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint64_t A, B, C; //, Ainit;

char *target = "2415750341165530";
char output[32];

void ex2(void) {
/*
0000:	00 03	A=A>>3
0002:	05 04	out A&7
0004:	03 00	if A!=0 jmp 0
*/
    do {
        A=A>>3;
        printf("%ld,",A&7);
    } while (A);
}


void stepforward(void) {
/*
0000:	02 04	B=A&7
0002:	01 05	B=B^5
0004:	07 05	C=A>>B
0006:	00 03	A=A>>3
0008:	04 01	B=B^C
000a:	01 06	B=B^6
000c:	05 05	out B&7
000e:	03 00	if A!=0 jmp 0
*/
        B=A&7;
        B=B^5;
        C=A>>B;
        A=A>>3;
        B=B^C;
        B=B^6;
        sprintf(output+strlen(output),"%ld",B&7);
}

void run(void) {
    do { stepforward(); } while (A);
}

void stepback(void) {
    printf("(%ld)\n",B&7);
    B=B^6;
    B=B^C;
    A=A<<3;
    C=A<<B;
    B=B^5;
    A&=~7;
    A|=B&7;
}

//int main(void) {
#if 0   // manually find number
    A=037101ULL;
    printf("%lo %lo %lo\n", A, B, C);
    output[0]=0;
    do {
        stepforward();
        printf("'%s'\n", output);
        printf("%lo %lo %lo\n", A, B, C);
    } while (A);
#endif
#if 0       // automatically find rest
    Ainit=037ULL;
    for (int i=13; i>=0; i--) {
        int j;
        Ainit <<= 3;
        for (j=0; j<8; j++) {
            output[0]=0;
            Ainit &= ~7;
            Ainit |= j;
            A=Ainit;
            B=C=0;
            run();
            printf("(%lo) '%s' '%s'\n", Ainit, &target[i], output);
            if (!strcmp(&target[i],output)) break;
        }
        if (j==8) {
            puts("huh?");
            return 1;
        } else { printf("j=%d\n", j); }
        printf("%lo\n", Ainit);
    }
    printf("part 2: %ld\n", Ainit);     // okay, but too high, why?
//    Ainit=03710142312204233ULL;
#endif
#if 0   // we need to backtrack, this fails after 5 digits
    Ainit=0ULL;
    for (int i=15; i>=0; i--) {
        uint64_t j;
        for (j=0; j<8; j++) {
            A=Ainit + (j << (i*3));
            printf("Test %0lo\n", A);
            B=C=0;
            memset(output,0,32);
            run();
            printf("'%s' '%s'\n", &target[i], output+strlen(output)-(15-i)-1);
            if (!strcmp(&target[i],output+strlen(output)-(15-i)-1)) break;
        }
        if (j==8) {
            puts("huh?");
            return 1;
        }
        printf("j=%ld\n", j);
        Ainit = Ainit + (j<<(i*3));
        printf("Ainit=%0lo (%ld)\n", Ainit, Ainit);
    }
#endif

void try(int i, uint64_t Ainit) {
    uint64_t j;
    for (j=0; j<8; j++) {
        A=Ainit + (j << (i*3));
        printf("Test %0lo\n", A);
        B=C=0;
        memset(output,0,32);
        run();
        printf("'%s' '%s'\n", &target[i], output+strlen(output)-(15-i)-1);
        if (!strcmp(&target[i],output+strlen(output)-(15-i)-1)) {
            uint64_t tmpAinit = Ainit + (j<<(i*3));
            if (i==0) {
                printf("found: %0lo (%ld)\n", tmpAinit, tmpAinit);
                exit(0);
            }
            try(i-1, tmpAinit);
        }
    }
    // back level up
}

int main(void) {
    try(15, 0);
}
