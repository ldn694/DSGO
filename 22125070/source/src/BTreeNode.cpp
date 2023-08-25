#include "BTreeNode.h"

BTreeNode::BTreeNode(sf::Vector2f pos, int value, sf::Font* font) :
rect(sf::Vector2f(edgeBTree - thicknessBTree, edgeBTree - thicknessBTree)), value(value),  type(BTree::ColorType::normal){
    rect.setOutlineThickness(thicknessBTree);
    rect.setOrigin(rect.getLocalBounds().left + rect.getLocalBounds().width / 2, rect.getLocalBounds().top + rect.getLocalBounds().height / 2);
    rect.setPosition(pos);
    valueText.setFont(*font);
    valueText.setStyle(sf::Text::Bold);
    valueText.setString(intToString(value));
    valueText.setCharacterSize(sizeValueLetterBTree);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(pos);
}

void BTreeNode::setPosition(float newX, float newY) {
    rect.setPosition(newX, newY);
    valueText.setPosition(newX, newY);
}

void BTreeNode::setPosition(sf::Vector2f newPosition) {
    rect.setPosition(newPosition);
    valueText.setPosition(newPosition);
}

void BTreeNode::setSize(float percent) {
    rect.setSize(sf::Vector2f((edgeBTree - thicknessBTree) * percent, (edgeBTree - thicknessBTree) * percent));
    rect.setOutlineThickness(thicknessBTree * percent);
    rect.setOrigin(rect.getLocalBounds().left + rect.getLocalBounds().width / 2, rect.getLocalBounds().top + rect.getLocalBounds().height / 2);
    rect.setPosition(rect.getPosition());
    valueText.setCharacterSize(sizeValueLetterBTree * percent);
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(rect.getPosition());
}

void BTreeNode::setValue(int value) {
    this->value = value;
    valueText.setString(intToString(value));
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(rect.getPosition().x, rect.getPosition().y);
}

void BTreeNode::setColorType(BTree::ColorType newType) {
    type = newType;
}

sf::Vector2f BTreeNode::getPosition() {
    return rect.getPosition();
}

int BTreeNode::getValue() {
    return value;
}

void BTreeNode::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    rect.setFillColor(BTree::color[theme][type].fillColor);
    rect.setOutlineColor(BTree::color[theme][type].outlineColor);
    valueText.setFillColor(BTree::color[theme][type].valueColor);
    if (totalTime < epsilonTime) {
        window.draw(rect);
        window.draw(valueText);
    }
    else {
        // for (int i = 0; i < animations.size(); i++) {
        //     for (int j = i + 1; j < animations.size(); j++) {
        //         if (animations[i].animationType == animations[j].animationType) {
        //             assert(false);
        //         }
        //     }
        // }
        BTreeNode tmp = *this;
        for (int i = 0; i < animations.size(); i++) {
            switch (animations[i].animationType) {
                case SetColorType: {
                    BTree::Color newColor = BTree::fadingColorType(type, BTree::ColorType(animations[i].nextValue), theme, timePassed.asSeconds() / totalTime.asSeconds());
                    tmp.rect.setFillColor(newColor.fillColor);
                    tmp.rect.setOutlineColor(newColor.outlineColor);
                    tmp.valueText.setFillColor(newColor.valueColor);
                    break;
                }
                case Move: {
                    sf::Vector2f diff = animations[i].nextPosition - tmp.rect.getPosition();
                    tmp.rect.move(diff * timePassed.asSeconds() / totalTime.asSeconds());
                    tmp.valueText.move(diff * timePassed.asSeconds() / totalTime.asSeconds());
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
        window.draw(tmp.rect);
        window.draw(tmp.valueText);
    }
}




