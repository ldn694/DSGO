#pragma once
#include <iostream>
#include "Template.h"
#include "Color.h"


struct ReadFromFile {
private:
	float x, y, width, height, xWarning, yWarning, widthWarning, heightWarning;
	sf::Font* font;
	std::string fileName, address, warning;
	bool isDisplaying, displayingWarning;
	int minValue, maxValue, maxSize;

public:
	ReadFromFile(float x, float y, float width, float height,
		float xWarning, float yWarning, float widthWarning, float heightWarning,
		sf::Font* font, int maxSize, int minValue, int maxValue);
	void handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y, sf::RenderWindow& window);
	std::vector <int> getListInt();
	void draw(sf::RenderWindow& window, ColorTheme theme);
	void setDisplaying(bool val);
	bool getDisplaying();
	void setWarning();
	void reset();
};