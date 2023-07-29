#include "MediaControl.h"

MediaControl::MediaControl(float _x, float _y, float _width, float _height, float* _percent, AnimatingDirection* _animatingDirection) :
	x(_x), y(_y), width(_width), height(_height), animatingDirection(_animatingDirection), percent(_percent),
	scrubber(x, y + (height / 4 - height / 6) / 2, width, height / 6, zipWidth, _percent),
	playButton(x + width * 0.5, y + height * 0.75, height / 4, _percent, _animatingDirection),
	prevButton("Images/backward.png", x + width * 2 / 6, y + height * 0.75, height / 4, height / 4, playButtonCircleColor),
	nextButton("Images/forward.png", x + width * 4 / 6, y + height * 0.75, height / 4, height / 4, playButtonCircleColor),
	beginButton("Images/skip_begin.png", x + width * 1 / 6, y + height * 0.75, height / 4, height / 4, playButtonCircleColor),
	endButton("Images/skip_end.png", x + width * 5 / 6, y + height * 0.75, height / 4, height / 4, playButtonCircleColor)
{
	
}

bool MediaControl::getIsChangingFrame() {
	return scrubber.getIsChangingFrame();
}

void MediaControl::handleMousePressed(float x, float y) {
	scrubber.handleMousePressed(x, y);
	playButton.handleMousePressed(x, y);
	if (nextButton.isMousePressed(x, y)) {
		*animatingDirection = Forward;
	}
	if (prevButton.isMousePressed(x, y)) {
		*animatingDirection = Backward;
	}
	if (beginButton.isMousePressed(x, y)) {
		*animatingDirection = Home;
	}
	if (endButton.isMousePressed(x, y)) {
		*animatingDirection = End;
	}
}

void MediaControl::handleMouseMove(float x, float y, sf::RenderWindow& window) {
	scrubber.handleMouseMove(x, y, window);
	playButton.handleMouseMove(x, y, window);
	prevButton.handleMouseMove(x, y, window);
	nextButton.handleMouseMove(x, y, window);
	beginButton.handleMouseMove(x, y, window);
	endButton.handleMouseMove(x, y, window);
}

void MediaControl::handleMouseReleased(float x, float y) {
	scrubber.handleMouseReleased(x, y);
}

void MediaControl::handleKeyPressed(int key) {
	scrubber.handleKeyPressed(key);
	if (key == (int)sf::Keyboard::Right) { //next
		// if (ds->curFrame + 1 < ds->listFrame.size()) {
		// 	ds->setFrame(ds->curFrame + 1);
		// 	ds->setIsAnimating(false);
		// }
	}
	if (key == (int)sf::Keyboard::Left) { //prev
		// if (ds->curFrame - 1 >= 0) {
		// 	ds->setFrame(ds->curFrame - 1);
		// 	ds->setIsAnimating(false);
		// }
	}
	if (key == (int)sf::Keyboard::Home) {
		// ds->setFrame(0);
		// ds->setIsAnimating(false);
	}
	if (key == (int)sf::Keyboard::End) {
		// ds->setFrame(ds->listFrame.size() - 1);
		// ds->setIsAnimating(false);
	}
}

void MediaControl::draw(sf::RenderWindow& window, ColorTheme theme) {
	scrubber.draw(window, theme);
	playButton.draw(window, theme);
	nextButton.draw(window, theme);
	prevButton.draw(window, theme);
	beginButton.draw(window, theme);
	endButton.draw(window, theme);
}