#pragma once

#include "Template.h"
#include "Color.h"

struct TriangleButton {
	float x, y, radius, outlineSize, rotation;
	ColorBoxType type;
	sf::CircleShape shape;
	TriangleButton(float x = 0.f, float y = 0.f, float radius = 0.f, float outlineSize = 0.f, float rotation = 0.f);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, ColorTheme theme);
	bool isInside(float x, float y);
};