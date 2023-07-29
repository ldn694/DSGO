#include <iostream>
#include "Scrubber.h"

Scrubber::Scrubber(float _x, float _y, float _width, float _height, float _zipWidth, float* _percent):
	x(_x), y(_y), width(_width), height(_height), zipWidth(_zipWidth), isChangingFrame(false), isHovering(false), percent(_percent) {}

bool Scrubber::getIsChangingFrame() {
	return isChangingFrame;
}

float Scrubber::getPercent(float mouseX, float mouseY) {
	float percent = (mouseX - x) / width;
	if (percent < 0) {
		percent = 0;
	}
	if (percent > 1) {
		percent = 1;
	}
	return percent;
}


void Scrubber::handleMousePressed(float mouseX, float mouseY) {
	if (mouseX < x || mouseX > x + width || mouseY < y || mouseY > y + height) {
		return;
	}
	isChangingFrame = true;
	*percent = getPercent(mouseX, mouseY);
}

void Scrubber::handleMouseMove(float mouseX, float mouseY, sf::RenderWindow& window) {
	if (mouseX < x || mouseX > x + width || mouseY < y || mouseY > y + height) {
		isHovering = false;
	}
	else {
		isHovering = true;
		window.setMouseCursor(handCursor);
	}
	if (isChangingFrame) {
		*percent = getPercent(mouseX, mouseY);
	}
}

void Scrubber::handleMouseReleased(float mouseX, float mouseY) {
	if (!isChangingFrame) {
		return;
	}
	isChangingFrame = false;
	*percent = getPercent(mouseX, mouseY);
}

void Scrubber::handleKeyPressed(int key) {
	
}

void Scrubber::draw(sf::RenderWindow& window, ColorTheme theme) {
	float curHeight = (isHovering || isChangingFrame) ? height : height * 0.7;
	sf::RectangleShape bar(sf::Vector2f(width, curHeight));
	bar.setOrigin(0, curHeight / 2);
	bar.setPosition(x, y + height / 2);
	bar.setFillColor(scrubberRemainColor[theme]);
	window.draw(bar);
	float passedWidth = width * (*percent);
	sf::RectangleShape passedBar(sf::Vector2f(passedWidth, curHeight));
	passedBar.setOrigin(0, curHeight / 2);
	passedBar.setPosition(x, y + height / 2);
	passedBar.setFillColor(scrubberPassedColor[theme]);
	window.draw(passedBar);
	if (isHovering || isChangingFrame) {
		sf::CircleShape zip(height * 0.8);
		zip.setOrigin(zip.getLocalBounds().left + zip.getLocalBounds().width / 2, zip.getLocalBounds().top + zip.getLocalBounds().height / 2);
		zip.setPosition(x + passedWidth, y + height / 2);
		zip.setFillColor(scrubberZipColor[theme]);
		window.draw(zip);
	}
}