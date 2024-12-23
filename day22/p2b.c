#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

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

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (nlines == space) {
            space += 16;
            lines = (char**)realloc(lines, space * sizeof(char*));
        }
        lines[nlines] = strdup(line);
        nlines++;
    }

    int **bar;
    bar = malloc(nlines * sizeof(int *));
    for (int l=0; l<nlines; l++)
        bar[l] = calloc(19*19*19*19, sizeof(int));

    ull best = 0;

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

    for (int a=0; a<19*19*19*19; a++) {
        if (!(a & 0xfff)) printf("%x\n", a);
        ull sum = 0;
        for (int l=0; l<nlines; l++) {
            sum += bar[l][a];
        }
        if (sum > best) {
            best = sum;
        }
    }
    printf("part 2: %lld\n", best);
}
