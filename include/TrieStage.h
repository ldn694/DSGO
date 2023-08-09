#pragma once

#include "TrieGraph.h"
#include "Stage.h"

struct TrieStage : public Stage {
private:
    sf::Vector2f rootPosition;
	std::pair<bool, ColorTheme> processEvents();
	std::vector <TrieGraph> TrieList;
	void setDefaultView();
	void resetAnimation();
	void addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description);
	void update(sf::Time deltaT);
	void render();

	void insertString(std::string str);
	void deleteString(std::string str);
	void searchString(std::string str);
public:
	TrieStage(sf::RenderWindow& window, ColorTheme theme = LightTheme);
	ColorTheme run();
};