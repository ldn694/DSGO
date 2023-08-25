#pragma once
#include "AVLNode.h"

struct HeapGraph{
    sf::Font* font;
    std::map <int, AVLNode> nodes;
    std::map <int, int> heapID;
    int root;
    sf::Vector2f startPosition;
    HeapGraph(sf::Vector2f startPosition, sf::Font* font);

    int getRealID(int hID);
    int getRealParent(int id);
    int getHeapID(int id);
    int getHeapParent(int id);
    int getMexID();
    int getHeapHeigth(int id);
    sf::RectangleShape getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent);

    sf::Vector2f getPosition(int id);
    void arrange(int id, sf::Vector2f position, int depth);
    void arrangeHeapTrees();

    //backend
    HeapGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};