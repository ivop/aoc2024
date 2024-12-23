#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

/* AMD FX(tm)-8320 Eight-Core Processor
 * brute-force, single thread on 12 year old CPU
 *
 * real	45m2,753s
 * user	45m1,380s
 * sys	0m0,680s
 */

char **lines;
int nlines, space;
char *line;
size_t n;

typedef unsigned long long ull;

ull next(ull v) {
    v ^= v << 6;
    v &= 0xffffff;
    v ^= v >> 5;
    v &= 0xffffff;
    v ^= v << 11;
    v &= 0xffffff;
    return v;
}

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (nlines == space) {
            space += 16;
            lines = realloc(lines, space * sizeof(char*));
        }
        lines[nlines] = strdup(line);
        nlines++;
    }

    struct foo {
        int changes[4];
        int price;
    };

    struct foo *bar[nlines];
    for (int l=0; l<nlines; l++)
        bar[l] = calloc(sizeof(struct foo),2000);

    for (int l=0; l<nlines; l++) {
        int changes[4] = {};
        ull v = atoi(lines[l]);
        int price = v % 10;

        for (int x=0; x<2000; x++) {
            int prevprice = price;
            v = next(v);
            price = v % 10;
            changes[0] = changes[1];
            changes[1] = changes[2];
            changes[2] = changes[3];
            changes[3] = price - prevprice;
            if (x >= 3) {
                for (int c=0; c<4; c++) {
                    bar[l][x].changes[c] = changes[c];
                    bar[l][x].price = price;
                }
            }
        }
    }

    ull best = 0;

    for (int a=-9; a<=9; a++) {
        for (int b=-9; b<=9; b++) {
            printf("%d,%d\n", a, b);
            for (int c=-9; c<=9; c++) {
                for (int d=-9; d<=9; d++) {
                    ull sum = 0;
                    for (int l=0; l<nlines; l++) {
                        for (int x=3; x<2000; x++) {
                            if (bar[l][x].changes[0] == a &&
                                bar[l][x].changes[1] == b &&
                                bar[l][x].changes[2] == c &&
                                bar[l][x].changes[3] == d) {
                                sum += bar[l][x].price;
                                break;
                            }
                        }
                    }
                    if (sum > best) {
                        best = sum;
                    }
                }
            }
        }
    }
    printf("part 2: %lld\n", best);
}