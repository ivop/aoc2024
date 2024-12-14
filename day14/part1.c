#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

struct robot {
    int xpos, ypos;
    int xvel, yvel;
};

struct robot *robots;
int space;
int nrobots;

char *line;
size_t n;

int width = 101;
int height = 103;
int wmid = 50;
int hmid = 51;

void wrap(int *v, int edge) {
    if (*v < 0) *v += edge;
    if (*v >= edge) *v -= edge;
}

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        char *p = strchr(line, '=') + 1;
        int xpos = strtoll(p, 0, 10);
        p = strchr(p, ',') + 1;
        int ypos = strtoll(p, 0, 10);
        p = strchr(p, '=') + 1;
        int xvel = strtoll(p, 0, 10);
        p = strchr(p, ',') + 1;
        int yvel = strtoll(p, 0, 10);

        if (space == nrobots) {
            space += 16;
            robots = realloc(robots, space * sizeof(struct robot));
        }
        robots[nrobots].xpos = xpos;
        robots[nrobots].ypos = ypos;
        robots[nrobots].xvel = xvel;
        robots[nrobots].yvel = yvel;
        nrobots++;
    }

    for (int s=0; s<100; s++) {
        for (int r=0; r<nrobots; r++) {
            robots[r].xpos += robots[r].xvel;
            robots[r].ypos += robots[r].yvel;
            wrap(&robots[r].xpos, width);
            wrap(&robots[r].ypos, height);
        }
    }

    unsigned long long q1=0, q2=0, q3=0, q4=0;

    for (int r=0; r<nrobots; r++) {
        int xpos = robots[r].xpos;
        int ypos = robots[r].ypos;
        if (xpos < wmid && ypos < hmid) q1++;
        if (xpos > wmid && ypos < hmid) q2++;
        if (xpos < wmid && ypos > hmid) q3++;
        if (xpos > wmid && ypos > hmid) q4++;
    }
    printf("%lld\n", q1*q2*q3*q4);
}
