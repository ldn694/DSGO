#pragma once
#include <functional>
#include "GeneralNode.h"
#include "GeneralEdge.h"
#include "DSU.h"

struct GeneralGraph{
    bool isDirected = false;
    sf::Font* font;
    std::map <int, GeneralNode> nodes;
    std::set <GeneralEdge> edges;
    sf::FloatRect viewRect;

    float fAttract(float x);
    float fRepel(float x);
    sf::Vector2f springForce(sf::Vector2f stablePosition, sf::Vector2f mobilePosition, float idealLength);
    sf::Vector2f repelForce(sf::Vector2f stablePosition, sf::Vector2f mobilePosition, float idealLength);

    GeneralGraph(std::vector <GeneralEdge> edges, sf::FloatRect viewRect, sf::Font* font);

    std::set <GeneralEdge>::iterator findEdge(int from, int to);

    std::vector <sf::RectangleShape> getEdgeLines(sf::Vector2f startPosition, sf::Vector2f endPosition, bool directed, bool upward = false);
    std::vector <sf::Text> getEdgeWeightText(sf::Vector2f startPosition, sf::Vector2f endPosition, int weight, bool upward = false);

    void setEdges(std::vector <GeneralEdge> edges, bool directed = false, int maxSize = 0);

    int getMexID();
    std::pair <sf::RectangleShape, sf::Text> getEdgeLine(int u, int v, bool directed);

    void arrangeGraph();

    //backend
    GeneralGraph execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};