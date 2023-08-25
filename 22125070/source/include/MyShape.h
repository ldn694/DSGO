#pragma once
#include <SFML/Graphics.hpp>
#include "Template.h"
#include "Color.h"

struct MyShape {
	sf::Texture texture;
	sf::Sprite sprite;
	float x, y, width, height;
	const sf::Color* color;
	MyShape(std::string filename, float x, float y, float width, float height, const sf::Color* color);
	bool isMousePressed(float x, float y);
	bool handleMouseMove(float x, float y, sf::RenderWindow& window);
	void setOrigin(float x, float y);
	void setPosition(float x, float y);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};