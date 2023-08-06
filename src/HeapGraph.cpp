#include "HeapGraph.h"

HeapGraph::HeapGraph(sf::Vector2f startPosition, sf::Font* font) : startPosition(startPosition), font(font) {
    nodes = std::map <int, AVLNode>();
    root = -1;
}

int HeapGraph::getMexID() {
    std::set <int> idSet;
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        idSet.insert(x->first);
    }
    int i = 0;
    while (true) {
        if (idSet.find(i) == idSet.end()) {
            return i;
        }
        i++;
    }
    assert(false);
}

int HeapGraph::getRealID(int hID) {
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        if (heapID.find(x->first) != heapID.end() && heapID[x->first] == hID) {
            return x->first;
        }
    }
    return -1;
}

int HeapGraph::getRealParent(int id) {
    if (heapID.find(id) == heapID.end()) {
        return -1;
    }
    int hID = heapID[id];
    if (hID == 1) {
        return -1;
    }
    return getRealID(hID / 2);
}

int HeapGraph::getHeapID(int id) {
    if (heapID.find(id) == heapID.end()) {
        return -1;
    }
    return heapID[id];
}

int HeapGraph::getHeapParent(int id) {
    if (heapID.find(id) == heapID.end()) {
        return -1;
    }
    int hID = heapID[id];
    if (hID == 1) {
        return -1;
    }
    return hID / 2;
}

int HeapGraph::getHeapHeigth(int id) {
    int h = 1;
    while (id > 1) {
        id /= 2;
        h++;
    }
    return h;
}

sf::Vector2f HeapGraph::getPosition(int id) {
    if (id == 1) {
        return startPosition;
    }
    if (id % 2 == 0) {
        return getPosition(id / 2) + sf::Vector2f(-minHorizontalDistHeap / 2 * (1 << (maxHeightHeap - getHeapHeigth(id))), verticalDistHeap);
    }
    else {
        return getPosition(id / 2) + sf::Vector2f(minHorizontalDistHeap / 2 * (1 << (maxHeightHeap - getHeapHeigth(id))), verticalDistHeap);
    }
}

sf::RectangleShape HeapGraph::getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent) {
    sf::Vector2f diff = endPosition - startPosition;
    if (length(diff) < 2 * (radiusHeap + thicknessHeap)) {
        return sf::RectangleShape();
    }
    sf::RectangleShape line(sf::Vector2f((length(diff) - 2 * (radiusHeap + thicknessHeap)) * percent, thicknessHeap));
    line.setOrigin(0, thicknessHeap / 2);
    line.setPosition(startPosition + normalize(diff) * (radiusHeap + thicknessHeap));
    line.setRotation(atan2(diff.y, diff.x) * 180 / PI);
    return line;
}

void HeapGraph::arrange(int id, sf::Vector2f position, int depth) {
    nodes[getRealID(id)].setPosition(position);
    if (depth <= maxHeightHeap) {
        sf::Vector2f leftPosition;
        sf::Vector2f rightPosition;
        if (depth < maxHeightHeap) {
            leftPosition = position + sf::Vector2f(-minHorizontalDistHeap / 2 * (1 << (maxHeightHeap - depth - 1)), verticalDistHeap);
            rightPosition = position + sf::Vector2f(minHorizontalDistHeap / 2 * (1 << (maxHeightHeap - depth - 1)), verticalDistHeap);
        }
        else {
            leftPosition = position + sf::Vector2f(-minHorizontalDistHeap / 2, verticalDistHeap);
            rightPosition = position + sf::Vector2f(minHorizontalDistHeap / 2, verticalDistHeap);
        }
        int idLeft = id * 2;
        if (idLeft <= nodes.size()) {
            arrange(idLeft, leftPosition, depth + 1);
        }
        int idRight = id * 2 + 1;
        if (idRight <= nodes.size()) {
            arrange(idRight, rightPosition, depth + 1);
        } 
    }
}

void HeapGraph::arrangeHeapTrees() {
    if (root != -1) {
        if (nodes.find(root) != nodes.end()) {
            arrange(1, startPosition, 1);
        }
    }
}

