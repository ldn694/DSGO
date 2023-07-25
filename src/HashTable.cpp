#include "HashTable.h"

//-----------------------------------------------------------------------------------
//HashCell

//-----------------------------------------------------------------------------------

HashTable::HashTable(int _size, sf::Font* font) :
    size(_size)
{
    cells.resize(size);
    int startX = (WIDTH_RES - (3 * radiusHash * (size - 1))) / 2;
    for (int i = 0; i < size; i++) {
        cells[i] = std::move(HashCell(startX + i * 3 * radiusHash, HEIGHT_RES / 3, font));
        cells[i].insertVariable(intToString(i));
    }
}

HashTable HashTable::execAnimation(std::vector <Animation> animations) {
    HashTable tmp = *this;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < animations.size(); j++) {
            if (animations[j].id1 == i) {
                tmp.cells[i].setColorType(Hash::ColorType(animations[j].nextColorType));
            }
        }
    }
    return tmp;
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

void HashTable::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    for (int i = 0; i < size; i++) {
        std::vector <Animation> tmp;
        for (int j = 0; j < animations.size(); j++) {
            if (animations[j].id1 == i) {
                tmp.push_back(animations[j]);
            }
        }
        cells[i].draw(window, theme, totalTime, timePassed, tmp);
    }
}