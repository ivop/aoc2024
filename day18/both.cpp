#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <list>

#define D 70
#define WHEN 1024

struct coor {
    int y,x;
} *bytes;
char map[D+1][D+1];
int size, nbytes, vis[D+1][D+1];
struct dir {
    int dy, dx;
} dirs[4] = { {-1,0}, {0,1}, {1,0}, {0,-1}};

void printmap(char m[D+1][D+1]) {
    //printf("[H");
    for (int y=0; y<=D; y++) {
        for (int x=0; x<=D; x++) {
            putchar(m[y][x]);
        }
        putchar('\n');
    }
}

int steps;

bool run(void) {
    steps=0;
    std::list<struct coor> pool;
    std::list<struct coor> newpool;
    pool.push_back({0,0});
    while (!pool.empty()) {
        for (auto c : pool) {
            if (vis[c.y][c.x] <= steps) continue;
            vis[c.y][c.x] = steps;
            for (int d=0; d<4; d++) {
                int ny = c.y + dirs[d].dy;
                int nx = c.x + dirs[d].dx;
                if (ny<0 || ny>D) continue;
                if (nx<0 || nx>D) continue;
                if (map[ny][nx] == '#') continue;
                if (ny==D && nx==D) {
                    steps++;
                    return true;
                }
                newpool.push_back({ny,nx});
            }
        }
        steps++;
        pool.clear();
        pool.swap(newpool);
    }
    return false;
}

int main(void) {
    //printf("[2J");
    while (1) {
        int x, y;
        if (fscanf(stdin, "%d,%d", &x, &y) != 2) break;
        if (nbytes == size) {
            size += 16;
            bytes = (struct coor *)realloc(bytes, size * sizeof(struct coor));
        }
        bytes[nbytes].x = x;
        bytes[nbytes].y = y;
        nbytes++;
    }
    memset(map, '.', sizeof(map));

    for (int ns=0; ns<WHEN; ns++) map[bytes[ns].y][bytes[ns].x] = '#';
    for (int y=0; y<=D; y++) for (int x=0; x<=D; x++) vis[y][x]=INT_MAX;
    run();
    printf("part 1: %d\n", steps);

    for (int ns=WHEN; ns<nbytes; ns++) {
        map[bytes[ns].y][bytes[ns].x] = '#';
        for (int y=0; y<=D; y++) for (int x=0; x<=D; x++) vis[y][x]=INT_MAX;
        if (run()) continue;
        printf("part 2: %d,%d\n", bytes[ns].x, bytes[ns].y);
        break;
    }
}
