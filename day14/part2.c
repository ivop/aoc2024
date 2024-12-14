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
char map[103][101];

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

    int s;
    for (s=0; s<100000; s++) {
        memset(map, ' ', sizeof(map));
        for (int r=0; r<nrobots; r++) {
            map[robots[r].ypos][robots[r].xpos] = 'X';
        }
        int has_direct_neighbors = 0;
        for (int y=0; y<height; y++) {
            for (int x=0; x<width; x++) {
                if (map[y][x] == 'X') {
                    if (map[y-1][x] == 'X') has_direct_neighbors++;
                    if (map[y+1][x] == 'X') has_direct_neighbors++;
                    if (map[y][x-1] == 'X') has_direct_neighbors++;
                    if (map[y][x+1] == 'X') has_direct_neighbors++;
                }
            }
        }
        if (has_direct_neighbors > 500) break;
        for (int r=0; r<nrobots; r++) {
            robots[r].xpos += robots[r].xvel;
            robots[r].ypos += robots[r].yvel;
            wrap(&robots[r].xpos, width);
            wrap(&robots[r].ypos, height);
        }

    }
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            putchar(map[y][x]);
        }
        putchar('\n');
    }
    printf("%d\n", s);
}
