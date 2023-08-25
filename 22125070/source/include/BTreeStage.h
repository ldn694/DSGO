#pragma once

#include "BTreeGraph.h"
#include "Stage.h"

struct BTreeStage : public Stage {
private:
    sf::Vector2f rootPosition;
	std::pair<bool, ColorTheme> processEvents();
	std::vector <BTreeGraph> BTreeList;
	void setDefaultView();
	void resetAnimation();
	void addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description);
	void update(sf::Time deltaT);
	void render();

	void insertValue(int value);
	void deleteValue(int value);
	void searchValue(int value);
public:
	BTreeStage(sf::RenderWindow& window, ColorTheme theme = LightTheme);
	ColorTheme run();
};