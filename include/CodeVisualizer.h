#include "Template.h"
#include "Color.h"
#include "CodeBox.h"

struct CodeVisualizer {
    CodeBox codeBox;
    std::string description;
    float xDescription = 0, yDescription = 0, widthDescription = 0, heightDescription = 0;
    CodeVisualizer(float x, float y, float width, float height, float xDescription, float yDescription, float widthDescription, float heightDescription);
    void handleMousePressed(float x, float y);
    void handleMouseMoved(float x, float y, sf::RenderWindow& window);
    void handleMouseReleased(float x, float y);
    void setLines(std::vector <std::string> lines);
    void setHighlightLine(int newHighlightLine);
    void setDescription(std::string newDescription);
    void draw(sf::RenderWindow& window, ColorTheme theme, bool displayDescription = true);
};