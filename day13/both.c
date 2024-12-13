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

ull calc(ull adx, ull ady, ull bdx, ull bdy, ull px, ull py) {
    double A[2][2]    = { { adx, bdx }, { ady, bdy } };
    double B[2][1]    = { { px } , { py } };

    double det = A[0][0]*A[1][1] - A[0][1]*A[1][0];
    if (det == 0.0) return 0;
    double Ainv[2][2] = { {  A[1][1] / det, -A[0][1] / det },
                          { -A[1][0] / det,  A[0][0] / det } };

    double X[2][1] = { { Ainv[0][0] * B[0][0] + Ainv[0][1] * B[0][1] },
                       { Ainv[1][0] * B[0][0] + Ainv[1][1] * B[0][1] } };

    ull a = round(X[0][0]);
    ull b = round(X[0][1]);

    return ((px != a*adx + b*bdx) || (py != a*ady + b*bdy)) ? 0 : a*3+b;
}

ull getnum(char **p, int after) {
    *p = strchr(*p, after) + 1;
    return strtoll(*p, 0, 10);
}

void run(bool part2) {
    int l = 0;
    ull cost = 0;
    while (l < nlines) {
        char *p = lines[l++];
        ull adx = getnum(&p, '+');
        ull ady = getnum(&p, '+');
        p = lines[l++];
        ull bdx = getnum(&p, '+');
        ull bdy = getnum(&p, '+');
        p = lines[l++];
        ull px = getnum(&p, '=');
        ull py = getnum(&p, '=');
        l++;

        if (part2) {
            px += 10000000000000ULL;
            py += 10000000000000ULL;
        }
        cost += calc(adx, ady, bdx, bdy, px, py);
    }
    printf("%lld\n", cost);
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
    run(0);
    run(1);
}
