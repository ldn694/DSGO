#include "Template.h"
#include "Color.h"
#include "Box.h"
#include "TypingBox.h"

struct GraphMatrixInput {
    bool directed;
    sf::Rect <float> rect;
    sf::Font* font;
    int size;
    std::vector <Box> firstRow, firstColumn;
    std::vector <std::vector <TypingBox>> boxes;
    GraphMatrixInput(sf::Rect <float> rect, int size, sf::Font *font, bool isDirected = true);
    void setSize(int size);
    void setDirected(bool isDirected);
    void handleMouseMoved(float mouseX, float mouseY, sf::RenderWindow& window);
    void handleMousePressed(float mouseX, float mouseY);
    void handleKeyPressed(int key);
    void update(sf::Time deltaT);
    void draw(sf::RenderWindow& window, ColorTheme theme);
};