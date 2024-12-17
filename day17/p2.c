#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint64_t A, B, C; //, Ainit;
char *target = "2415750341165530";
char output[32];

void run(void) {
    do {
        B=A&7; B=B^5; C=A>>B; A=A>>3; B=B^C; B=B^6;
        sprintf(output+strlen(output),"%ld",B&7);
    } while (A);
}

void try(int i, uint64_t Ainit) {
    uint64_t j;
    for (j=0; j<8; j++) {
        uint64_t tmpAinit = A = Ainit + (j << (i*3));
        B = C = 0;
        memset(output,0,32);
        run();
        if (!strcmp(&target[i], output+strlen(output)-(15-i)-1)) {
            if (!i) {
                printf("%ld\n", tmpAinit);
                exit(0);
            }
            try(i-1, tmpAinit);
        }
    }
}

int main(void) {
    try(15, 0);
}
