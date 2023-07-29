#pragma once
#include <SFML/Graphics.hpp>
#include "Scrubber.h"
#include "PlayButton.h"

struct MediaControl {
	float x, y, width, height;
	PlayButton playButton;
	MyShape nextButton, prevButton, beginButton, endButton;
	Scrubber scrubber;
	AnimatingDirection* animatingDirection;
	float* percent;
	MediaControl(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, float* percent = nullptr, AnimatingDirection* animatingDirection = nullptr);
	bool getIsChangingFrame();
	float getPercent();
	void handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void handleMouseReleased(float x, float y);
	void handleKeyPressed(int key);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};