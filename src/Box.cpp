#include "Box.h"

Box::Box(float _x1, float _y1, float _width, float _height,
	std::vector <ColorBoxType> _colorModes,
	std::string _text, sf::Font* _font, float _textSize,
	bool _isBorder, float _outlineSize) :
	x1(_x1), y1(_y1), width(_width), height(_height),
	text(_text), font(_font), textSize(_textSize),
	colorModes(_colorModes),
	isBorder(_isBorder), outlineSize(_outlineSize)
{
	float tmpOutlineSize = (isBorder ? outlineSize : 0.f);
	outerRect = sf::RectangleShape(sf::Vector2f(width - tmpOutlineSize, height - tmpOutlineSize));
	outerRect.setPosition(x1 + tmpOutlineSize / 2.0f, y1 + tmpOutlineSize / 2.0f);
	curMode = 0;

	Text.setFont(*font);
	Text.setString(text);
	Text.setCharacterSize(textSize);
	Text.setStyle(sf::Text::Bold);
	sf::FloatRect textRect = Text.getLocalBounds();
	Text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	Text.setPosition(x1 + width / 2.0f, y1 + height / 2.0f);

	isDrawable = true;
}

float Box::getOutlineSize() {
	return outlineSize;
}

void Box::setTextSize(float newSize) {
	textSize = newSize;
	Text.setCharacterSize(newSize);
}

void Box::setText(std::string newText) {
	text = newText;
	Text.setString(newText);
}

sf::Font* Box::getFont() {
	return font;
}

int Box::getWidth() {
	return width;
}
int Box::getHeight() {
	return height;
}

void Box::setPosition(float _x1, float _y1) {
	x1 = _x1;
	y1 = _y1;
	float tmpOutlineSize = (isBorder ? outlineSize : 0.f);
	outerRect.setPosition(x1 + tmpOutlineSize / 2.0f, y1 + tmpOutlineSize / 2.0f);
	sf::FloatRect textRect = Text.getLocalBounds();
	Text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	Text.setPosition(x1 + width / 2.0f, y1 + height / 2.0f);
}

void Box::setSize(float newWidth, float newHeight) {
	width = newWidth;
	height = newHeight;
	float tmpOutlineSize = (isBorder ? outlineSize : 0.f);
	outerRect.setSize(sf::Vector2f(width - tmpOutlineSize, height - tmpOutlineSize));
	sf::FloatRect textRect = Text.getLocalBounds();
	Text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	Text.setPosition(x1 + width / 2.0f, y1 + height / 2.0f);
}

void Box::setDrawable(bool drawable) {
	isDrawable = drawable;
}

bool Box::handleMouseMove(float x, float y, sf::RenderWindow& window) {
	if (colorModes.size() != 2) {
		return false;
	}
	if (isInside(x, y)) {
		curMode = 1;
		window.setMouseCursor(handCursor);
		return true;
	}
	else {
		curMode = 0;
	}
	return false;
}

void Box::draw(sf::RenderWindow& window, ColorTheme theme) {
	if (!isDrawable) {
		return;
	}
	outerRect.setFillColor(colorBox[colorModes[curMode]][theme].fillColor);
	outerRect.setOutlineColor(colorBox[colorModes[curMode]][theme].outlineColor);
	outerRect.setOutlineThickness(outlineSize);
	window.draw(outerRect);
	Text.setFillColor(colorBox[colorModes[curMode]][theme].textColor);
	window.draw(Text);
}

bool Box::isInside(float x, float y) {
	return (x1 < x && x < x1 + width && y1 < y && y < y1 + height);
}

void Box::toggleColorMode() {
	curMode = (curMode + 1) % colorModes.size();
}

void Box::setColorMode(int mode) {
	curMode = mode;
}