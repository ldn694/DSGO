#pragma once
#include "AVLNode.h"

struct AVLGraph{
    sf::Font* font;
    std::map <int, AVLNode> nodes;
    int root;
    sf::Vector2f startPosition;
    AVLGraph(sf::Vector2f startPosition, sf::Font* font);

    int getParent(int id);
    int getMexID();
    int getHeigth(int id);
    int getBalanceFactor(int id);
    sf::RectangleShape getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent);

    void arrange(int id, sf::Vector2f position, int depth);
    void arrangeAVLTrees();

    //backend
    AVLGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};