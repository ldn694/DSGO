#pragma once

#include "MyShape.h"

struct BackButton {
	MyShape arrow, square, innerSquare;
	BackButton(float x, float y, float width, float height);
	bool handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};