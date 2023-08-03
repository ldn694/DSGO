#pragma once

#include "AVLGraph.h"
#include "Stage.h"

struct AVLStage : public Stage {
private:
    sf::Vector2f rootPosition;
	std::pair<bool, ColorTheme> processEvents();
	std::vector <AVLGraph> AVLList;
	void setDefaultView();
	void leftRotate(std::vector <Animation>& animations, int id);
	void rightRotate(std::vector <Animation>& animations, int id);
	void resetAnimation();
	void addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description);
	void update(sf::Time deltaT);
	void render();

	void insertValue(int value);
	void deleteValue(int value);
	void searchValue(int value);
public:
	AVLStage(sf::RenderWindow& window, ColorTheme theme = LightTheme);
	ColorTheme run();
};