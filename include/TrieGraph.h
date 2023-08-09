#pragma once
#include "TrieNode.h"

struct TrieGraph{
    sf::Font* font;
    std::map <int, TrieNode> nodes;
    int root;
    sf::Vector2f startPosition;
    TrieGraph(sf::Vector2f startPosition, sf::Font* font);

    int getParent(int id);
    int getMexID();
    sf::RectangleShape getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent);

    void DFS(int id, int height, std::vector <std::vector <int> > &tour);
    void arrangeTrieTree();

    //backend
    TrieGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};