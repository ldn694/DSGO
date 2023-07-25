#pragma once

#include "MediaControl.h"
#include "MultipleChoice.h"
#include "Switch.h"

struct IngameSettings {
private: 
	float x, y, width, height;
	MediaControl mediaControl;
	MultipleChoice speedChoices;
	Switch descriptionSwitch, skipAnimationSwitch;
	float speed, percent;	
	bool isDescription, skipAnimation;
public:
	IngameSettings(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, ColorTheme theme = LightTheme, AnimatingDirection* animatingDirection = nullptr);
	float getPercent();
	float getSpeed();
	bool getIsDescription();
	bool getSkipAnimation();
	void handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	void handleMouseReleased(float x, float y);
	void handleKeyPressed(int key);
	void update(sf::Time deltaT);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};