#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <time.h>

char **map;
char **copy;
int height;
int space;
char *line;
size_t n;
int width;
char *moves;
int nmoves;

struct move {
    int dy, dx;
} dirs[4] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };

int ypos, xpos;
char *p;
bool visual;

// part 1
//
void move(int y, int x, int m) {
    int ynb = y + dirs[m].dy;
    int xnb = x + dirs[m].dx;

    if (map[ynb][xnb] == '#') return;
    if (map[ynb][xnb] == 'O') move(ynb, xnb, m);
    if (map[ynb][xnb] == '.') {
        map[ynb][xnb] = map[y][x];
        map[y][x] = '.';
    }
}

// part 2
//
bool move2b(char *line, int y, int m) {
    int ynb = y + dirs[m].dy;
    for (int x=0; x<width; x++) {
        if (line[x] != '.') {
            if (map[ynb][x] == '#') return false;  // bump
        }
    }

    char newline[width];
    memset(newline, '.', width);
    for (int x=0; x<width; x++) {
        if (line[x] != '.') {
            newline[x] = map[ynb][x];
            if (newline[x] == '[') {
                newline[x+1] = map[ynb][x+1];
            } else if (newline[x] == ']') {
                newline[x-1] = map[ynb][x-1];
            }
        }
    }
    if (strchr(newline, '[')) {
        if (!move2b(newline, ynb, m)) return false;
    }
    for (int x=0; x<width; x++) {
        if (line[x] != '.') {
            map[ynb][x] = map[y][x];
            map[y][x] = '.';
        }
    }
    return true;
}

void move2(int y, int x, int m) {
    int ynb = y + dirs[m].dy;
    int xnb = x + dirs[m].dx;

    if (map[ynb][xnb] == '#') return;
    if (m == 2 || m == 3) {
        if (map[ynb][xnb] == '[') move2(ynb,xnb,m);
        if (map[ynb][xnb] == ']') move2(ynb,xnb,m);
        if (map[ynb][xnb] == '.') {
            map[ynb][xnb] = map[y][x];
            map[y][x] = '.';
        }
        return;
    }

    if (map[ynb][xnb] == '[' || map[ynb][xnb] == ']') {
        char line[width];
        memset(line, '.', width);
        line[xnb] = map[ynb][xnb];
        if (line[xnb] == '[')
            line[xnb+1] = map[ynb][xnb+1];
        else
            line[xnb-1] = map[ynb][xnb-1];
        move2b(line, ynb, m);
    }
    if (map[ynb][xnb] == '.') {
        map[ynb][xnb] = map[y][x];
        map[y][x] = '.';
    }
}

// common
//
char dirchars[4] = "^v<>";

void printmap(int move) {
    if (!visual) return;
    printf("[H");
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            char c = map[y][x];
            if (c == '#') c=' ', printf("[7m");
            putchar(c);
            printf("[0m");
        }
        putchar('\n');
    }
    struct timespec ts = { 0, 1000000 };
    nanosleep(&ts, NULL);
    printf("dir: %c\n", dirchars[move]);
}

unsigned long long calc_sum(char c) {
    unsigned long long sum = 0;
    for (int y=0; y<height; y++) {
        for (int x=0; x<width; x++) {
            if (map[y][x] == c) sum += 100*y + x;
        }
    }
    return sum;
}

int main(int argc, char **argv) {
    if (argc == 2 && !strcmp(argv[1],"-v")) visual = true;
    if (visual) printf("[2J[H[?25l");
    while (getline(&line, &n, stdin) > 0) {
        if (height == space) {
            space += 16;
            map = realloc(map, space * sizeof(char*));
            copy = realloc(copy, space * sizeof(char*));
        }
        if (strlen(line) == 1) break;
        if ((p = strchr(line, '@'))) {
            ypos = height;
            xpos = p-line;
        }
        map[height] = strdup(line);
        copy[height] = strdup(line);
        height++;
    }
    width = strlen(map[0])-1;

    space = 0;
    int c = fgetc(stdin);
    while (c >= 0) {
        if (c == '^' || c == 'v' || c == '<' || c == '>') {
            if (space == nmoves) {
                space += 128;
                moves = realloc(moves, space);
            }
            if (c == '^') c = 0;
            else if (c == 'v') c = 1;
            else if (c == '<') c = 2;
            else if (c == '>') c = 3;
            moves[nmoves++] = c;
        }
        c = fgetc(stdin);
    }

    // part 1

    for (int m=0; m<nmoves; m++) {
        printmap(moves[m]);
        move(ypos, xpos, moves[m]);
        if (map[ypos][xpos] == '.') {
            ypos += dirs[moves[m]].dy;
            xpos += dirs[moves[m]].dx;
        }
    }

    printf("%lld\n", calc_sum('O'));

    // part 2

    for (int y=0; y<height; y++) {
        map[y] = realloc(map[y], width*2);
        for (int x=width-1; x>=0; x--) {
            int v = copy[y][x];
            if (v == '#') {
                map[y][2*x] = map[y][2*x+1] = v;
            } else if (v == 'O') {
                map[y][2*x] = '[';
                map[y][2*x+1] = ']';
            } else if (v == '@') {
                map[y][2*x] = v;
                map[y][2*x+1] = '.';
                ypos = y;
                xpos = 2*x;
            } else {
                map[y][2*x] = map[y][2*x+1] = '.';
            }
        }
    }
    width *= 2;

    for (int m=0; m<nmoves; m++) {
        printmap(moves[m]);
        move2(ypos, xpos, moves[m]);
        if (map[ypos][xpos] == '.') {
            ypos += dirs[moves[m]].dy;
            xpos += dirs[moves[m]].dx;
        }
    }

    printf("%lld\n", calc_sum('['));
    if (visual) printf("[?25h");
}
