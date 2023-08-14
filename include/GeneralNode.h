#pragma once
#include <set>
#include <cassert>
#include "Color.h"
#include "Template.h"

struct GeneralNode {
    sf::CircleShape circle;
    int value;
    General::ColorType type;
    sf::Text valueText;
    std::set <std::string> variableList;
    sf::Text variableText;
    GeneralNode(sf::Vector2f position = {0, 0}, int value = 0, sf::Font* font = nullptr);
    //backend
    void setPosition(float newX, float newY);
    void setPosition(sf::Vector2f newPosition);
    void setSize(float newPercent);
    void setValue(int newValue);
    void insertVariable(std::string variable);
    void deleteVariable(std::string variable);
    void insertVariable(std::vector <std::string> variables);
    void deleteVariable(std::vector <std::string> variables);
    void setColorType(General::ColorType newType);


    sf::Vector2f getPosition();
    std::vector <std::string> getVariables();
    int getValue();
    std::string getVariableString();
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};