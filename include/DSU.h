#pragma once
#include <vector>

struct DSU {
    int n;
    std::vector <int> lab;
    DSU(int n);
    int find(int v);
    int getSize(int v);
    bool join(int a, int b);
};