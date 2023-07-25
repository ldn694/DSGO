#pragma once
#include <SFML/Graphics.hpp>
#include "Template.h"
#include "Color.h"
#include "Box.h"

struct Switch {
private:
	float x, y, width, height, progress;
	Box outerBox;
	sf::RectangleShape bar, zipper;
	sf::Text nameText;
	sf::Time remainTime;
	bool state;
public:
	Switch(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, std::string name = "", sf::Font* font = nullptr, bool curState = 0);
	bool getState();
	void draw(sf::RenderWindow& window, ColorTheme theme);
	bool isInside(float x, float y);
	bool handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void update(sf::Time deltaT);
};