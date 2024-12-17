#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

uint64_t A, B, C;
char *target = "0000000000000000";  // your target here, adjust len if not 16
int len = 16;
char output[32];

void out(uint64_t V) { sprintf(output+strlen(output),"%ld",V&7); }

void run(void) {
    // transpiled code here, i.e.
L0:
    // instructions
    if (A) goto L0;
}

void try(int i, uint64_t Ainit) {
    uint64_t j;
    for (j=0; j<8; j++) {
        uint64_t tmpAinit = A = Ainit + (j << (i*3));
        B = C = 0;
        memset(output,0,32);
        run();
        if (!strcmp(&target[i], output+strlen(output)-(len-1-i)-1)) {
            if (!i) {
                printf("%ld\n", tmpAinit);
                exit(0);
            }
            try(i-1, tmpAinit);
        }
    }
}

int main(void) {
    try(len-1, 0);
}
