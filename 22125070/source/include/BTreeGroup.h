#pragma once

#include <cassert>
#include <set>
#include <functional>
#include <algorithm>
#include "BTreeNode.h"

struct BTreeGroup {
    std::vector <int> nodes;
    std::vector <int> listEdge;
    std::set <std::string> variableList;
    BTreeGroup(std::vector <int> nodes = std::vector <int>());
    void refreshVariableTextPosition();

    void setEdge(int pos, int id);
    void insertNode(int idNode);
    void deleteNode(BTreeNode* idNode);
    bool isLeaf();

    sf::Vector2f getSize();
};