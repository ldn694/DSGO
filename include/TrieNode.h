#pragma once
#include <set>
#include <cassert>
#include "Color.h"
#include "Template.h"

struct TrieNode {
    sf::CircleShape circle;
    bool state; //0: not word, 1: is word
    std::map <int, std::string> edges;
    std::string value;
    Trie::ColorType type;
    sf::Text valueText;
    std::set <std::string> variableList;
    sf::Text variableText;
    //AVLNode(float x = 0.f, float y = 0.f, int value = 0, sf::Font* font = nullptr);
    TrieNode(sf::Vector2f position = {0, 0}, std::string value = "", sf::Font* font = nullptr);
    //backend
    void setPosition(sf::Vector2f newPosition);
    void setSize(float newPercent);
    void setValue(std::string newValue);
    void insertEdge(int id, std::string weight);
    void deleteEdge(int id);
    void insertVariable(std::string variable);
    void deleteVariable(std::string variable);
    void insertVariable(std::vector <std::string> variables);
    void deleteVariable(std::vector <std::string> variables);
    void setColorType(Trie::ColorType newType);
    void setState(bool newState);


    sf::Vector2f getPosition();
    std::vector <std::string> getVariables();
    int getValue();
    bool getState();
    std::string getVariableString();
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};