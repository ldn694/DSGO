#pragma once

#include "HashTable.h"
#include "Stage.h"

struct HashTableStage : public Stage {
private:
	int size;
	std::pair<bool, ColorTheme> processEvents();
	std::vector <HashTable> hashList;
	void resetAnimation();
	void addAnimationStep(std::vector <Animation> animations, sf::Time time);
	void update(sf::Time deltaT);
	void render();

	void insertValue(int value);
	void deleteValue(int value);
	void searchValue(int value);
public:
	HashTableStage(sf::RenderWindow& window, ColorTheme theme = LightTheme);
	ColorTheme run();
};