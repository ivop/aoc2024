#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

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

    ull sum = 0;
    for (int l=0; l<nlines; l++) {
        ull v = strtoll(lines[l], 0, 10);
        for (int x=0; x<2000; x++) v = next(v);
        sum += v;
    }

    printf("%lld\n", sum);
}
