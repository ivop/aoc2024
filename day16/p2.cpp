#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <iostream>
#include <list>

char **map;
int height, width;
int space;

char *line;
size_t n;

int ypos, xpos;
int best = INT_MAX;

enum { NORTH, EAST, SOUTH, WEST };

struct dir {
    int dy, dx;
} dirs[4] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } }; // 90 deg rot order

int **visited;

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (height == space) {
            space += 16;
            map = (char**)realloc((void*)map, space * sizeof(char*));
        }
        map[height] = strdup(line);
        char *p = strchr(line, 'S');
        if (p) {
            ypos = height;
            xpos = p-line;
        }
        height++;
    }
    width = strlen(map[0])-1;

    visited = (int**)malloc(sizeof(int *) * height);
    for (int y=0; y<height; y++) {
        visited[y] = (int*)malloc(sizeof(int)*width);
        for (int x=0; x<width; x++) {
            visited[y][x] = INT_MAX;
        }
    }
    visited[ypos][xpos] = 0;

    int best = INT_MAX;

    struct coor {
        int y, x;
    };
    struct pos {
        int y, x, cost, d;
        std::list<struct coor> path;
    };
    std::list<struct pos> newpos;
    std::list<struct pos> pos;
    std::list<struct pos> reached_end;
    struct pos start;
    start.x=xpos;
    start.y=ypos;
    start.d=EAST;
    start.cost = 0;
    start.path.push_back({ypos,xpos});
    pos.push_back(start);

    while (!pos.empty()) {
        for (auto p : pos) {
            //printf("(%d,%d), ", p.y, p.x);
            if (visited[p.y][p.x] < p.cost - 1000) continue;
            visited[p.y][p.x] = p.cost;

            if (map[p.y][p.x] == 'E') {
                //printf("[[%d]] ", p.cost);
                if (p.cost < best) best = p.cost;
                reached_end.push_back(p);
                continue;
            }

            int ny, nx, nd;

            nd = p.d;
            ny = p.y + dirs[nd].dy;
            nx = p.x + dirs[nd].dx;
            if (map[ny][nx] != '#') {
                std::list<struct coor> path(p.path);
                path.push_back({ny,nx});
                newpos.push_back({ny,nx,p.cost+1,nd,path});
            }

            nd = (p.d-1)&3;
            ny = p.y + dirs[nd].dy;
            nx = p.x + dirs[nd].dx;
            if (map[ny][nx] != '#') {
                std::list<struct coor> path(p.path);
                path.push_back({ny,nx});
                newpos.push_back({ny,nx,p.cost+1001,nd,path});
            }

            nd = (p.d+1)&3;
            ny = p.y + dirs[nd].dy;
            nx = p.x + dirs[nd].dx;
            if (map[ny][nx] != '#') {
                std::list<struct coor> path(p.path);
                path.push_back({ny,nx});
                newpos.push_back({ny,nx,p.cost+1001,nd,path});
            }
        }
        //printf("\n");
        pos.clear();
        pos.swap(newpos);
    }
    printf("part 1: %d\n", best);

    for (auto p : reached_end) {
        if (p.cost != best) continue;
        for (auto c : p.path) {
            map[c.y][c.x] = 'O';
        }
    }
    int seats = 0;
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (map[y][x] == 'O') seats++;
        }
    }
    printf("part 2: %d\n", seats);
}
