#include <iostream>
#include "TypingBox.h"
#include "Box.h"

TypingBox::TypingBox(float _x, float _y, float _width, float _height, 
	float _xWarning, float _yWarning, float _widthWarning, float _heightWarning,
	TypingBoxMode _typingMode, sf::Font* _font, int _maxCharacter, int _minValue, int _maxValue) :
	x(_x), y(_y), width(_width), height(_height), 
	xWarning(_xWarning), yWarning(_yWarning), widthWarning(_widthWarning), heightWarning(_heightWarning),
	typingMode(_typingMode), font(_font), maxCharacter(_maxCharacter), minValue(_minValue), maxValue(_maxValue)
{
	reading = false;
	displayingLine = true;
	displayingWarning = false;
	if (typingMode == singleNumber) {
		text = intToString(getInt());
	}
}

bool TypingBox::isReading() {
	return reading;
}

void TypingBox::setWarning() {
	if (typingMode == singleNumber) {
		int curVal = getInt();
		if (curVal < minValue || curVal > maxValue) {
			displayingWarning = true;
			warning = "Value must be in [";
			warning = warning + intToString(minValue) + "," + intToString(maxValue) + "]!";
		}
		else {
			displayingWarning = false;
		}
	}
	if (typingMode == multipleNumber) {
		std::vector <int> tmp = getListInt();
		if (!tmp.empty() && tmp[0] == -1) {
			displayingWarning = true;
			warning = "Illegar character found, character must be space or in [";
			warning = warning + intToString(minValue) + "," + intToString(maxValue) + "]!";
			return;
		}
		if (!tmp.empty() && (tmp[0] == -2 || tmp[0] == -3)) {
			displayingWarning = true;
			warning = "Value must be in [";
			warning = warning + intToString(minValue) + "," + intToString(maxValue) + "]!";
			return;
		}
		displayingWarning = false;
	}
}

std::string TypingBox::getText() {
	if (typingMode == singleNumber) {
		return (text.empty() ? "0" : text);
	}
	return text;
}

void TypingBox::setText(std::string newText) {
	if (typingMode == singleNumber) {
		text.clear();
		for (char x : newText) {
			if ('0' <= x && x <= '9') {
				text.push_back(x);
			}
		}
	}
	else {
		text = newText;
	}
}

int TypingBox::getInt() {
	int res = 0;
	for (char x : text) {
		res = res * 10 + x - '0';
	}
	return res;
}

std::vector <int> TypingBox::getListInt() {
	std::vector <int> tmp;
	int i = 0;
	std::string valText;
	std::string curText = text + ",";
	while (i < curText.size()) {
		if (curText[i] == ',') {
			if (!valText.empty()) {
				tmp.push_back(stringToInt(valText));
				valText.clear();
			}
			i++;
			continue;
		}
		if (curText[i] < '0' || curText[i] > '9') {
			return { -1 };
		}
		valText.push_back(curText[i]);
		if (valText.size() > 9) {
			return { -3 };
		}
		if (stringToInt(valText) < minValue || stringToInt(valText) > maxValue) {
			return { -2 };
		}
		i++;
	}
	return tmp;
}

int TypingBox::getProperInt() {
	int val = getInt();
	if (val < minValue || val > maxValue) {
		return -1;
	}
	return val;
}

void TypingBox::insert(int key) {
	if ((key < 0 || key > 35) && key != int(sf::Keyboard::Comma)) return;
	if (text.size() + 1 > maxCharacter) {
		return;
	}
	if (typingMode == singleNumber) {
		if (key < 26 || key == int(sf::Keyboard::Comma)) return;
	}
	if (typingMode == multipleNumber) {
		if (key < 26 && key != int(sf::Keyboard::Comma)) return;
	}
	char x;
	if (key < 26) {
		x = char(key + 'a'); 
	} else if (key <= 35) {
		x = char(key - 26 + '0');
	} else if (key == int(sf::Keyboard::Comma)) {
		x = ',';
	}
	text.push_back(x);
	sf::Text Text;
	Text.setFont(*font);
	Text.setString(text);
	Text.setCharacterSize(height * 0.6);
	sf::FloatRect textRect = Text.getLocalBounds();
	if (textRect.width > width - 15) {
		text.pop_back();
	}
	setWarning();
}

void TypingBox::deleteBack() {
	if (!text.empty()) {
		text.pop_back();
	}
	setWarning();
}

void TypingBox::update(sf::Time deltaT) {
	if (time + deltaT >= flickeringTime) {
		displayingLine ^= 1;
		time = time + deltaT - flickeringTime;
	}
	else {
		time += deltaT;
	}
}

