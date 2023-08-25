#pragma once

#include "Template.h"
#include "Color.h"

struct Scrubber {
private: 
	float x, y, width, height, zipWidth;;
	bool isChangingFrame, preIsAnimating, isHovering;
	float* percent;
public:
	Scrubber(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, float zipWidth = 0.f, float* percent = nullptr);
	bool getIsChangingFrame();
	float getPercent(float mouseX, float mouseY);
	void handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void handleMouseReleased(float x, float y);
	void handleKeyPressed(int key);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};