#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <map>
#include <stack>

typedef unsigned long long ull;
char *line;
size_t n;
std::map<char,std::list<char *>> designs;
std::stack<char *> targets;
std::map<char *, ull> memo;

ull check(char *t) {
    if (!strlen(t)) return 1;
    if (memo.contains(t)) return memo[t];

    ull count = 0;
    for (auto d : designs[t[0]]) {
        if (strncmp(t, d, strlen(d))) continue;
        ull c = check(t+strlen(d));
        memo[t+strlen(d)] = c;
        count += c;
    }
    return count;
}

int main(void) {
    if (getline(&line, &n, stdin) < 0) return 1;
    char *p = strtok(line, ", \n");
    while (p) {
        designs[p[0]].push_back(strdup(p));
        p = strtok(NULL, ", \n");
    }
    if (getline(&line, &n, stdin) < 0) return 1;
    while (getline(&line, &n, stdin) > 0) {
        line[strlen(line)-1] = 0;
        targets.push(strdup(line));
    }

    ull cnt1 = 0, cnt2 = 0;
    while (!targets.empty()) {
        auto t = targets.top();
        ull count = check(t);
        if (count) {
            cnt1++;
            cnt2 += count;
        }
        free(t);
        targets.pop();
    }

    printf("part 1: %lld\n", cnt1);
    printf("part 2: %lld\n", cnt2);
}
