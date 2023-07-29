#include "SlideBar.h"
#include "Box.h"

struct CodeBox {
    float x, y, width, height, finalMaxWidth;
    std::vector <std::string> lines;
    std::vector <sf::Text> texts;
    Box outerBox;
    sf::RectangleShape leftRect, rightRect;
    SlideBar slideBar;
    int highlightLine;
    CodeBox(float x, float y, float width, float height);
    void setLines(std::vector <std::string> lines);
    void setHighlightLine(int newHighlightLine);
    void handleMousePressed(float x, float y);
    void handleMouseMoved(float x, float y, sf::RenderWindow& window);
    void handleMouseReleased(float x, float y);
    void draw(sf::RenderWindow& window, ColorTheme theme);
};