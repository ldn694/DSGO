#include "SlideBar.h"

SlideBar::SlideBar(float x, float y, float width, float height):
    x(x), y(y), width(width), height(height), isSliding(false) {
    setBarWidth(1);
    outerRect.setPosition(x + outlineBox, y + outlineBox);
    outerRect.setSize(sf::Vector2f(width - 2 * outlineBox, height - 2 * outlineBox));
    outerRect.setOutlineThickness(outlineBox);
}

void SlideBar::setBarWidth(float percent) {
    percent = std::min(1.f, std::max(0.f, percent));
    barWidth = width * percent;
    bar.setSize(sf::Vector2f(barWidth - 2 * outlineBox, height - outlineBox * 2));
    bar.setOrigin(bar.getLocalBounds().left + bar.getLocalBounds().width / 2, bar.getLocalBounds().top + bar.getLocalBounds().height / 2);
    bar.setPosition(x + barWidth / 2, y + height / 2);
}

bool SlideBar::isMousePressed(float mouseX, float mouseY) {
    return outerRect.getGlobalBounds().contains(mouseX, mouseY);
}

void SlideBar::setBarPosition(float mouseX, float mouseY) {
    if (mouseX < x + barWidth / 2) {
        bar.setPosition(x + barWidth / 2, y + height / 2);
        return;
    }
    if (mouseX > x + width - barWidth / 2) {
        bar.setPosition(x + width - barWidth / 2, y + height / 2);
        return;
    }
    bar.setPosition(mouseX, y + height / 2);
}

float SlideBar::getPercent() {
    return (bar.getPosition().x - x - barWidth / 2) / (width - barWidth);
}

void SlideBar::handleMousePressed(float mouseX, float mouseY) {
    if (!isMousePressed(mouseX, mouseY)) {
        return;
    }
    setBarPosition(mouseX, mouseY);
    isSliding = true;
}

void SlideBar::handleMouseMoved(float mouseX, float mouseY, sf::RenderWindow &window) {
    if (!isSliding) {
        return;
    }
    setBarPosition(mouseX, mouseY);
}

void SlideBar::handleMouseReleased(float mouseX, float mouseY) {
    if (!isSliding) {
        return;
    }
    setBarPosition(mouseX, mouseY);
    isSliding = false;
}

void SlideBar::draw(sf::RenderWindow& window, ColorTheme theme) {
    outerRect.setFillColor(slideBarBackgroundColor[theme]);
    outerRect.setOutlineColor(slideBarOutlineColor[theme]);
    bar.setFillColor(slideBarColor[theme]);
    window.draw(outerRect);
    window.draw(bar);
}