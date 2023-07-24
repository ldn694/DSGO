#include "HashTable.h"

//-----------------------------------------------------------------------------------
//HashCell

HashCell::HashCell(float _x, float _y, sf::Font* font, Hash::ColorType _type) :
    cell(radiusHash, 4), type(_type), value(0), variable(""), state(Hash::empty), mainDiagonal(sf::Vector2f(2 * radiusHash, thicknessHash)), antiDiagonal(sf::Vector2f(2 * radiusHash, thicknessHash))
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
    variableText.setString(variable);
    variableText.setCharacterSize(sizeValueLetter);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(_x, _y + radiusHash * 2);
}

void HashCell::setValue(int _value) {
    value = _value;
    valueText.setString(intToString(value));
    valueText.setOrigin(valueText.getLocalBounds().left + valueText.getLocalBounds().width / 2, valueText.getLocalBounds().top + valueText.getLocalBounds().height / 2);
    valueText.setPosition(cell.getPosition().x, cell.getPosition().y);
}

void HashCell::setVariable(std::string _variable) {
    variable = _variable;
    variableText.setString(variable);
    variableText.setOrigin(variableText.getLocalBounds().left + variableText.getLocalBounds().width / 2, variableText.getLocalBounds().top + variableText.getLocalBounds().height / 2);
    variableText.setPosition(cell.getPosition().x, cell.getPosition().y + radiusHash * 2);
}

void HashCell::setState(Hash::State _state) {
    state = _state;
}

void HashCell::setColorType(Hash::ColorType _type) {
    type = _type;
}

void HashCell::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Hash::Animation> animations) {
    if (totalTime < epsilonTime) {
        cell.setFillColor(Hash::color[theme][type].fillColor);
        cell.setOutlineColor(Hash::color[theme][type].outlineColor);
        valueText.setFillColor(Hash::color[theme][type].valueColor);
        variableText.setFillColor(Hash::color[theme][type].variableColor);
    }
    else {
        for (int i = 0; i < animations.size(); i++) {
            for (int j = i + 1; j < animations.size(); j++) {
                if (animations[i].animationType == animations[j].animationType) {
                    assert(false);
                }
            }
        }
        for (int i = 0; i < animations.size(); i++) {
            if (animations[i].animationType == Hash::colorTypeChanging) {
                Hash::Color newColor = Hash::fadingColorType(type, animations[i].colorType, theme, timePassed.asSeconds() / totalTime.asSeconds());
                cell.setFillColor(newColor.fillColor);
                cell.setOutlineColor(newColor.outlineColor);
                valueText.setFillColor(newColor.valueColor);
                variableText.setFillColor(newColor.variableColor);
            }
        }
    }
    window.draw(cell);
    if (state == Hash::full) {
        window.draw(valueText);
    }
    if (state == Hash::deleted) {
        mainDiagonal.setFillColor(Hash::color[theme][type].outlineColor);
        window.draw(mainDiagonal);
        antiDiagonal.setFillColor(Hash::color[theme][type].outlineColor);
        window.draw(antiDiagonal);
    }
    window.draw(variableText);
}

//-----------------------------------------------------------------------------------

HashTable::HashTable(int _size, sf::Font* font) :
    size(_size)
{
    cells.resize(size);
    int startX = (WIDTH_RES - (3 * radiusHash * (size - 1))) / 2;
    for (int i = 0; i < size; i++) {
        cells[i] = std::move(HashCell(startX + i * 3 * radiusHash, HEIGHT_RES / 3, font));
        cells[i].setVariable(intToString(i));
    }
}

int HashTable::countEmpty() {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (cells[i].state == Hash::empty) {
            count++;
        }
    }
    return count;
}

int HashTable::countDeleted() {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (cells[i].state == Hash::deleted) {
            count++;
        }
    }
    return count;
}

int HashTable::countFull() {
    int count = 0;
    for (int i = 0; i < size; i++) {
        if (cells[i].state == Hash::full) {
            count++;
        }
    }
    return count;
}

bool HashTable::insertV(int value) {
    if (countFull() == size) {
        return false;
    }
    std::cout << "inserting " << value << "\n";
    int index = value % size;
    int i = 0;
    while (cells[(index + i) % size].state == Hash::full) {
        std::cout << i << " ";
        i++;
    }
    std::cout << "\n" << (index + i) % size << "\n";
    cells[(index + i) % size].setValue(value);
    cells[(index + i) % size].setState(Hash::full);
    return true;
}

bool HashTable::deleteV(int value) {
    int index = value % size;
    int i = 0;
    while ((cells[(index + i) % size].state == Hash::deleted
           || (cells[(index + i) % size].state == Hash::full && cells[(index + i) % size].value != value))
           && i < size) {
        i++;
    }
    if (cells[(index + i) % size].state == Hash::empty || i == size) {
        return false;
    }
    cells[(index + i) % size].setState(Hash::deleted);
    cells[(index + i) % size].setValue(0);
    return true;
}

void HashTable::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed) {
    for (int i = 0; i < size; i++) {
        cells[i].draw(window, theme, totalTime, timePassed);
    }
}