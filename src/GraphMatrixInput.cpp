#include "GraphMatrixInput.h"

GraphMatrixInput::GraphMatrixInput(sf::Rect <float> rect, int size, sf::Font *font, bool isDirected) :
    rect(rect), size(size), font(font)
{
    setSize(size);
    setDirected(isDirected);
}

void GraphMatrixInput::setEdges(std::vector <GeneralEdge> edges, bool directed, int size) {
    setSize(size);
    setDirected(directed);
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j < i; j++) {
            boxes[i][j].setText("0");
        }
    }
    for (auto edge : edges) {
        int u = edge.from;
        int v = edge.to;
        int weight = edge.weight;
        boxes[u][v].setText(intToString(weight));
    }
}

void GraphMatrixInput::setDirected(bool isDirected) {
    directed = isDirected;
    float boxSize = rect.width / std::max(size + 1, 8);
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j < i; j++) {
            if (directed != boxes[i][j].isDrawable()) {
                float x = rect.left + j * boxSize;
                float y = rect.top + i * boxSize;
                float outline = (1 > boxSize ? 0 : outlineBox);
                boxes[i][j].setText("0");
                boxes[i][j].setDrawable(directed);
            }
        }
    }
}

int getRandomWeight(int bigValue, int smallValue, int chance) {
    int o = rand() % chance;
    if (!o) {
        return rand() % smallValue + 1;
    }
    else {
        return rand() % bigValue + 1;
    }
}

void GraphMatrixInput::createRandom() {
    if (size == 0) return;
    int graphType = rand() % 3;
    if (graphType == 0) {//densly connected
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                if (i == j) continue;
                if (!directed && i > j) continue;
                int isEdge = rand() % ((size + 2) / 2);
                if (isEdge) {
                    int weight = getRandomWeight(maxValueDataGraph, maxSizeDataGraph, std::max(size * size / 4, 2));
                    boxes[i][j].setText(intToString(weight));
                }
                else {
                    boxes[i][j].setText("0");
                }
            }
        }
    }
    if (graphType == 1) {//many components
        int numComp = rand() % size + 1;
        DSU F(size);
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                boxes[i][j].setText("0");
            }
        }
        std::vector <int> cha;
        for (int i = 1; i <= size; i++) {
            cha.push_back(i);
        }
        int cnt = size;
        while (cnt > numComp) {
            int pu = cha[rand() % cha.size()];
            int pv = cha[rand() % cha.size()];
            if (pu == pv) continue;
            std::vector <int> listU = F.getComponents(pu);
            std::vector <int> listV = F.getComponents(pv);
            int u = listU[rand() % listU.size()];
            int v = listV[rand() % listV.size()];
            if (F.join(u, v)) {
                int o = F.find(u) ^ pu ^ pv;
                for (int p = 0; p < cha.size(); p++) {
                    if (cha[p] == o) {
                        cha.erase(cha.begin() + p);
                        break;
                    }
                }
                cnt--;
                int weight = getRandomWeight(maxValueDataGraph, maxSizeDataGraph, std::max(size * size / 4, 2));
                boxes[u][v].setText(intToString(weight));
            }
        }
    }
    if (graphType == 2) {//a tree with some extra edges
        DSU F(size);
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                boxes[i][j].setText("0");
            }
        }
        int cnt = size - 1;
        while (cnt) {
            int u = rand() % size + 1;
            int v = rand() % size + 1;
            if (u == v) continue;
            if (F.join(u, v)) {
                cnt--;
                int weight = getRandomWeight(maxValueDataGraph, maxSizeDataGraph, std::max(size * size / 4, 2));
                if (u > v && !directed) std::swap(u, v);
                boxes[u][v].setText(intToString(weight));
            }
        }
        int maximumExtraEdge = std::min((size + 1), size * (size - 1) / 2 - size + 1);
        int numExtraEdge = rand() % (maximumExtraEdge + 1);
        while (numExtraEdge) {
            int u = rand() % size + 1;
            int v = rand() % size + 1;
            if (u == v) continue;
            if (u > v && !directed) std::swap(u, v);
            if (boxes[u][v].getText() == "0") {
                int weight = getRandomWeight(maxValueDataGraph, maxSizeDataGraph, std::max(size * size / 4, 2));
                boxes[u][v].setText(intToString(weight));
                numExtraEdge--;
            }
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

std::vector <GeneralEdge> GraphMatrixInput::getEdges() {
    if (directed) {
        std::vector <GeneralEdge> edges;
        for (int i = 1; i <= size; i++) {
            for (int j = 1; j <= size; j++) {
                if (i == j) continue;
                int weight = boxes[i][j].getProperInt();
                if (weight == -1) return {};
                if (weight > 0) {
                    edges.push_back(GeneralEdge(i, j, weight));
                }
            }
        }
        return edges;
    }
    else {
        std::vector <GeneralEdge> edges;
        for (int i = 1; i <= size; i++) {
            for (int j = i + 1; j <= size; j++) {
                int weight = boxes[i][j].getProperInt();
                if (weight == -1) return {};
                if (weight > 0) {
                    edges.push_back(GeneralEdge(i, j, weight));
                    //edges.push_back(GeneralEdge(j, i, weight));
                }
            }
        }
        return edges;
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

bool GraphMatrixInput::handleKeyPressed(int key) {
    bool flag = false;
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            if (boxes[i][j].isReading()) {
                boxes[i][j].readKey(key);
                flag = true;
            }
        }
    }
    return flag;
}

void GraphMatrixInput::update(sf::Time deltaT) {
    for (int i = 1; i <= size; i++) {
        for (int j = 1; j <= size; j++) {
            boxes[i][j].update(deltaT);
        }
    }
}

void GraphMatrixInput::draw(sf::RenderWindow& window, ColorTheme theme) {
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