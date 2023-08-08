#pragma once

#include "BTreeGroup.h"

struct BTreeGraph {
    sf::Font* font;
    std::map <int, BTreeNode> nodes;
    std::map <int, BTreeGroup> groups;
    int root;
    sf::Vector2f startPosition;
    BTreeGraph(sf::Vector2f startPosition, sf::Font* font);

    int getGroupID(int id);
    int getParentGroup(int id);
    int getMexNodeID();
    int getMexGroupID();
    std::vector <int> getMexNodeIDs(int size);
    std::vector <int> getMexGroupIDs(int size);
    sf::RectangleShape getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent);

    void DFS(int id, int height, std::vector <std::vector <std::vector <int> > >&tour);
    void arrangeBTree();
    void setPosition(int idGroup, sf::Vector2f position);
    void setEdge(int idGroup, int pos, int id);
    void insertNode(int idGroup, int idNode);
    void deleteNode(int idGroup, int idNode);

    sf::Vector2f getPosition(int idGroup);
    std::vector <int> getEdges(int idGroup);
    sf::Vector2f getStartEdgePosition(int idGroup, int pos, sf::Vector2f curPosition);
    sf::Vector2f getEndEdgePosition(int idGroup, sf::Vector2f curPosition);
    int findEdge(int idGroup, int value);
    int findEdgePos(int idGroup, int value);

    //backend
    BTreeGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};