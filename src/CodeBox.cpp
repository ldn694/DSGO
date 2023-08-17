#include "CodeBox.h"

CodeBox::CodeBox(float _x, float _y, float _width, float _height) :
    x(_x), y(_y), width(_width), height(_height), slideBar(x, y + height * 0.95, width, height * 0.05),
    leftRect(sf::Vector2f(_width, _height)), rightRect(sf::Vector2f(_width, _height)),
    smallLeftRect(sf::Vector2f(3, _height * 0.95)), smallRightRect(sf::Vector2f(3, _height * 0.95)),
    outerBox(_x, _y, _width, _height * 0.95, {CodeOuterBox}, "", nullptr, 0, false), highlightLine(-1), finalMaxWidth(0) {
    highlightLine = -1;
    leftRect.setPosition(x - width - 3, y);
    rightRect.setPosition(x + width + 3, y);
    smallLeftRect.setPosition(x - 3, y + height * 0.025);
    smallRightRect.setPosition(x + width, y + height * 0.025);
}

void CodeBox::setLines(std::vector <std::string> lines) {
    texts.clear();
    texts.resize(lines.size());
    for (int i = 0; i < texts.size(); i++) {
        texts[i].setString(lines[i]);
        texts[i].setFont(*font(fontType::Consolas));
    }
    float L = 0.f, R = 20.f, finalTotalHeight = 0.f;
    finalMaxWidth = 0.f;
    for (int i = 0; i < 60; i++) {
        float totalHeight = 0, maxWidth = 0;
        float mid = (L + R) / 2;
        for (int j = 0; j < lines.size(); j++) {
            texts[j].setCharacterSize(mid);
            totalHeight += texts[j].getLocalBounds().height * 1.5f;
            maxWidth = std::max(maxWidth, texts[j].getLocalBounds().width);
        }
        if (totalHeight > height * 0.9 || maxWidth > width * 1.9) {
            R = mid;
        } else {
            finalTotalHeight = totalHeight;
            finalMaxWidth = maxWidth;
            L = mid;
        }
    }
    for (int i = 0; i < lines.size(); i++) {
        texts[i].setCharacterSize(L);
    }
    float curY = y + (height * 0.9 - finalTotalHeight) / 2;
    for (int i = 0; i < texts.size(); i++) {
        texts[i].setPosition(x, curY + texts[i].getLocalBounds().height * 0.25f);
        curY += texts[i].getLocalBounds().height * 1.5f;
    }
    slideBar.setBarWidth(width / (finalMaxWidth * (2.f/1.9f)));
}

void CodeBox::setHighlightLine(int newHighlightLine) {
    highlightLine = newHighlightLine;
}

void CodeBox::handleMousePressed(float x, float y) {
    slideBar.handleMousePressed(x, y);
}

void CodeBox::handleMouseMoved(float x, float y, sf::RenderWindow& window) {
    slideBar.handleMouseMoved(x, y, window);
}

void CodeBox::handleMouseReleased(float x, float y) {
    slideBar.handleMouseReleased(x, y);
}

void CodeBox::draw(sf::RenderWindow& window, ColorTheme theme) {
    outerBox.draw(window, theme);
    if (highlightLine != -1) {
        sf::RectangleShape highlightRect;
        highlightRect.setPosition(x, texts[highlightLine].getGlobalBounds().top - texts[highlightLine].getLocalBounds().height * 0.25f);
        highlightRect.setSize(sf::Vector2f(width, texts[highlightLine].getLocalBounds().height * 1.5f));
        highlightRect.setFillColor(codeHighlightBackGroundColor[theme]);
        window.draw(highlightRect);
    }
    float curPercent = slideBar.getPercent();
    for (int i = 0; i < texts.size(); i++) {
        if (i == highlightLine) {
            texts[i].setFillColor(codeHighlightViewColor[theme]);
        }
        else {
            texts[i].setFillColor(codeNormalViewColor[theme]);
        }
        texts[i].setPosition(texts[i].getPosition().x - curPercent * std::max(finalMaxWidth - width, 0.f), texts[i].getPosition().y);
        window.draw(texts[i]);
        texts[i].setPosition(texts[i].getPosition().x + curPercent * std::max(finalMaxWidth - width, 0.f), texts[i].getPosition().y);
    }
    slideBar.draw(window, theme);
    leftRect.setFillColor(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
    rightRect.setFillColor(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
    smallLeftRect.setFillColor(toolBoxColor[theme]);
    smallRightRect.setFillColor(toolBoxColor[theme]);
    window.draw(leftRect);
    window.draw(rightRect);
    window.draw(smallLeftRect);
    window.draw(smallRightRect);
}