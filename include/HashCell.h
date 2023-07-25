#pragma once
#include <set>
#include <vector>
#include <string.h>
#include <cassert>
#include "Template.h"
#include "Color.h"

namespace Hash {
    enum State {
        empty, full, deleted
    };
};

struct HashCell {
    sf::CircleShape cell;
    sf::RectangleShape mainDiagonal, antiDiagonal;
    Hash::ColorType type;
    int value;
    int state; //state = 0: empty, state = 1: full, state = 2: deleted
    sf::Text valueText;
    std::set <std::string> variableList;
    sf::Text variableText;

    HashCell(float x = 0, float y = 0, sf::Font* font = nullptr, Hash::ColorType type = Hash::normal);
    //backend
    std::string getVariableString();
    void setValue(int value);
    void insertVariable(std::string variable);
    void deleteVariable(std::string variable);
    void insertVariable(std::vector <std::string> variables);
    void deleteVariable(std::vector <std::string> variables);
    void setState(Hash::State state);
    void setColorType(Hash::ColorType type);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};