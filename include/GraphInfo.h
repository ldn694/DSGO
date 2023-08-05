#pragma once
#include <set>
#include "NodeInfo.h"
#include "EdgeInfo.h"

struct GraphInfo {
    int numNodes;
    std::vector <NodeInfo> nodes;
    std::vector <EdgeInfo> edges;
    GraphInfo(int numNodes, std::vector <NodeInfo> nodes = {}, std::vector <EdgeInfo> edges = {});
    int findMexID();
    void addNode(NodeInfo node);
    void addEdge(int idStart, int idEnd, std::string weight = "");
    void removeNode(int id);
    void removeEdge(int idStart, int idEnd);
    int findEdgeID(int idStart, int idEnd);
    EdgeInfo findEdge(int idStart, int idEnd);
    std::vector <int> getNeighbors(int id);
    sf::RectangleShape getLine(int idStart, int idEnd, float thickness);
    sf::Text getWeight(int idStart, int idEnd, float thickness, float characterSize, sf::Font* font);
    void DFS(int id, std::vector <bool>& visited, std::vector <int>& depth,  std::vector <int>& order);
    GraphInfo rootedAt(int id);
};