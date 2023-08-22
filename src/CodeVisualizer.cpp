#include "CodeVisualizer.h"

CodeVisualizer::CodeVisualizer(float x, float y, float width, float height, float xDescription, float yDescription, float widthDescription, float heightDescription) :
    codeBox(x, y, width, height), xDescription(xDescription), yDescription(yDescription), widthDescription(widthDescription), heightDescription(heightDescription) {
}

void CodeVisualizer::handleMousePressed(float x, float y) {
    codeBox.handleMousePressed(x, y);
}

void CodeVisualizer::handleMouseMoved(float x, float y, sf::RenderWindow& window) {
    codeBox.handleMouseMoved(x, y, window);
}

void CodeVisualizer::handleMouseReleased(float x, float y) {
    codeBox.handleMouseReleased(x, y);
}

void CodeVisualizer::setLines(std::vector <std::string> lines) {
    codeBox.setLines(lines);
    codeBox.setHighlightLine(-1);
    setDescription("");
}

void CodeVisualizer::setHighlightLine(int newHighlightLine) {
    codeBox.setHighlightLine(newHighlightLine);
}

void CodeVisualizer::setDescription(std::string newDescription) {
    description = newDescription;
}

void CodeVisualizer::draw(sf::RenderWindow& window, ColorTheme theme, bool displayDescription) {
    codeBox.draw(window, theme);
    if (displayDescription) {
		sf::RectangleShape tmpRect(sf::Vector2f(widthDescription, heightDescription));
		tmpRect.setPosition(xDescription, yDescription);
		tmpRect.setFillColor(descriptionFillColor[theme]);
		window.draw(tmpRect);
		sf::Text descriptionText = CompressWords(description,
			xDescription, yDescription, widthDescription, heightDescription,
			font(fontType::Consolas), sizeLetterDescription, descriptionTextColor[theme]);
		window.draw(descriptionText);
    }
}