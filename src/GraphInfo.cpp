#include "GraphInfo.h"

GraphInfo::GraphInfo(int numNodes, std::vector <NodeInfo> nodes, std::vector <EdgeInfo> edges) :
    numNodes(numNodes), nodes(nodes), edges(edges) {}

int GraphInfo::findMexID() {
    std::set <int> idList;
    for (int i = 0; i < numNodes; i++) {
        idList.insert(i);
    }
    int i = 0;
    while (true) {
        if (idList.find(i) == idList.end()) {
            return i;
        }
        i++;
    }
}

void GraphInfo::addNode(NodeInfo node) {
    if (node.id == -1) {
        node.id = findMexID();
    }
    nodes.push_back(node);
    numNodes++;
}

void GraphInfo::addEdge(int idStart, int idEnd, std::string weight) {
    edges.push_back(EdgeInfo(idStart, idEnd, weight));
}

void GraphInfo::removeNode(int id) {
    for (int i = 0; i < numNodes; i++) {
        if (nodes[i].id == id) {
            nodes.erase(nodes.begin() + i);
            numNodes--;
            break;
        }
    }
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].idStart == id || edges[i].idEnd == id) {
            edges.erase(edges.begin() + i);
            i--;
        }
    }
}

std::vector <int> GraphInfo::getNeighbors(int id) {
    std::vector <int> neighbors;
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].idStart == id) {
            neighbors.push_back(edges[i].idEnd);
        }
    }
    return neighbors;
}

int GraphInfo::findEdgeID(int idStart, int idEnd) {
    for (int i = 0; i < edges.size(); i++) {
        if (edges[i].idStart == idStart && edges[i].idEnd == idEnd) {
            return i;
        }
    }
    return -1;
}

EdgeInfo GraphInfo::findEdge(int idStart, int idEnd) {
    int id = findEdgeID(idStart, idEnd);
    if (id == -1) {
        return EdgeInfo(-1, -1);
    }
    return edges[id];
}

sf::RectangleShape GraphInfo::getLine(int idStart, int idEnd, float thickness) {
    sf::Vector2f positionStart = nodes[idStart].position;
    sf::Vector2f positionEnd = nodes[idEnd].position;
    sf::Vector2f positionDifference = positionEnd - positionStart;
    sf::RectangleShape line(sf::Vector2f(length(positionDifference), thickness));
    line.setOrigin(0, thickness / 2);
    line.setPosition(positionStart);
    line.setRotation(atan2(positionEnd.y - positionStart.y, positionEnd.x - positionStart.x) * 180 / PI);
    return line;
}

sf::Text GraphInfo::getWeight(int idStart, int idEnd, float thickness, float characterSize, sf::Font* font) {
    sf::Vector2f positionStart = nodes[idStart].position;
    sf::Vector2f positionEnd = nodes[idEnd].position;
    sf::Vector2f positionDifference = positionEnd - positionStart;
    sf::Text weight(edges[findEdgeID(idStart, idEnd)].weight, *font);
    weight.setCharacterSize(characterSize);
    weight.setPosition(weight.getLocalBounds().left + weight.getLocalBounds().width / 2, weight.getLocalBounds().top + weight.getLocalBounds().height / 2);
    float x = positionStart.x + positionDifference.x / 2;
    float y = positionStart.y + positionDifference.y / 2;
    MovePointUpward(x, y, positionEnd.x, positionEnd.y, thickness * 2);
    weight.setPosition(x, y);
}


void GraphInfo::DFS(int u, std::vector <bool>& visited, std::vector <int>& depth, std::vector <int>& order) {
    visited[u] = true;
    order.push_back(u);
    for (int v : getNeighbors(u)) {
        if (!visited[v]) {
            depth[v] = depth[u] + 1;;
            DFS(v, visited, depth, order);
        }
    }
}

GraphInfo GraphInfo::rootedAt(int id) {
    std::vector <bool> visited(numNodes, false);
    std::vector <int> depth(numNodes, 0);
    std::vector <int> order;
    DFS(id, visited, depth, order);
    int L = 0, R = 0;
    //while (R < )
}
