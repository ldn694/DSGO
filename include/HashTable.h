#pragma once
#include <set>
#include <vector>
#include <string.h>
#include <cassert>
#include "HashCell.h"

struct HashTable {
    const int maxCellRow = 10;
    int size;
    std::vector <HashCell> cells;
    int countEmpty();
    int countDeleted();
    int countFull();

    HashTable(int size, sf::Font* font);
    //backend
    bool insertV(int value);
    bool deleteV(int value);
    HashTable execAnimation(std::vector <Animation> animations);
    //frontend
    void draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime = sf::seconds(0.f), sf::Time timePassed = sf::seconds(0.f), std::vector<Animation> animations = std::vector<Animation>());
};