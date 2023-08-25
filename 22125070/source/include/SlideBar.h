#include "Template.h"
#include "Color.h"

struct SlideBar {
    sf::RectangleShape outerRect, bar;
    float x, y, width, height, barWidth;
    float curX, startX;
    bool isSliding;
    SlideBar(float x, float y, float width, float height);
    void setBarWidth(float percent);
    bool isMousePressed(float x, float y);
    void setBarPosition(float x, float y);
    float getPercent();
    void handleMousePressed(float x, float y);
    void handleMouseMoved(float x, float y, sf::RenderWindow& window);
    void handleMouseReleased(float x, float y);
    void draw(sf::RenderWindow& window, ColorTheme theme);
};