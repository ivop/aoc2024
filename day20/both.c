#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>

char **map;
int space, height, width;
char *line, *t;
size_t n;
int sxpos, sypos, expos, eypos;
struct dir {
    int dy, dx;
} dirs[4] = { { -1, 0 }, { 0, 1 }, { 1, 0 }, { 0, -1 } };

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (height == space) {
            space += 16;
            map = realloc(map, space * sizeof(char *));
        }
        map[height] = strdup(line);
        if ((t = strchr(line, 'S'))) {
            sxpos = t - line;
            sypos = height;
        }
        if ((t = strchr(line, 'E'))) {
            expos = t - line;
            eypos = height;
        }
        height++;
    }
    width = strlen(map[0]) - 1;

    int tohere[height][width];
    memset(tohere, -1, sizeof(tohere));
    char pathdir[height][width];
    memset(pathdir, -1, sizeof(pathdir));

    int ypos = sypos, xpos = sxpos, ps = 0;

    while (map[ypos][xpos] != 'E') {
        tohere[ypos][xpos] = ps;
        for (int d = 0; d < 4; d++) {
            int ny = ypos + dirs[d].dy;
            int nx = xpos + dirs[d].dx;

            if (map[ny][nx] == '#')
                continue;
            if (tohere[ny][nx] != -1)
                continue;
            pathdir[ypos][xpos] = d;    // save for easy traversal later on
            ypos = ny;
            xpos = nx;
            break;
        }
        ps++;
    }
    tohere[ypos][xpos] = ps;
    int maxtime = ps;

    for (int part = 0; part < 2; part++) {
        ypos = sypos, xpos = sxpos;
        int saved[maxtime];
        memset(saved, 0, sizeof(saved));

        // calculate all displacement vectors for distance 2 to M

        int M = part ? 20 : 2;
        int MM = 2 * M + 1;

        struct dir2 {
            int dy, dx, dist;
        } dirs2[MM * MM];

        for (int y = 0; y < MM; y++) {
            for (int x = 0; x < MM; x++) {
                int dy = y - M;
                int dx = x - M;
                int dist = abs(dy) + abs(dx);

                if (dist > M || dist == 1) dist = 0; // skip, too far
                dirs2[y * MM + x].dy = dy;
                dirs2[y * MM + x].dx = dx;
                dirs2[y * MM + x].dist = dist;
            }
        }

        for (ps = 0; ps < maxtime; ps++) {      // traverse start positions
            for (int m = 0; m < MM * MM; m++) { // try all vectors
                if (!dirs2[m].dist) continue;   // skip, too far
                int ny = ypos + dirs2[m].dy;
                int nx = xpos + dirs2[m].dx;;
                if (ny < 0 || ny >= height) continue;
                if (nx < 0 || nx >= width) continue;
                if (map[ny][nx] == '#') continue;
                int t1 = tohere[ypos][xpos];
                int t2 = tohere[ny][nx];

                if (t2 < t1) continue;
                int s = t2 - t1 - dirs2[m].dist;

                if (!s) continue;
                if (s < 0 || s >= maxtime) continue;
                saved[s]++;
            }

            int ny = ypos + dirs[pathdir[ypos][xpos]].dy;
            int nx = xpos + dirs[pathdir[ypos][xpos]].dx;
            ypos = ny;
            xpos = nx;
        }

        long long total = 0;

        for (int ps = 100; ps < maxtime; ps++) total += saved[ps];
        printf("part %d: %lld\n", part + 1, total);
    }
}