void TypingBox::clickOn(float hereX, float hereY) {
	if (hereX < x || hereX > x + width || hereY < y || hereY > y + height) {
		reading = false;
		if (typingMode == singleNumber) {
			text = intToString(getInt());
		}
		return;
	}
	if (!reading) {
		reading = true;
		time = sf::seconds(0.f);
		displayingLine = true;
	}
}

bool TypingBox::handleMouseMove(float hereX, float hereY, sf::RenderWindow& window) {
	if (x <= hereX && hereX <= x + width && y <= hereY && hereY <= y + height) {
		window.setMouseCursor(textCursor);
		return true;
	}
	return false;
}

void TypingBox::readKey(int key) {
	if ((0 <= key && key <= 35) || key == int(sf::Keyboard::Comma)) {
		insert(key);
	}
	if (key == int(sf::Keyboard::BackSpace)) {
		deleteBack();
	}
	if (key == int(sf::Keyboard::Enter)) {
		reading = false;
		if (typingMode == singleNumber) {
			text = intToString(getInt());
		}
	}
}

void TypingBox::draw(sf::RenderWindow& window, ColorTheme theme) {
	sf::RectangleShape rect(sf::Vector2f(width, height));
	rect.setPosition(x, y);
	rect.setFillColor(colorBox[ColorBoxType::Typing_Box][theme].fillColor);
	rect.setOutlineThickness(0.f);
	window.draw(rect);
	sf::Text Text;
	Text.setFont(*font);
	Text.setString(text);
	//std::cout << text << "\n";
	Text.setCharacterSize(height * 0.6);
	Text.setFillColor(colorBox[ColorBoxType::Typing_Box][theme].textColor);
	Text.setPosition(x + 10, y + height * 0.1);
	sf::FloatRect textRect = Text.getLocalBounds();
	window.draw(Text);
	if (reading && displayingLine) {
		sf::Vertex vtx[4]; 
		float textHeight = height * 0.6;
		float lineX = textRect.width < epsilonFloat ? x + 10 : x + textRect.width + 15;
		vtx[0] = sf::Vertex(sf::Vector2f(lineX, y + (height - textHeight) * 0.5f), colorBox[Typing_Box][theme].textColor);
		vtx[1] = sf::Vertex(sf::Vector2f(lineX, y + (height - textHeight) * 0.5f + textHeight), colorBox[Typing_Box][theme].textColor);
		window.draw(vtx, 2, sf::Lines);
	}
	if (displayingWarning) {
		sf::RectangleShape tmpRect(sf::Vector2f(widthWarning, heightWarning));
		tmpRect.setPosition(xWarning, yWarning);
		tmpRect.setFillColor(warningFillColor[theme]);
		window.draw(tmpRect);
		sf::Text WarningText = CompressWords(warning,
			xWarning, yWarning, widthWarning, heightWarning,
			font, 25, warningTextColor[theme]);
		window.draw(WarningText);
	}
}

BigTypingBox::BigTypingBox(float _x, float _y, float _width, float _height, float _valueWidth, float _outlineSize, std::string _name, 
	float xWarning, float yWarning, float widthWarning, float heightWarning,
	TypingBoxMode _typingMode, sf::Font* _font, int _maxCharacter, int _minValue, int _maxValue, bool _drawable, bool _typingBoxDrawable) :
	outerX(_x), outerY(_y), outerWidth(_width), outerHeight(_height), valueWidth(_valueWidth), outlineSize(_outlineSize), name(_name),
	TypingBox(_x + _valueWidth + _outlineSize, _y + _height * 0.2, _width - _valueWidth - 3 * _outlineSize, _height * 0.6,
		xWarning, yWarning, widthWarning, heightWarning, _typingMode, _font, _maxCharacter, _minValue, _maxValue),
	drawable(_drawable), typingBoxDrawable(_typingBoxDrawable)
{}

void BigTypingBox::setDrawable(bool val) {
	drawable = val;
}

void BigTypingBox::setTypingBoxDrawable(bool val) {
	typingBoxDrawable = val;
}

void BigTypingBox::setName(std::string newName) {
	name = newName;
}

void BigTypingBox::drawAll(sf::RenderWindow& window, ColorTheme theme) {
	if (drawable) {
		Box tmpBox(outerX, outerY, outerWidth, outerHeight, { ColorBoxType::CommandBoxNormal }, "", font, 0, WITH_BORDER, outlineSize);
		tmpBox.draw(window, theme);
		sf::Text Text;
		Text.setFont(*font);
		Text.setString(name);
		Text.setCharacterSize(height * 0.6);
		Text.setFillColor(colorBox[ColorBoxType::CommandBoxNormal][theme].textColor);
		sf::FloatRect textRect = Text.getLocalBounds();
		Text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		Text.setPosition(outerX + (valueWidth + outlineSize) / 2, outerY + outerHeight / 2);
		window.draw(Text);
		if (typingBoxDrawable) {
			draw(window, theme);
		}
	}
}