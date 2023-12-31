#include "DSU.h"

DSU::DSU(int n) : n(n) {
    lab.resize(n + 10);
    for (int i = 0; i <= n; ++i) {
        lab[i] = -1;
    }
}

int DSU::find(int v) {
    return lab[v] < 0 ? v : lab[v] = find(lab[v]);
}

std::vector <int> DSU::getComponents(int u) {
    std::vector <int> tmp;
    for (int i = 1; i <= n; i++) {
        if (find(i) == find(u)) {
            tmp.push_back(i);
        }
    }
    return tmp;
}

bool DSU::join(int a, int b) {
    a = find(a);
    b = find(b);
    
    if (a != b) {
        if (lab[a] > lab[b]) std::swap(a, b);
        lab[a] += lab[b];
        lab[b] = a;
        return true;
    }
    return false;
}

int DSU::getSize(int v) {
    return -lab[find(v)];
}