#include "AVLNode.h"

// AVLNode::AVLNode(float x, float y, int value, sf::Font* font) : 
// circle(radiusAVL), value(value), leftNode(-1), rightNode(-1){
//     circle.setOrigin(radiusAVL / 2, radiusAVL / 2);
//     circle.setPosition(x, y);
//     circle.setOutlineThickness(thicknessAVL);
//     valueText.setFont(*font);
//     valueText.setString(intToString(value));
//     valueText.setCharacterSize(sizeValueLetterAVL);
//     valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
//     valueText.setPosition(x, y);
//     variableText.setFont(*font);
//     variableText.setString("");
//     variableText.setCharacterSize(sizeValueLetterAVL);
//     variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
//     variableText.setPosition(x, y + radiusAVL * 2);
// }

AVLNode::AVLNode(sf::Vector2f pos, int value, sf::Font* font) :
circle(radiusAVL), value(value), leftNode(-1), rightNode(-1), type(AVL::ColorType::normal), leftEdgeType(AVL::ColorType::normal), rightEdgeType(AVL::ColorType::normal){
    circle.setOrigin(radiusAVL / 2, radiusAVL / 2);
    circle.setPosition(pos);
    circle.setOutlineThickness(thicknessAVL);
    valueText.setFont(*font);
    valueText.setStyle(sf::Text::Bold);
    valueText.setString(intToString(value));
    valueText.setCharacterSize(sizeValueLetterAVL);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(pos);
    variableText.setFont(*font);
    variableText.setStyle(sf::Text::Bold);
    variableText.setString("");
    variableText.setCharacterSize(sizeValueLetterAVL);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(pos.x, pos.y + radiusAVL * 2);
}

void AVLNode::setPosition(float newX, float newY) {
    circle.setPosition(newX, newY);
    valueText.setPosition(newX, newY);
    variableText.setPosition(newX, newY + radiusAVL * 2);
}

void AVLNode::setPosition(sf::Vector2f newPosition) {
    circle.setPosition(newPosition);
    valueText.setPosition(newPosition);
    variableText.setPosition(newPosition.x, newPosition.y + radiusAVL * 2);
}

void AVLNode::setSize(float percent) {
    circle.setRadius(radiusAVL * percent);
    circle.setOutlineThickness(thicknessAVL * percent);
    circle.setOrigin(circle.getLocalBounds().left + circle.getLocalBounds().width / 2, circle.getLocalBounds().top + circle.getLocalBounds().height / 2);
    circle.setPosition(circle.getPosition());
    valueText.setCharacterSize(sizeValueLetterAVL * percent);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(circle.getPosition());
    variableText.setCharacterSize(sizeValueLetterAVL * percent);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition() + sf::Vector2f(0, radiusAVL * 2 * percent));
}

void AVLNode::setValue(int value) {
    this->value = value;
    valueText.setString(intToString(value));
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(circle.getPosition().x, circle.getPosition().y);
}

void AVLNode::setLeftNode(int newLeftNode) {
    leftNode = newLeftNode;
}

void AVLNode::setRightNode(int newRightNode) {
    rightNode = newRightNode;
}

void AVLNode::setColorType(AVL::ColorType newType) {
    type = newType;
}

std::string AVLNode::getVariableString() {
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

void AVLNode::insertVariable(std::string variable) {
    variableList.insert(variable);
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusAVL * 2);
}

void AVLNode::deleteVariable(std::string variable) {
    variableList.erase(variableList.find(variable));
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusAVL * 2);
}

void AVLNode::insertVariable(std::vector <std::string> variables) {
    for (auto variable : variables) {
        variableList.insert(variable);
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusAVL * 2);
}

void AVLNode::deleteVariable(std::vector <std::string> variables) {
    for (auto variable : variables) {
        variableList.erase(variableList.find(variable));
    }
    variableText.setString(getVariableString());
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(circle.getPosition().x, circle.getPosition().y + radiusAVL * 2);
}

sf::Vector2f AVLNode::getPosition() {
    return circle.getPosition();
}

std::vector <std::string> AVLNode::getVariables() {
    std::vector <std::string> res;
    for (auto it = variableList.begin(); it != variableList.end(); it++) {
        res.push_back(*it);
    }
    return res;
}

void AVLNode::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    circle.setFillColor(AVL::color[theme][type].fillColor);
    circle.setOutlineColor(AVL::color[theme][type].outlineColor);
    valueText.setFillColor(AVL::color[theme][type].valueColor);
    variableText.setFillColor(AVL::color[theme][type].variableColor);
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
        AVLNode tmp = *this;
        for (int i = 0; i < animations.size(); i++) {
            switch (animations[i].animationType) {
                case SetColorType: {
                    AVL::Color newColor = AVL::fadingColorType(type, AVL::ColorType(animations[i].nextValue), theme, timePassed.asSeconds() / totalTime.asSeconds());
                    tmp.circle.setFillColor(newColor.fillColor);
                    tmp.circle.setOutlineColor(newColor.outlineColor);
                    tmp.valueText.setFillColor(newColor.valueColor);
                    tmp.variableText.setFillColor(newColor.variableColor);
                    break;
                }
                case SetLeftEdgeColorType: {
                    tmp.leftEdgeType = AVL::ColorType(animations[i].nextValue);
                    break;
                }
                case SetRightEdgeColorType: {
                    tmp.rightEdgeType = AVL::ColorType(animations[i].nextValue);
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
                case SetLeftNode: {
                    tmp.setLeftNode(animations[i].nextValue);
                    break;
                }
                case SetRightNode: {
                    tmp.setRightNode(animations[i].nextValue);
                    break;
                }
            }
        }
        window.draw(tmp.circle);
        window.draw(tmp.variableText);
        window.draw(tmp.valueText);
    }
}




