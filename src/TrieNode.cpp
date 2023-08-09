#include "TrieNode.h"


TrieNode::TrieNode(sf::Vector2f pos, std::string value, sf::Font* font) :
circle(radiusTrie), value(value), type(Trie::ColorType::normal) {
    circle.setOrigin(radiusTrie / 2, radiusTrie / 2);
    circle.setPosition(pos);
    circle.setOutlineThickness(thicknessTrie);
    valueText.setFont(*font);
    valueText.setStyle(sf::Text::Bold);
    valueText.setString(value);
    valueText.setCharacterSize(sizeValueLetterTrie);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(pos);
    variableText.setFont(*font);
    variableText.setStyle(sf::Text::Bold);
    variableText.setString("");
    variableText.setCharacterSize(sizeValueLetterTrie);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(pos.x, pos.y + radiusTrie * 2);
}

void TrieNode::setPosition(sf::Vector2f newPosition) {
    circle.setPosition(newPosition);
    valueText.setPosition(newPosition);
    variableText.setPosition(newPosition.x, newPosition.y + radiusTrie * 2);
}

void TrieNode::setSize(float percent) {
    circle.setRadius(radiusTrie * percent);
    circle.setOutlineThickness(thicknessTrie * percent);
    circle.setOrigin(circle.getLocalBounds().left + circle.getLocalBounds().width / 2, circle.getLocalBounds().top + circle.getLocalBounds().height / 2);
    circle.setPosition(circle.getPosition());
    valueText.setCharacterSize(sizeValueLetterTrie * percent);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(circle.getPosition());
    variableText.setCharacterSize(sizeValueLetterTrie * percent);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition() + sf::Vector2f(0, radiusTrie * 2 * percent));
}

void TrieNode::insertEdge(int id) {
    edges.insert(id);
}

void TrieNode::deleteEdge(int id) {
    if (edges.find(id) == edges.end()) {
        assert(false);
    }
    edges.erase(edges.find(id));
}

void TrieNode::setValue(std::string newValue) {
    this->value = value;
    valueText.setString(value);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(circle.getPosition().x, circle.getPosition().y);
}

void TrieNode::setColorType(Trie::ColorType newType) {
    type = newType;
}

std::string TrieNode::getVariableString() {
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

void TrieNode::insertVariable(std::string variable) {
    variableList.insert(variable);
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusTrie * 2);
}

void TrieNode::deleteVariable(std::string variable) {
    variableList.erase(variableList.find(variable));
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusTrie * 2);
}

void TrieNode::insertVariable(std::vector <std::string> variables) {
    for (auto variable : variables) {
        variableList.insert(variable);
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusTrie * 2);
}

void TrieNode::deleteVariable(std::vector <std::string> variables) {
    for (auto variable : variables) {
        variableList.erase(variableList.find(variable));
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusTrie * 2);
}

sf::Vector2f TrieNode::getPosition() {
    return circle.getPosition();
}

std::vector <std::string> TrieNode::getVariables() {
    std::vector <std::string> res;
    for (auto it = variableList.begin(); it != variableList.end(); it++) {
        res.push_back(*it);
    }
    return res;
}

void TrieNode::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    circle.setFillColor(Trie::color[theme][type].fillColor);
    circle.setOutlineColor(Trie::color[theme][type].outlineColor);
    valueText.setFillColor(Trie::color[theme][type].valueColor);
    variableText.setFillColor(Trie::color[theme][type].variableColor);
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
        TrieNode tmp = *this;
        for (int i = 0; i < animations.size(); i++) {
            switch (animations[i].animationType) {
                case SetColorType: {
                    Trie::Color newColor = Trie::fadingColorType(type, Trie::ColorType(animations[i].nextValue), theme, timePassed.asSeconds() / totalTime.asSeconds());
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
                    tmp.setValue(animations[i].nextString);
                    break;
                }
            }
        }
        window.draw(tmp.circle);
        window.draw(tmp.variableText);
        window.draw(tmp.valueText);
    }
}