HeapGraph HeapGraph::execAnimation(std::vector <Animation> animations) {
    HeapGraph tmp = *this;
    std::sort(animations.begin(), animations.end());
    // std::cout << "------\n";
    // std::cout << "size = " << animations.size() << "\n";
    for (int i = 0; i < animations.size(); i++) {
        //  std::cout << "before " << i << ":" << animations[i].animationType << " " << animations[i].id1 << " " << animations[i].id2 << "\n";
        switch (animations[i].animationType) {
            case SetValue: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].setValue(animations[i].nextValue);
                break;
            }
            case InsertVariable: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].insertVariable(animations[i].variableList);
                break;
            }
            case DeleteVariable: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].deleteVariable(animations[i].variableList);
                break;
            }
            case SetColorType: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].setColorType(AVL::ColorType(animations[i].nextValue));
                break;
            }
            case SwapNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end() || tmp.nodes.find(animations[i].id2) == tmp.nodes.end()) {
                    assert(false);
                }
                sf::Vector2f pos1 = tmp.nodes[animations[i].id1].getPosition();
                sf::Vector2f pos2 = tmp.nodes[animations[i].id2].getPosition();
                tmp.nodes[animations[i].id1].setPosition(pos2);
                tmp.nodes[animations[i].id2].setPosition(pos1);
                int backupID = tmp.heapID[animations[i].id1];
                tmp.heapID[animations[i].id1] = tmp.heapID[animations[i].id2];
                tmp.heapID[animations[i].id2] = backupID;
                tmp.arrangeHeapTrees();
                break;
            }
            case AddNode: {
                tmp.nodes[animations[i].id1] = AVLNode({0, 0}, animations[i].nextValue, font);
                tmp.heapID[animations[i].id1] = tmp.nodes.size();
                tmp.arrangeHeapTrees();
                tmp.nodes[animations[i].id1].setSize(1.f);
                break;
            }
            case DeleteNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes.erase(animations[i].id1);
                tmp.heapID.erase(animations[i].id1);
                tmp.arrangeHeapTrees();
                break;
            }
            case SetLeftNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].leftNode = animations[i].nextValue;
                tmp.arrangeHeapTrees();
                break;
            }
            case SetRightNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].rightNode = animations[i].nextValue;
                tmp.arrangeHeapTrees();
                break;
            }
            case SetLeftEdgeColorType: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].leftEdgeType = AVL::ColorType(animations[i].nextValue);
                tmp.arrangeHeapTrees();
                break;
            }
            case SetRightEdgeColorType: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].rightEdgeType = AVL::ColorType(animations[i].nextValue);
                tmp.arrangeHeapTrees();
                break;
            }
            case SetRoot: {
                tmp.root = animations[i].nextValue;
                tmp.arrangeHeapTrees();
                break;
            }
        }
        // std::cout << "after " << i << ":" << animations[i].animationType << " " << animations[i].id1 << " " << animations[i].id2 << "\n";
    }
    return tmp;
}

void HeapGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            x->second.draw(window, theme);
            int idLeft = heapID[x->first] * 2;
            if (idLeft <= nodes.size() && nodes.find(idLeft) != nodes.end()) {
                sf::RectangleShape line = getEdgeLine(x->second.getPosition(), nodes[getRealID(idLeft)].getPosition(), 1.f);
                if (line.getSize() != sf::Vector2f()) {
                    line.setFillColor(Heap::color[theme][x->second.leftEdgeType].outlineColor);
                    window.draw(line);
                }
            }
            int idRight = heapID[x->first] * 2 + 1;
            if (idRight <= nodes.size() && nodes.find(idRight) != nodes.end()) {
                sf::RectangleShape line = getEdgeLine(x->second.getPosition(), nodes[getRealID(idRight)].getPosition(), 1.f);
                if (line.getSize() != sf::Vector2f()) {
                    line.setFillColor(Heap::color[theme][x->second.rightEdgeType].outlineColor);
                    window.draw(line);
                }
            }
        }
    }
    float percent = (totalTime < epsilonTime ? 1.f : timePassed / totalTime);
    HeapGraph tmp = execAnimation(animations);
    std::map <int, std::vector <Animation> > animationMap;
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        animationMap[x->first] = std::vector <Animation>();
    }
    for (auto x = tmp.nodes.begin(); x != tmp.nodes.end(); x++) {
        animationMap[x->first] = std::vector <Animation>();
    }
    //Nodes
    //Move nodes
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        if (tmp.nodes.find(x->first) != tmp.nodes.end()) {
            if (x->second.getPosition() != tmp.nodes[x->first].getPosition()) {
                Animation animation;
                animation.animationType = Move;
                animation.id1 = x->first;
                animation.nextPosition = tmp.nodes[x->first].getPosition();
                animationMap[x->first].push_back(animation);
            }
        }
    }
    for (int i = 0; i < animations.size(); i++) {
        if (animations[i].id1 != -1) {
            animationMap[animations[i].id1].push_back(animations[i]);
        }
    }
    //Edges
    for (int i = 1; i <= nodes.size(); i++) {
        int idLeft = i * 2;
        if (idLeft <= nodes.size() && idLeft > tmp.nodes.size()) {//left edge disappear
            sf::RectangleShape line = getEdgeLine(getPosition(i), getPosition(idLeft), 1 - percent);
            if (line.getSize() != sf::Vector2f()) {
                line.setFillColor(Heap::color[theme][nodes[getRealID(i)].leftEdgeType].outlineColor);
                window.draw(line);
            }
        }
        if (idLeft > nodes.size() && idLeft <= tmp.nodes.size()) {//left edge appear
            sf::RectangleShape line = getEdgeLine(getPosition(i), getPosition(idLeft), percent);
            if (line.getSize() != sf::Vector2f()) {
                line.setFillColor(Heap::color[theme][tmp.nodes[tmp.getRealID(i)].leftEdgeType].outlineColor);
                window.draw(line);
            }
        }
        if (idLeft <= nodes.size() && idLeft <= tmp.nodes.size()) {//same left edge
            sf::RectangleShape line = getEdgeLine(getPosition(i), getPosition(idLeft), 1.f);
            if (line.getSize() != sf::Vector2f()) {
                line.setFillColor(Heap::color[theme][tmp.nodes[tmp.getRealID(i)].leftEdgeType].outlineColor);
                window.draw(line);
            }
        }
        int idRight = i * 2 + 1;
        if (idRight <= nodes.size() && idRight > tmp.nodes.size()) {//right edge disappear
            sf::RectangleShape line = getEdgeLine(getPosition(i), getPosition(idRight), 1 - percent);
            if (line.getSize() != sf::Vector2f()) {
                line.setFillColor(Heap::color[theme][nodes[getRealID(i)].rightEdgeType].outlineColor);
                window.draw(line);
            }
        }
        if (idRight > nodes.size() && idRight <= tmp.nodes.size()) {//right edge appear
            sf::RectangleShape line = getEdgeLine(getPosition(i), getPosition(idRight), percent);
            if (line.getSize() != sf::Vector2f()) {
                line.setFillColor(Heap::color[theme][tmp.nodes[tmp.getRealID(i)].rightEdgeType].outlineColor);
                window.draw(line);
            }
        }
        if (idRight <= nodes.size() && idRight <= tmp.nodes.size()) {//same right edge
            sf::RectangleShape line = getEdgeLine(getPosition(i), getPosition(idRight), 1.f);
            if (line.getSize() != sf::Vector2f()) {
                line.setFillColor(Heap::color[theme][tmp.nodes[tmp.getRealID(i)].rightEdgeType].outlineColor);
                window.draw(line);
            }
        }
    }
    //Draw nodes
    //system("cls");
    // std::cout << "---\n";
     for (auto x = animationMap.begin(); x != animationMap.end(); x++) {
        int id = x->first;
        if (nodes.find(id) == nodes.end()) { //New node
            // std::cout << "Drawing new node " << id << "\n";
            tmp.nodes[id].draw(window, theme, totalTime, timePassed, x->second);
        }
        else { //Old node
            // std::cout << "Drawing old node " << id << "\n";
            nodes[id].draw(window, theme, totalTime, timePassed, x->second);
        }
    }
}