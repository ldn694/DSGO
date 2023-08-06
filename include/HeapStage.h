#pragma once

#include "HeapGraph.h"
#include "Stage.h"
#include <functional>

struct HeapStage : public Stage {
private:
    std::function <bool(int, int)> compare;
	std::string comparisonStr, negativeComparisonStr;
    sf::Vector2f rootPosition;
	bool isMinHeap;
	std::pair<bool, ColorTheme> processEvents();
	std::vector <HeapGraph> HeapList;
	void setDefaultView();
	void resetAnimation();
	void addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description);
	void update(sf::Time deltaT);
	void render();

	void insertValue(int value);
	void deleteValue(int value);
	void getTop();
public:
	HeapStage(sf::RenderWindow& window, ColorTheme theme = LightTheme, bool isMinHeap = true);
	ColorTheme run();
};