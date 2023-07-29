#pragma once
#include "MyShape.h"

struct PlayButton {
	float* percent;
	AnimatingDirection* animatingDirection;
	MyShape circle, triangle, pause, replay;
	PlayButton(float x = 0.f, float y = 0.f, float radius = 0.f, float* percent = nullptr, AnimatingDirection* animatingDirection = nullptr);
	void handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void handleKeyPressed(int key);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};