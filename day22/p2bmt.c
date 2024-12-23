#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <pthread.h>

/* std::map
 * real	1m29,114s
 * user	1m28,879s
 * sys	0m0,229s
 *
 * std::unordered_map
 * real	0m48,030s
 * user	0m47,862s
 * sys	0m0,145s
 *
 * plain C array:
 * real	0m11,332s
 * user	0m10,524s
 * sys	0m0,808s
 *
 * multi-threaded:
 * real	0m2,704s
 * user	0m13,471s
 * sys	0m0,895s
 */

char **lines;
int nlines, space;
char *line;
size_t n;

typedef unsigned long long ull;
ull best[19];
int **bar;

ull next(ull v) {
    v ^= v << 6;
    v &= 0xffffff;
    v ^= v >> 5;
    v &= 0xffffff;
    v ^= v << 11;
    v &= 0xffffff;
    return v;
}

int pack(int a, int b, int c, int d) {
    int r = a;
    r *= 19;
    r += b;
    r *= 19;
    r += c;
    r *= 19;
    r += d;
    return r;
}

void *calc(void *ptr) {
    int b = *((int *)ptr);
    for (int a=b*19*19*19; a<(b+1)*19*19*19; a++) {
        ull sum = 0;
        for (int l=0; l<nlines; l++) {
            sum += bar[l][a];
        }
        if (sum > best[b]) {
            best[b] = sum;
        }
    }
    return ptr;
}

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (nlines == space) {
            space += 16;
            lines = (char**)realloc(lines, space * sizeof(char*));
        }
        lines[nlines] = strdup(line);
        nlines++;
    }

    bar = malloc(nlines * sizeof(int *));
    for (int l=0; l<nlines; l++)
        bar[l] = calloc(19*19*19*19, sizeof(int));

    for (int l=0; l<nlines; l++) {
        int c[4] = {};
        ull v = atoi(lines[l]);
        int price = v % 10;

        for (int x=0; x<2000; x++) {
            int prevprice = price;
            v = next(v);
            price = v % 10;
            c[0] = c[1];
            c[1] = c[2];
            c[2] = c[3];
            c[3] = price - prevprice;
            if (x >= 3) {
                int p = pack(c[0]+9, c[1]+9, c[2]+9, c[3]+9);
                if (bar[l][p] == 0)   // first occurance only
                    bar[l][p] = price;
            }
        }
    }

    int bs[19] = {};
    pthread_t ts[19];
    for (int b=0; b<19; b++) {
        bs[b] = b;
        pthread_create(&ts[b], NULL, calc, &bs[b]);
    }
    for (int b=0; b<19; b++) pthread_join(ts[b], NULL);
    ull verybest = 0;
    for (int i=0; i<19; i++) if (best[i] > verybest) verybest = best[i];
    printf("part 2: %lld\n", verybest);
}
