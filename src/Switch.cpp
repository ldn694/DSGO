#include "Switch.h"

Switch::Switch(float _x, float _y, float _width, float _height, std::string name, sf::Font* font, bool curState):
	x(_x), y(_y), width(_width), height(_height), state(curState)
{
	progress = 1;
	outerBox = std::move(Box(x, y, width, height, {CommandBoxNormal}));
	bar = std::move(sf::RectangleShape(sf::Vector2f(0.18 * width, 0.4 * height)));
	bar.setOrigin(bar.getLocalBounds().left + bar.getLocalBounds().width, bar.getLocalBounds().top + bar.getLocalBounds().height / 2);
	bar.setPosition(x + width * 0.95, y + height * 0.5f);
	zipper = std::move(sf::RectangleShape(sf::Vector2f(0.05 * width, 0.3 * height)));
	zipper.setOrigin(zipper.getLocalBounds().left + zipper.getLocalBounds().width / 2, zipper.getLocalBounds().top + zipper.getLocalBounds().height / 2);
	if (state) {
		zipper.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width * 0.75, bar.getGlobalBounds().top + bar.getGlobalBounds().height * 0.5);
	}
	else {
		zipper.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width * 0.25, bar.getGlobalBounds().top + bar.getGlobalBounds().height * 0.5);
	}
	float l = 0.f, r = 100.f;
	nameText.setFont(*font);
	nameText.setString(name);
	for (int cnt = 0; cnt < 60; cnt++) {
		float mid = (l + r) / 2;
		nameText.setCharacterSize(mid);
		if (nameText.getLocalBounds().width <= width * 0.6 && nameText.getLocalBounds().height <= height / 2) {
			l = mid;
		}
		else {
			r = mid;
		}
	}
	nameText.setOrigin(nameText.getLocalBounds().left + nameText.getLocalBounds().width / 2, nameText.getLocalBounds().top + nameText.getLocalBounds().height / 2);
	nameText.setPosition(x + (width - bar.getGlobalBounds().width) / 2, y + height / 2);
}

bool Switch::getState() {
	return state;
}

bool Switch::isInside(float mouseX, float mouseY) {
	sf::FloatRect rect = bar.getGlobalBounds();
	return (rect.left < mouseX && mouseX < rect.left + rect.width && rect.top < mouseY && mouseY < rect.top + rect.height);
}

bool Switch::handleMousePressed(float mouseX, float mouseY) {
	if (!isInside(mouseX, mouseY)) {
		return false;
	}
	state ^= 1;
	progress = 0;
	remainTime = switchTime;
	return true;
}

void Switch::handleMouseMove(float mouseX, float mouseY, sf::RenderWindow& window) {
	if (!isInside(mouseX, mouseY)) {
		return;
	}
	window.setMouseCursor(handCursor);
}

void Switch::update(sf::Time deltaT) {
	sf::Time passedTime = min(deltaT, remainTime);
	if (passedTime < epsilonTime) {
		return;
	}
	float dProgress = passedTime / remainTime * (1 - progress);
	progress += dProgress;
	remainTime -= passedTime;
	if (!state) { //off 
		zipper.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width * (0.25 + 0.5 * (1 - progress)), bar.getGlobalBounds().top + bar.getGlobalBounds().height * 0.5);
	}
	else { //on
		zipper.setPosition(bar.getGlobalBounds().left + bar.getGlobalBounds().width * (0.75 - 0.5 * (1 - progress)), bar.getGlobalBounds().top + bar.getGlobalBounds().height * 0.5);
	}
}

void Switch::draw(sf::RenderWindow& window, ColorTheme theme) {
	outerBox.draw(window, theme);
	if (state) {
		zipper.setFillColor(switchZipperOnColor[theme]);
		bar.setFillColor(switchOnColor[theme]);
	}
	else {
		zipper.setFillColor(switchZipperOffColor[theme]);
		bar.setFillColor(switchOffColor[theme]);
	}
	window.draw(bar);
	window.draw(zipper);
	nameText.setFillColor(switchTextColor[theme]);
	window.draw(nameText);
}