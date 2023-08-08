#include "BTreeGroup.h"

BTreeGroup::BTreeGroup(std::vector <int> nodes): nodes(nodes) {
    listEdge.resize(nodes.size() + 1, -1);
}

void BTreeGroup::setEdge(int pos, int id) {
    if (pos > nodes.size()) {
        while (listEdge.size() <= pos) {
            listEdge.push_back(-1);
        }
    }
    listEdge[pos] = id;
}

sf::Vector2f BTreeGroup::getSize() {
    if (nodes.empty()) {
        return sf::Vector2f(0, 0);
    }
    return sf::Vector2f(nodes.size() * edgeBTree + thicknessBTree, edgeBTree + thicknessBTree);
}