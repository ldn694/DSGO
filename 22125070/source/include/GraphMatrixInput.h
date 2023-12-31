#include "Template.h"
#include "Color.h"
#include "Box.h"
#include "TypingBox.h"
#include "GeneralEdge.h"
#include "DSU.h"

struct GraphMatrixInput {
    bool directed;
    sf::Rect <float> rect;
    sf::Font* font;
    int size;
    std::vector <Box> firstRow, firstColumn;
    std::vector <std::vector <TypingBox>> boxes;
    GraphMatrixInput(sf::Rect <float> rect, int size, sf::Font *font, bool isDirected = true);
    void setEdges(std::vector <GeneralEdge> edges, bool isDirected, int size);
    void createRandom();
    void setSize(int size);
    void setDirected(bool isDirected);
    void handleMouseMoved(float mouseX, float mouseY, sf::RenderWindow& window);
    void handleMousePressed(float mouseX, float mouseY);
    bool handleKeyPressed(int key);
    void update(sf::Time deltaT);
    std::vector <GeneralEdge> getEdges();
    void draw(sf::RenderWindow& window, ColorTheme theme);
};