#include "HashCell.h"

HashCell::HashCell(float _x, float _y, sf::Font* font, Hash::ColorType _type) :
    cell(radiusHash, 4), type(_type), value(0), state(Hash::empty), mainDiagonal(sf::Vector2f(2 * radiusHash, thicknessHash)), antiDiagonal(sf::Vector2f(2 * radiusHash, thicknessHash))
{
    cell.setOrigin(radiusHash, radiusHash);
    cell.setPosition(_x, _y);
    cell.setRotation(45);
    cell.setOutlineThickness(thicknessHash);
    mainDiagonal.setPosition(cell.getPosition().x - radiusHash / sqrt(2), cell.getPosition().y - radiusHash / sqrt(2));
    mainDiagonal.setOrigin(mainDiagonal.getLocalBounds().left, mainDiagonal.getLocalBounds().top + mainDiagonal.getLocalBounds().height / 2);
    mainDiagonal.setRotation(45);
    antiDiagonal.setPosition(cell.getPosition().x + radiusHash / sqrt(2), cell.getPosition().y - radiusHash / sqrt(2));
    antiDiagonal.setOrigin(antiDiagonal.getLocalBounds().left + antiDiagonal.getLocalBounds().width, antiDiagonal.getLocalBounds().top  + antiDiagonal.getLocalBounds().height / 2);
    antiDiagonal.setRotation(-45);
    valueText.setFont(*font);
    valueText.setString(intToString(value));
    valueText.setCharacterSize(sizeValueLetter);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(_x, _y);
    variableText.setFont(*font);
    variableText.setString("");
    variableText.setCharacterSize(sizeValueLetter);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(_x, _y + radiusHash * 2);
}

std::string HashCell::getVariableString() {
    std::string res = "";
    for (auto it = variableList.begin(); it != variableList.end(); it++) {
        res += *it;
        res += ", ";
    }
    if (res.size() > 0) {
        res.pop_back();
        res.pop_back();
    }
    return res;
}

void HashCell::setValue(int _value) {
    value = _value;
    valueText.setString(intToString(value));
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(cell.getPosition().x, cell.getPosition().y);
}

void HashCell::insertVariable(std::string variable) {
    variableList.insert(variable);
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(cell.getPosition().x, cell.getPosition().y + radiusHash * 2);
}

void HashCell::insertVariable(std::vector <std::string> variables) {
    for (int i = 0; i < variables.size(); i++) {
        variableList.insert(variables[i]);
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(cell.getPosition().x, cell.getPosition().y + radiusHash * 2);
}

void HashCell::deleteVariable(std::string variable) {
    if (variableList.find(variable) != variableList.end()) {
        variableList.erase(variableList.find(variable));
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(cell.getPosition().x, cell.getPosition().y + radiusHash * 2);
}

void HashCell::deleteVariable(std::vector <std::string> variables) {
    for (int i = 0; i < variables.size(); i++) {
        if (variableList.find(variables[i]) != variableList.end()) {
            variableList.erase(variableList.find(variables[i]));
        }
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(cell.getPosition().x, cell.getPosition().y + radiusHash * 2);
}

void HashCell::setState(Hash::State _state) {
    state = _state;
}

void HashCell::setColorType(Hash::ColorType _type) {
    type = _type;
}

int HashCell::getValue() {
    return value;
}

int HashCell::getState() {
    return state;
}

std::vector <std::string> HashCell::getVariables() {
    std::vector <std::string> res;
    for (auto it = variableList.begin(); it != variableList.end(); it++) {
        res.push_back(*it);
    }
    return res;
}

void HashCell::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    cell.setFillColor(Hash::color[theme][type].fillColor);
    cell.setOutlineColor(Hash::color[theme][type].outlineColor);
    valueText.setFillColor(Hash::color[theme][type].valueColor);
    variableText.setFillColor(Hash::color[theme][type].variableColor);
    mainDiagonal.setFillColor(Hash::color[theme][type].outlineColor);
    antiDiagonal.setFillColor(Hash::color[theme][type].outlineColor);
    if (totalTime < epsilonTime) {
        window.draw(cell);
        if (state == Hash::full) {
            window.draw(valueText);
        }
        if (state == Hash::deleted) {
            window.draw(mainDiagonal);
            window.draw(antiDiagonal);
        }
        window.draw(variableText);
    }
    else {
        for (int i = 0; i < animations.size(); i++) {
            for (int j = i + 1; j < animations.size(); j++) {
                if (animations[i].animationType == animations[j].animationType) {
                    assert(false);
                }
            }
        }
        HashCell tmp = *this;
        for (int i = 0; i < animations.size(); i++) {
            if (animations[i].animationType == SetColorType) {
                Hash::Color newColor = Hash::fadingColorType(type, Hash::ColorType(animations[i].nextColorType), theme, timePassed.asSeconds() / totalTime.asSeconds());
                tmp.cell.setFillColor(newColor.fillColor);
                tmp.cell.setOutlineColor(newColor.outlineColor);
                tmp.valueText.setFillColor(newColor.valueColor);
                tmp.variableText.setFillColor(newColor.variableColor);
                tmp.mainDiagonal.setFillColor(newColor.outlineColor);
                tmp.antiDiagonal.setFillColor(newColor.outlineColor);
            }
            if (animations[i].animationType == AnimationType::InsertVariable) {
                tmp.insertVariable(animations[i].variableList);
            }
            if (animations[i].animationType == AnimationType::DeleteVariable) {
                tmp.deleteVariable(animations[i].variableList);
            }
            if (animations[i].animationType == AnimationType::SetState) {
                tmp.setState(Hash::State(animations[i].nextState));
            }
            if (animations[i].animationType == AnimationType::SetValue) {
                tmp.setValue(animations[i].nextValue);
            }
        }
        window.draw(tmp.cell);
        window.draw(tmp.variableText);
        if (tmp.state == Hash::full) {
            window.draw(tmp.valueText);
        }
        if (tmp.state == Hash::deleted) {
            mainDiagonal.setFillColor(Hash::color[theme][type].outlineColor);
            window.draw(tmp.mainDiagonal);
            antiDiagonal.setFillColor(Hash::color[theme][type].outlineColor);
            window.draw(tmp.antiDiagonal);
        }
    }
}