#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **map;
char *line;
size_t n;
int space, height, width, area, perimeter, fences;
unsigned long long part1, part2;
char **done;

int nb(int y, int x) {
    if (y<0 || y>=height || x<0 || x>=width) return -1;
    return map[y][x];
}

void process(int y, int x, int v) {
    if (done[y][x]) return; else done[y][x] = 1;
    area++;
    if (nb(y-1,x) != v && nb(y,x-1) != v) fences++;
    if (nb(y-1,x) != v && nb(y,x+1) != v) fences++;
    if (nb(y+1,x) != v && nb(y,x+1) != v) fences++;
    if (nb(y+1,x) != v && nb(y,x-1) != v) fences++;
    if (nb(y,x+1) == v && nb(y+1,x) == v && nb (y+1,x+1) != v) fences++;
    if (nb(y-1,x) == v && nb(y,x+1) == v && nb (y-1,x+1) != v) fences++;
    if (nb(y,x-1) == v && nb(y+1,x) == v && nb (y+1,x-1) != v) fences++;
    if (nb(y,x-1) == v && nb(y-1,x) == v && nb (y-1,x-1) != v) fences++;
    if (nb(y-1,x) != v) perimeter++; else process(y-1,x,v);
    if (nb(y+1,x) != v) perimeter++; else process(y+1,x,v);
    if (nb(y,x-1) != v) perimeter++; else process(y,x-1,v);
    if (nb(y,x+1) != v) perimeter++; else process(y,x+1,v);
}

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (height == space) {
            space += 16;
            map = realloc(map, space * sizeof(char*));
        }
        map[height++] = strdup(line);
    }
    width = strlen(map[0])-1;
    done = realloc(done, height * sizeof(char*));
    for (int y=0; y<height; y++) done[y] = calloc(width,1);
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (!done[y][x]) {
                area = perimeter = fences = 0;
                process(y, x, map[y][x]);
                part1 += area * perimeter;
                part2 += area * fences;
            }
        }
    }
    printf("%lld\n%lld\n", part1, part2);
}
