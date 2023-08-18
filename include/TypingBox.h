#pragma once

#include <SFML/Graphics.hpp>
#include "Template.h"
#include "Color.h"

struct TypingBox {
protected:
	bool drawable;
	float x, y, width, height;
	float xWarning, yWarning, widthWarning, heightWarning;
	bool displayingWarning;
	bool centerAligned;
	TypingBoxMode typingMode;
	bool reading;
	std::string text, warning; 
	sf::Font* font;
	sf::Time time;
	int maxCharacter, minValue, maxValue;
	bool displayingLine;
public:
	TypingBox(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, 
		float xWarning = 0.f, float yWarning = 0.f, float widthWarning = 0.f, float heightWarning = 0.f,
		TypingBoxMode typingMode = singleNumber, sf::Font *font = nullptr, int maxCharacter = 5, int minValue = 0, int maxValue = 10, bool centerAligned = false);
	bool isDrawable();
	void setDrawable(bool drawable);
	bool isReading();
	std::string getText();
	void setWarning();
	void setMinValue(int newMinValue);
	void setMaxValue(int newMaxValue);
	void setText(std::string newText);
	void insert(int key);
	void deleteBack();
	void readKey(int key);
	int getInt();
	int getProperInt();
	std::vector <int>  getListInt();
	void update(sf::Time deltaT);
	void clickOn(float x, float y);
	bool handleMouseMove(float x, float y, sf::RenderWindow& window);
	void draw(sf::RenderWindow& window, ColorTheme theme);
};

struct BigTypingBox : public TypingBox{
private:
	float outerX, outerY, outerWidth, outerHeight, valueWidth, outlineSize;
	std::string name;
	bool drawable, typingBoxDrawable;
public:
	BigTypingBox(float x = 0.f, float y = 0.f, float width = 0.f, float height = 0.f, float valueWidth = 0.f, float outlineSize = 0.f, std::string name = "",
		float xWarning = 0.f, float yWarning = 0.f, float widthWarning = 0.f, float heightWarning = 0.f,
		TypingBoxMode typingMode = singleNumber, sf::Font* font = nullptr, int maxCharacter = 5, int minValue = 0, int maxValue = 10,
		bool drawable = true, bool typingBoxDrawable = true);
	void setDrawable(bool val);
	void setTypingBoxDrawable(bool val);
	void setName(std::string newName);
	void drawAll(sf::RenderWindow& window, ColorTheme theme);
};