#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

char **lines;
int nlines;
int space;
char *line;
size_t n;
typedef unsigned long long ull;

ull process(ull adx, ull ady, ull bdx, ull bdy, ull px, ull py) {
    ull cost = ULLONG_MAX;

    for (int b=0; b<=100; b++) {
        for (int a=0; a<=100; a++) {
            ull x = a*adx + b*bdx;
            ull y = a*ady + b*bdy;
            if (x != px || y != py) continue;
            ull c = a*3 + b*1;
            if (c < cost) cost = c;
        }
    }
    return cost == ULLONG_MAX ? 0 : cost;
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
    int l = 0;
    ull cost = 0;
    while (l < nlines) {
        char *p = strtok(lines[l++], "+");
        p = strtok(NULL, "+");
        ull adx = strtoll(p, 0, 10);
        p = strtok(NULL, "+");
        ull ady = strtoll(p, 0, 10);

        p = strtok(lines[l++], "+");
        p = strtok(NULL, "+");
        ull bdx = strtoll(p, 0, 10);
        p = strtok(NULL, "+");
        ull bdy = strtoll(p, 0, 10);

        p = strtok(lines[l++], "=");
        p = strtok(NULL, "=");
        ull px = strtoll(p, 0, 10);
        p = strtok(NULL, "=");
        ull py = strtoll(p, 0, 10);
        l++;

        cost += process(adx, ady, bdx, bdy, px, py);
    }
    printf("%lld\n", cost);
}
