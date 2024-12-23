#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

/* AMD FX(tm)-8320 Eight-Core Processor
 * brute-force, single thread on 12 year old CPU
 *
 * real	45m2,753s
 * user	45m1,380s
 * sys	0m0,680s
 *
 * same CPU, MT:
 *
 * real	9m2,203s
 * user	70m40,137s
 * sys	0m4,399s
 */

char **lines;
int nlines, space;
char *line;
size_t n;

typedef unsigned long long ull;

struct foo {
    int changes[4];
    int price;
};

struct foo **bar;

ull next(ull v) {
    v ^= v << 6;
    v &= 0xffffff;
    v ^= v >> 5;
    v &= 0xffffff;
    v ^= v << 11;
    v &= 0xffffff;
    return v;
}

ull best[19];

void *calc(void *ptr) {
    int a = *((int *) ptr);
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
                if (sum > best[a+9]) best[a+9] = sum;
            }
        }
    }
    return ptr;
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

    bar = malloc(sizeof(struct foo*) * nlines);
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

    int as[19] = {};
    pthread_t ts[19];

    for (int a=0; a<19; a++) {
        as[a] = a-9;
        pthread_create(&ts[a], NULL, calc, &as[a]);
    }
    for (int a=0; a<19; a++) pthread_join(ts[a], NULL);
    ull verybest = 0;
    for (int i=0; i<19; i++) if (best[i] > verybest) verybest = best[i];
    printf("part 2: %lld\n", verybest);
}
