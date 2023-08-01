#include "Template.h"
#include "Color.h"

struct NodeInfo {
    sf::Vector2f position;
    int id;
    NodeInfo(float x, float y, int id = -1, int value = 0);
    NodeInfo(sf::Vector2f position, int id = -1, int value = 0);
    void setPosition(float newX, float newY);
    void setPosition(sf::Vector2f newPosition);
};