#pragma once

#include "HashTable.h"
#include "Stage.h"

struct HashTableStage : public Stage {
private:
	HashTable hash;
	std::pair<bool, ColorTheme> processEvents();
	void update(sf::Time deltaT);
	void render();
public:
	HashTableStage(sf::RenderWindow& window, ColorTheme theme = LightTheme);
	ColorTheme run();
};