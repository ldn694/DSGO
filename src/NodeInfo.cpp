#include "NodeInfo.h"

NodeInfo::NodeInfo(float x, float y, int id, int value) :
    position(sf::Vector2f(x, y)), id(id) {}

NodeInfo::NodeInfo(sf::Vector2f position, int id, int value) :
    position(position), id(id) {}

void NodeInfo::setPosition(float newX, float newY) {
    position = sf::Vector2f(newX, newY);
}

void NodeInfo::setPosition(sf::Vector2f newPosition) {
    position = newPosition;
}