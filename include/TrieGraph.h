#pragma once
#include <functional>
#include "TrieNode.h"

struct TrieGraph{
    sf::Font* font;
    std::map <int, TrieNode> nodes;
    int root;
    sf::Vector2f startPosition;
    TrieGraph(sf::Vector2f startPosition, sf::Font* font);

    int getParent(int id);
    int getMexID();
    int findEdge(int id, std::string weight);
    int countString();
    std::pair <sf::RectangleShape, sf::Text> getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, std::string weight, float percent);
    void setThemeEdge(std::pair <sf::RectangleShape, sf::Text> &edge, Trie::ColorType type, ColorTheme theme);

    void DFS(int id, int height, std::vector <std::pair<int, int> > &tour);
    void arrangeTrieTree();

    //backend
    TrieGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};