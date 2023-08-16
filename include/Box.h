#pragma once
#include <SFML/Graphics.hpp>
#include "Template.h"
#include "Color.h"

const bool WITH_BORDER = true;
const bool NO_BORDER = false;

struct Box {
private:
	float x1, y1, width, height, outlineSize, textSize;
	std::string text;
	sf::Font* font;
	std::vector <ColorBoxType> colorModes;
	bool isBorder, isDrawable;
	int curMode;
	sf::RectangleShape outerRect;
	sf::Text Text;
public:
	Box(float x1 = 0.f, float y1 = 0.f, float width = 0.f, float height = 0.f,
		std::vector <ColorBoxType> colorModes = {},
		std::string text = "", sf::Font* font = nullptr, float textSize = 0.f,
		bool isBorder = true, float outlineSize = outlineBox);
	float getOutlineSize();
	void setPosition(float x1, float y1);
	void setSize(float width, float height);
	void setText(std::string newText);
	void setTextSize(float newSize);
	std::string getText();
	int getWidth();
	int getHeight();
	sf::Font* getFont();
	void draw(sf::RenderWindow& window, ColorTheme theme);
	bool isInside(float x, float y);
	void toggleColorMode();
	void setDrawable(bool drawable);
	bool handleMouseMove(float x, float y, sf::RenderWindow& window);
	void setColorMode(int mode);
};