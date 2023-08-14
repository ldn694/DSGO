#pragma once
#include <functional>
#include "GeneralNode.h"
#include "GeneralEdge.h"

struct GeneralGraph{
    sf::Font* font;
    std::map <int, GeneralNode> nodes;
    std::set <GeneralEdge> edges;
    sf::RectangleShape viewRect;

    sf::Vector2f fAttract(int u, int v, std::map <int, sf::Vector2f>& pos);
    sf::Vector2f fRepel(int u, int v, std::map <int, sf::Vector2f>& pos);

    GeneralGraph(std::vector <GeneralEdge> edges, sf::RectangleShape viewRect, sf::Font* font);

    void setEdges(std::vector <GeneralEdge> edges);

    int getMexID();
    std::pair <sf::RectangleShape, sf::Text> getEdgeLine(int u, int v, bool directed);

    void arrangeGraph();

    //backend
    GeneralGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};