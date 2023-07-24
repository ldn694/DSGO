#include <iostream>
#include <algorithm>
#include "MultipleChoice.h"
#include "Box.h"
MultipleChoice::MultipleChoice(float _x, float _y, float _width, float _height, std::vector <std::string> _choices, sf::Font* _font, int _curChoice) :
	x(_x), y(_y), width(_width), height(_height), choices(_choices), curChoice(_curChoice), font(_font), outerBox(Box(_x, _y, _width, _height, { MediaBox }))
{
	stepHeight = height / choices.size();
	float l = 0.f, r = 100.f;
	for (int cnt = 0; cnt < 60; cnt++) {
		float mid = (l + r) / 2;
		float maxWidth = 0, maxHeight = 0;
		sf::Text text;
		text.setFont(*font);
		text.setCharacterSize(mid);
		for (int i = 0; i < choices.size(); i++)
		{
			text.setString(choices[i]);
			maxWidth = (maxWidth > text.getLocalBounds().width ? maxWidth : text.getLocalBounds().width);
			maxHeight = (maxHeight > text.getLocalBounds().height ? maxHeight : text.getLocalBounds().height);
		}
		if (maxWidth <= width * 0.6 && maxHeight <= stepHeight / 2) {
			l = mid;
		}
		else {
			r = mid;
		}
	}
	letterSize = l;
	circles.resize(choices.size());
	for (int i = 0; i < choices.size(); i++) {
		circles[i].setPointCount(pointCountCircle);
		circles[i].setRadius(std::min(stepHeight * 0.35, width * 0.05));
		circles[i].setOrigin(circles[i].getLocalBounds().width / 2, circles[i].getLocalBounds().height / 2);
		circles[i].setPosition(x + width * 0.85, y + stepHeight * (i + 0.5));
	}
	listBox.resize(choices.size());
	for (int i = 0; i < choices.size(); i++) {
		listBox[i] = Box(x, y + stepHeight * i, width, stepHeight, { MediaBox });
	}
}

bool MultipleChoice::handleMousePressed(float mouseX, float mouseY) {
	for (int i = 0; i < choices.size(); i++) {
		float dist = sqrt((mouseX - circles[i].getPosition().x) * (mouseX - circles[i].getPosition().x) + (mouseY - circles[i].getPosition().y) * (mouseY - circles[i].getPosition().y));
		if (dist <= circles[i].getRadius()) {
			curChoice = i;
			return true;
		}
	}
	return false;
}

void MultipleChoice::handleMouseMove(float mouseX, float mouseY, sf::RenderWindow& window) {
	for (int i = 0; i < choices.size(); i++) {
		float dist = sqrt((mouseX - circles[i].getPosition().x) * (mouseX - circles[i].getPosition().x) + (mouseY - circles[i].getPosition().y) * (mouseY - circles[i].getPosition().y));
		if (dist <= circles[i].getRadius()) {
			window.setMouseCursor(handCursor);
		}
	}
}

int MultipleChoice::getChoice() {
	return curChoice;
}

void MultipleChoice::setChoice(int choice) {
	curChoice = choice;
}

void MultipleChoice::draw(sf::RenderWindow& window, ColorTheme theme) {
	outerBox.draw(window, theme);
	for (int i = 0; i < choices.size(); i++) {
		listBox[i].draw(window, theme);
		sf::Text text;
		text.setString(choices[i]);
		text.setFont(*font);
		text.setCharacterSize(letterSize);
		text.setFillColor(choicesColor[theme]);
		text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2.0f, text.getLocalBounds().top + text.getLocalBounds().height / 2.0f);
		text.setPosition(x + width * 0.4, y + stepHeight * (i + 0.5));
		window.draw(text);
		circles[i].setFillColor(choicesOuterCircleColor[theme]);
		window.draw(circles[i]);
	}
	sf::CircleShape curCircle;
	curCircle.setPointCount(pointCountCircle);
	curCircle.setRadius(std::min(stepHeight * 0.35, width * 0.05) * 0.65);
	curCircle.setOrigin(curCircle.getLocalBounds().width / 2, curCircle.getLocalBounds().height / 2);
	curCircle.setFillColor(choicesInnerCircleColor[theme]);
	curCircle.setPosition(circles[curChoice].getPosition());
	window.draw(curCircle);

}