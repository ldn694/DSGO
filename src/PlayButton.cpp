#include "PlayButton.h"
#include "Template.h"

PlayButton::PlayButton(float x, float y, float radius, float* _percent, AnimatingDirection* _animatingDirection) :
	circle("Images/Circle.png", x, y, radius * 2, radius * 2, playButtonCircleColor),
	triangle("Images/Triangle.png", x, y, radius * 1, radius * 1, playButtonTriangleColor),
	pause("Images/Pause.png", x, y, radius, radius, playButtonTriangleColor),
	replay("Images/Replay.png", x, y, radius * 1.2, radius * 1.2, playButtonTriangleColor),
	percent(_percent), animatingDirection(_animatingDirection)
{
	triangle.setOrigin(triangle.sprite.getLocalBounds().width / 3, triangle.sprite.getLocalBounds().height / 2);
	replay.setOrigin(replay.sprite.getLocalBounds().width * 0.5 * 60 / 52, replay.sprite.getLocalBounds().height * 0.5 * 52 / 60);
}

void PlayButton::handleMousePressed(float mouseX, float mouseY) {
	if (circle.isMousePressed(mouseX, mouseY)) {
		if (*animatingDirection == Pause) {
			*animatingDirection = Continuous;
		}
		else {
			*animatingDirection = Pause;
		}
	}
}

void PlayButton::handleMouseMove(float mouseX, float mouseY, sf::RenderWindow& window) {
	if (circle.isMousePressed(mouseX, mouseY)) {
		window.setMouseCursor(handCursor);
	}
}

void PlayButton::handleKeyPressed(int key) {
	if (key == sf::Keyboard::Space) {
		if (*animatingDirection == Pause) {
			*animatingDirection = Continuous;
		}
		else {
			*animatingDirection = Pause;
		}
	}
}

void PlayButton::draw(sf::RenderWindow& window, ColorTheme theme) {
	circle.draw(window, theme);
	if (*animatingDirection == Pause) {
		triangle.draw(window, theme);
	}
	else {
		pause.draw(window, theme);
	}
}