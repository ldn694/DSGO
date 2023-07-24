#pragma once

#include "Template.h"
#include "MyShape.h"
#include "Box.h"

struct MultipleChoice {
private:
	float x, y, width, height, stepHeight, letterSize;
	std::vector <std::string> choices;
	std::vector <sf::CircleShape> circles;
	std::vector <Box> listBox;
	sf::Font* font;
	int curChoice;
	Box outerBox;
public:
	MultipleChoice(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, std::vector <std::string> choices = {}, sf::Font* font = nullptr, int curChoice = 0);
	int getChoice();
	void setChoice(int choice);
	bool handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};