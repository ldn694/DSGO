#include "GraphMatrixInput.h"

GraphMatrixInput::GraphMatrixInput(sf::Rect <float> rect, int size, sf::Font *font, bool isDirected) :
    rect(rect), size(size), font(font)
{
    setSize(size);
    setDirected(isDirected);
}

void GraphMatrixInput::setDirected(bool isDirected) {
    directed = isDirected;
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= i - 1; j++) {
            boxes[i][j].setDrawable(directed);
        }
    }
}

void GraphMatrixInput::setSize(int newSize) {
    size = newSize;
    firstRow = std::vector <Box> (size + 1, Box());
    firstColumn = std::vector <Box> (size + 1, Box());
    boxes = std::vector <std::vector <TypingBox>> (size + 1, std::vector <TypingBox> (size + 1, TypingBox()));
    float boxSize = rect.width / std::max(size + 1, 8);
    firstRow[0] = Box(rect.left, rect.top, boxSize, boxSize, {MatrixHeaderBox}, "", font, boxSize * 0.6f);
    for (int i = 1; i <= size; i++) {
        float y = rect.top + i * boxSize;
        for (int j = 1; j <= size; j++) {
            float x = rect.left + j * boxSize;
            float outline = (1 > boxSize ? 0 : outlineBox);
            boxes[i][j] = TypingBox(x + outline / 2, y + outline / 2, boxSize - outline, boxSize - outline, 0, HEIGHT_RES - heightBox, widthBox * 2, heightBox, singleNumber, font, 2, 0, maxValueDataGraph, true);
            if (i == j) boxes[i][j].setDrawable(false);
        }
    }
    for (int i = 1; i <= size; i++) {
        float x = rect.left + i * boxSize;
        float y = rect.top;
        float outline = (1 > boxSize ? 0 : outlineBox);
        firstRow[i] = Box(x, y, boxSize, boxSize, {MatrixHeaderBox}, intToString(i), font, boxSize * 0.6f, true, std::min(1.f, boxSize));
    }

    for (int i = 1; i <= size; i++) {
        float x = rect.left;
        float y = rect.top + i * boxSize;
        float outline = (1 > boxSize ? 0 : outlineBox);
        firstColumn[i] = Box(x, y, boxSize, boxSize, {MatrixHeaderBox}, intToString(i), font, boxSize * 0.6f, true, std::min(1.f, boxSize));
    }
}

void GraphMatrixInput::handleMouseMoved(float mouseX, float mouseY, sf::RenderWindow& window) {
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            boxes[i][j].handleMouseMove(mouseX, mouseY, window);
        }
    }
}

void GraphMatrixInput::handleMousePressed(float mouseX, float mouseY) {
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            boxes[i][j].clickOn(mouseX, mouseY);
        }
    }
}

void GraphMatrixInput::handleKeyPressed(int key) {
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            if (boxes[i][j].isReading()) {
                boxes[i][j].readKey(key);
            }
        }
    }
}

void GraphMatrixInput::update(sf::Time deltaT) {
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            boxes[i][j].update(deltaT);
        }
    }
}

void GraphMatrixInput::draw(sf::RenderWindow& window, ColorTheme theme) {
    //std::cout << "HAHA\n";
    if (size > 0) {
        firstRow[0].draw(window, theme);
    }
    for (int i = 1; i <= size; i++) {
        firstRow[i].draw(window, theme);
        firstColumn[i].draw(window, theme);
    }
    if (size > 0) {
        boxes[0][0].draw(window, theme);
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                boxes[i][j].draw(window, theme);
            }
        }
    }
}