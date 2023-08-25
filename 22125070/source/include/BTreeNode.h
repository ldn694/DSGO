#pragma once
#include <set>
#include <cassert>
#include "Color.h"
#include "Template.h"

struct BTreeNode {
    sf::RectangleShape rect;
    int value;
    BTree::ColorType type;
    sf::Text valueText;
    BTreeNode(sf::Vector2f position = {0, 0}, int value = 0, sf::Font* font = nullptr);
    //backend
    void setPosition(float newX, float newY);
    void setPosition(sf::Vector2f newPosition);
    void setSize(float newPercent);
    void setValue(int newValue);
    void setColorType(BTree::ColorType newType);


    sf::Vector2f getPosition();
    int getValue();
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};