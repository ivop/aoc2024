#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <unistd.h>
#include <map>
#include <string>
#include <set>

typedef unsigned long long ull;
char *line;
size_t n;

std::map<std::string,int> nets;

enum oper { OPER_AND, OPER_OR, OPER_XOR };

struct gate {
    std::string net1, net2;
    enum oper oper;
};

std::map<std::string, struct gate> gates;

void calc(const std::string &net) {
    if (nets.count(net)) return;
    if (!gates.count(net)) return;
    if (!nets.count(gates[net].net1)) calc(gates[net].net1);
    if (!nets.count(gates[net].net2)) calc(gates[net].net2);
    switch (gates[net].oper) {
    case OPER_AND:
        nets[net] = nets[gates[net].net1] & nets[gates[net].net2];
        break;
    case OPER_OR:
        nets[net] = nets[gates[net].net1] | nets[gates[net].net2];
        break;
    case OPER_XOR:
        nets[net] = nets[gates[net].net1] ^ nets[gates[net].net2];
        break;
    }
}

bool find_gate(int oper, std::string y_n, std::string x_n, std::string &p_n) {
    for (auto g : gates) {
        if ( (g.second.net1 == y_n && g.second.net2 == x_n) ||
             (g.second.net1 == x_n && g.second.net2 == y_n)) {
            if  (g.second.oper == oper) {
                p_n = g.first;
                return true;
            }
        }
    }
    return false;
}

bool find_other_gate(int oper, std::string y_n, std::string x_n, std::string &p_n) {
    for (auto g : gates) {
        if ( (g.second.net1 == y_n && g.second.net2 == x_n) ||
             (g.second.net1 == x_n && g.second.net2 == y_n)) {
            if  (g.second.oper != oper) {   // NOT equals
                p_n = g.first;
                return true;
            }
        }
    }
    return false;
}

void swap(std::string g1, std::string g2) {
    struct gate temp = gates[g1];
    gates[g1] = gates[g2];
    gates[g2] = temp;
}

int main(void) {
    while (1) {
        getline(&line, &n, stdin);
        if (strlen(line) == 1) break;
        char *p = strtok(line, ":");
        char *net = strdup(p);
        p = strtok(NULL, ":");
        int value = atoi(p);
        nets[net] = value;
    }

    while (getline(&line, &n, stdin) > 0) {
        char *net1, *op, *net2, *out;
        char *p = strtok(line, " ");
        net1 = strdup(p);
        p = strtok(NULL, " ");
        op = strdup(p);
        p = strtok(NULL, " ");
        net2 = strdup(p);
        p = strtok(NULL, " ");
        p = strtok(NULL, " \n");
        out = strdup(p);
        enum oper oper = OPER_AND;
        if (!strcmp(op, "OR")) oper = OPER_OR;
        else if (!strcmp(op, "XOR")) oper = OPER_XOR;
        struct gate gate = { net1, net2, oper };
        gates[out] = gate;
    }

    int nzs = 0;
    for (auto o : gates) {
        if (o.first[0] == 'z') {
            nzs++;
            calc(o.first);
        }
    }

    ull z = 0;
    for (int i=0; i<nzs; i++) {
        char zs[4];
        snprintf(zs, 4, "z%02d", i);
        if (nets[zs]) z |= 1ULL<<i;
    }
    printf("part 1: %lld\n", z);

// (                    (x00 y00 AND[mcg])         (x01 y01 XOR[vcn]) XOR[z01])
// ( ((mcg vcn AND[ndb])(y01 x01 AND[gbh])OR[pqk]) (y02 x02 XOR[srh]) XOR[z02])
// ( ((srh pqk AND[kkk])(x02 y02 AND[rgb])OR[gkk]) (y03 x03 XOR[pvt]) XOR[z03])
// ( ((pvt gkk AND[pwn])(x03 y03 AND[ngs])OR[vfk]) (x04 y04 XOR[wfj]) XOR[z04])
//     ----------------  ----------------           ----------------
//          a_n                q_n                       p_n
//    -----------------------------------
//                    o_n
//
// we need p_n = y_n   XOR x_n
// we need q_n = y_n-1 AND x_n-1
// we need a_n = p_n-1 AND o_n-1
// we need o_n = a_n   OR  q_n
//         z_n = o_n   XOR p_n
//
// z00 and z01 are assumed correct
// not all possible errors are handled, the unhandled ones did not occur

    std::string o_n_1, p_n_1;
    for (auto g : gates) {
        if ( (g.second.net1 == "x00" && g.second.net2 == "y00" && g.second.oper == OPER_AND) ||
             (g.second.net2 == "x00" && g.second.net1 == "y00" && g.second.oper == OPER_AND) ) {
            o_n_1 = g.first;
        }
        if ( (g.second.net1 == "x01" && g.second.net2 == "y01" && g.second.oper == OPER_XOR) ||
             (g.second.net2 == "x01" && g.second.net1 == "y01" && g.second.oper == OPER_XOR) ) {
            p_n_1 = g.first;
        }
    }

    std::set<std::string> swapped;

    for (int i=2; i<nzs-1; i++) {
restart:
        char x_n_1[4], y_n_1[4], x_n[4], y_n[4], z_n[4];
        std::string p_n, q_n, a_n, o_n, zfound;
        snprintf(x_n_1,4,"x%02d",i-1);
        snprintf(y_n_1,4,"y%02d",i-1);
        snprintf(x_n,4,"x%02d",i);
        snprintf(y_n,4,"y%02d",i);
        snprintf(z_n,4,"z%02d",i);
        // we need p_n = y_n   XOR x_n
        if (!find_gate(OPER_XOR, y_n, x_n, p_n)) {
            printf("unhandled, err1, i=%d\n", i);
            return 1;
        }
        // we need q_n = y_n-1 AND x_n-1
        if (!find_gate(OPER_AND, y_n_1, x_n_1, q_n)) {
            printf("unhandled, err2, i=%d\n", i);
            return 1;
        }
        // we need a_n = p_n-1 AND o_n-1
        if (!find_gate(OPER_AND, p_n_1, o_n_1, a_n)) {
            printf("unhandled, err3, i=%d\n", i);
            return 1;
        }
        // we need o_n = a_n   OR  q_n
        if (!find_gate(OPER_OR, a_n, q_n, o_n)) {
            printf("unhandled, err4, i=%d\n", i);
            return 1;
        }
        //         z_n = o_n   XOR p_n
        if (!find_gate(OPER_XOR, o_n, p_n, zfound)) {
            if (gates[z_n].net1 != p_n && gates[z_n].net2 != p_n) {
                std::string good;
                find_other_gate(OPER_XOR, y_n, x_n, good);
                swapped.insert(p_n);
                swapped.insert(good);
                swap(p_n, good);
                goto restart;
            } else if (gates[z_n].net1 != o_n && gates[z_n].net2 != o_n) {
                printf("unhandled, o_n: %s wrong\n", o_n.c_str());
                return 1;
            }
        }
        if (zfound.compare(z_n) != 0) {
            swapped.insert(z_n);
            swapped.insert(zfound);
            swap(z_n, zfound);
            goto restart;
        }

        // propagate for next loop
        p_n_1 = p_n;
        o_n_1 = o_n;
    }

    printf("part 2: ");
    for (auto i : swapped) {
        printf("%s,", i.c_str());
    }
    printf("\b \n");
}
