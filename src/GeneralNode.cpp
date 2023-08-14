#include "GeneralNode.h"

GeneralNode::GeneralNode(sf::Vector2f pos, int value, sf::Font* font) :
circle(radiusGraph), value(value), type(General::ColorType::normal){
    circle.setOrigin(radiusGraph / 2, radiusGraph / 2);
    circle.setPosition(pos);
    circle.setOutlineThickness(thicknessGraph);
    valueText.setFont(*font);
    valueText.setStyle(sf::Text::Bold);
    valueText.setString(intToString(value));
    valueText.setCharacterSize(sizeValueLetterGraph);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(pos);
    variableText.setFont(*font);
    variableText.setStyle(sf::Text::Bold);
    variableText.setString("");
    variableText.setCharacterSize(sizeValueLetterGraph);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(pos.x, pos.y + radiusGraph * 2);
}

void GeneralNode::setPosition(float newX, float newY) {
    circle.setPosition(newX, newY);
    valueText.setPosition(newX, newY);
    variableText.setPosition(newX, newY + radiusGraph * 2);
}

void GeneralNode::setPosition(sf::Vector2f newPosition) {
    circle.setPosition(newPosition);
    valueText.setPosition(newPosition);
    variableText.setPosition(newPosition.x, newPosition.y + radiusGraph * 2);
}

void GeneralNode::setSize(float percent) {
    circle.setRadius(radiusGraph * percent);
    circle.setOutlineThickness(thicknessGraph * percent);
    circle.setOrigin(circle.getLocalBounds().left + circle.getLocalBounds().width / 2, circle.getLocalBounds().top + circle.getLocalBounds().height / 2);
    circle.setPosition(circle.getPosition());
    valueText.setCharacterSize(sizeValueLetterGraph * percent);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(circle.getPosition());
    variableText.setCharacterSize(sizeValueLetterGraph * percent);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition() + sf::Vector2f(0, radiusGraph * 2 * percent));
}

void GeneralNode::setValue(int value) {
    this->value = value;
    valueText.setString(intToString(value));
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(circle.getPosition().x, circle.getPosition().y);
}

void GeneralNode::setColorType(General::ColorType newType) {
    type = newType;
}

std::string GeneralNode::getVariableString() {
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

void GeneralNode::insertVariable(std::string variable) {
    variableList.insert(variable);
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusGraph * 2);
}

void GeneralNode::deleteVariable(std::string variable) {
    variableList.erase(variableList.find(variable));
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusGraph * 2);
}

void GeneralNode::insertVariable(std::vector <std::string> variables) {
    for (auto variable : variables) {
        variableList.insert(variable);
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusGraph * 2);
}

void GeneralNode::deleteVariable(std::vector <std::string> variables) {
    for (auto variable : variables) {
        variableList.erase(variableList.find(variable));
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusGraph * 2);
}

sf::Vector2f GeneralNode::getPosition() {
    return circle.getPosition();
}

std::vector <std::string> GeneralNode::getVariables() {
    std::vector <std::string> res;
    for (auto it = variableList.begin(); it != variableList.end(); it++) {
        res.push_back(*it);
    }
    return res;
}

void GeneralNode::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    circle.setFillColor(General::color[theme][type].fillColor);
    circle.setOutlineColor(General::color[theme][type].outlineColor);
    valueText.setFillColor(General::color[theme][type].valueColor);
    variableText.setFillColor(General::color[theme][type].variableColor);
    if (totalTime < epsilonTime) {
        window.draw(circle);
        window.draw(variableText);
        window.draw(valueText);
    }
    else {
        for (int i = 0; i < animations.size(); i++) {
            for (int j = i + 1; j < animations.size(); j++) {
                if (animations[i].animationType == animations[j].animationType) {
                    assert(false);
                }
            }
        }
        GeneralNode tmp = *this;
        for (int i = 0; i < animations.size(); i++) {
            switch (animations[i].animationType) {
                case SetColorType: {
                    General::Color newColor = General::fadingColorType(type, General::ColorType(animations[i].nextValue), theme, timePassed.asSeconds() / totalTime.asSeconds());
                    tmp.circle.setFillColor(newColor.fillColor);
                    tmp.circle.setOutlineColor(newColor.outlineColor);
                    tmp.valueText.setFillColor(newColor.valueColor);
                    tmp.variableText.setFillColor(newColor.variableColor);
                    break;
                }
                case Move: {
                    sf::Vector2f diff = animations[i].nextPosition - tmp.circle.getPosition();
                    tmp.circle.move(diff * timePassed.asSeconds() / totalTime.asSeconds());
                    tmp.valueText.move(diff * timePassed.asSeconds() / totalTime.asSeconds());
                    tmp.variableText.move(diff * timePassed.asSeconds() / totalTime.asSeconds());
                    break;
                }
                case InsertVariable: {
                    tmp.insertVariable(animations[i].variableList);
                    break;
                }
                case DeleteVariable: {
                    tmp.deleteVariable(animations[i].variableList);
                    break;
                }
                case AddNode: {
                    float percent = timePassed.asSeconds() / totalTime.asSeconds();
                    tmp.setSize(percent);
                    break;
                }
                case DeleteNode: {
                    float percent = 1 - timePassed.asSeconds() / totalTime.asSeconds();
                    tmp.setSize(percent);
                    break;
                }
                case SetValue: {
                    tmp.setValue(animations[i].nextValue);
                    break;
                }
            }
        }
        window.draw(tmp.circle);
        window.draw(tmp.variableText);
        window.draw(tmp.valueText);
    }
}




