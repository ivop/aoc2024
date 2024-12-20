// Based on: https://www.reddit.com/r/adventofcode/comments/1hhiawu/2024_day_18_part_2_visualization_of_my_algorithm/
// And on: https://github.com/sahil-777/Algorithm-and-Data-Structure/blob/master/DSU-Disjoint%20Set%20Union.cpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define D 70
#define W (D+3)
#define H (D+3)

struct coor {
    int y,x;
} *bytes;
char map[H][W];
int size, nbytes, steps;
struct dir {
    int dy, dx;
} dirs[8] = { {-1,-1}, {-1,0}, {-1,1},
              { 0,-1},         { 0,1},
              { 1,-1}, { 1,0}, { 1,1} };

#define SIZE ((D+3)*(D+3))

int parent[SIZE], rank[SIZE];

void make_set(int node) {
    parent[node] = node;
    rank[node]   = 0;
}

int find_set(int node) {
    if (parent[node] == node) return node;
    return parent[node] = find_set(parent[node]);
}

void Union(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (rank[a] < rank[b]) {
            int tmp = a;
            a = b;
            b = tmp;
        }
        parent[b] = a;
        if (rank[a] == rank[b]) {
            rank[a]++;
        }
    }
}

int yxtoi(int y, int x) {
    return y*W + x;
}

int main(void) {
    while (1) {
        int x, y;
        if (fscanf(stdin, "%d,%d", &x, &y) != 2) break;
        if (nbytes == size) {
            size += 16;
            bytes = (struct coor *)realloc(bytes, size * sizeof(struct coor));
        }
        bytes[nbytes].x = x+1; // adjust inside "border"
        bytes[nbytes].y = y+1; // idem
        nbytes++;
    }
    memset(map, '.', sizeof(map));

    for (int i=0; i<SIZE; i++) make_set(i);

    // top and right border is one set
    map[0][1] = '#';
    for (int x=2; x<W; x++) {
        map[0][x] = '#';
        Union(yxtoi(0,1), yxtoi(0,x));
    }
    for (int y=1; y<H; y++) {
        map[y][W-1] = '#';
        Union(yxtoi(0,W-1), yxtoi(y,W-1));
    }
    // bottom and left border is one set
    map[H-1][0] = '#';
    for (int x=1; x<W-1; x++) {
        map[H-1][x] = '#';
        Union(yxtoi(H-1,0), yxtoi(H-1,x));
    }
    for (int y=0; y<H-1; y++) {
        map[y][0] = '#';
        Union(yxtoi(H-1,0), yxtoi(y,0));
    }

    for (int ns = 0; ns<nbytes; ns++) {
        int y = bytes[ns].y;
        int x = bytes[ns].x;
        map[y][x] = '#';
        for (int d=0; d<8; d++) {
            int ny = y + dirs[d].dy;
            int nx = x + dirs[d].dx;
            if (map[ny][nx] == '#') Union(yxtoi(ny,nx),yxtoi(y,x));
        }

        // stop if top/right set and bottom/left set are equal
        if (find_set(yxtoi(4,0)) == find_set(yxtoi(0,4))) {
            printf("%d,%d\n", x-1, y-1);
            break;
        }
    }
}
