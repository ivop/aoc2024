#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <set>
#include <iostream>

char **lines;
int nlines, space;
char *line;
size_t n;

#define S 26
#define T (S*S)

std::set<int> sets[T];
std::set<int> triplets;
std::set<int> computers;

int tobase26(char a, char b) {
    return (a-'a')*S + (b-'a');
}

int totuple(int a, int b) {
    return a*T+b;
}

int totriplet(int a, int b, int c) {
    return a*T*T + b*T + c;
}

void print(int x) {
     printf("%c%c,", (x/S)+'a', (x%S)+'a');
}

int compar(const void *a, const void *b) {
    int *p = (int *) a;
    int *q = (int *) b;
    if (*p < *q) return -1;
    if (*p > *q) return 1;
    return 0;
}

void add_sorted_triplet(int a, int b, int c) {
    int t[3] = { a, b, c };
    qsort(t, 3, sizeof(int), compar);
    triplets.insert(totriplet(t[0],t[1],t[2]));
}

bool check(std::set<int> &party) {
    for (auto a : party) {
        for (auto b : party) {
            if (a == b) continue;
            if (sets[a].count(b)) continue;
            return false;
        }
    }
    return true;
}

int main(void) {
    while (getline(&line, &n, stdin) > 0) {
        if (nlines == space) {
            space += 16;
            lines = (char **) realloc(lines, space * sizeof(char*));
        }
        lines[nlines] = strdup(line);
        nlines++;
    }

    for (int l=0; l<nlines; l++) {
        char *p = lines[l];
        int left  = tobase26(p[0],p[1]);
        int right = tobase26(p[3],p[4]);
        sets[left].insert(right);
        sets[right].insert(left);
        computers.insert(left);
        computers.insert(right);
    }

    for (int x = 0; x<S; x++) {
        int a = tobase26('t',x+'a');
        for (auto b : sets[a]) {
            if (b == a) continue;
            for (auto c : sets[a]) {
                if (c == a) continue;
                if (c == b) continue;
                if (!sets[c].count(b)) continue;
                add_sorted_triplet(a,b,c);
            }
        }
    }
    printf("part 1: %ld\n", triplets.size());

    int largest = 1;
    std::set<int> largestparty;

    for (auto a : computers) {
        std::set<int> party = { a };
        for (auto b : computers) {
            if (b == a) continue;
            party.insert(b);
            if (check(party)) continue;
            party.erase(party.find(b));
        }
        int size = party.size();
        if (size > largest) {
            largest = size;
            largestparty = party;
        }
    }
    printf("part 2: ");
    for (auto a : largestparty)print(a);
    printf("\b \n");
}
