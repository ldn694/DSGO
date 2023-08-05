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

int HeapGraph::getParent(int id) {
    if (id == 1) {
        return -1;
    }
    return id / 2;
}

int HeapGraph::getHeigth(int id) {
    int h = 1;
    while (id > 1) {
        id /= 2;
        h++;
    }
    return h;
}

sf::RectangleShape HeapGraph::getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent) {
    sf::Vector2f diff = endPosition - startPosition;
    if (length(diff) < 2 * (radiusAVL + thicknessAVL)) {
        return sf::RectangleShape();
    }
    sf::RectangleShape line(sf::Vector2f((length(diff) - 2 * (radiusAVL + thicknessAVL)) * percent, thicknessAVL));
    line.setOrigin(0, thicknessAVL / 2);
    line.setPosition(startPosition + normalize(diff) * (radiusAVL + thicknessAVL));
    line.setRotation(atan2(diff.y, diff.x) * 180 / PI);
    return line;
}

void HeapGraph::arrange(int id, sf::Vector2f position, int depth) {
    nodes[id].setPosition(position);
    if (depth <= maxHeightAVL) {
        sf::Vector2f leftPosition;
        sf::Vector2f rightPosition;
        if (depth < maxHeightAVL) {
            leftPosition = position + sf::Vector2f(-minHorizontalDistAVL / 2 * (1 << (maxHeightAVL - depth - 1)), verticalDistAVL);
            rightPosition = position + sf::Vector2f(minHorizontalDistAVL / 2 * (1 << (maxHeightAVL - depth - 1)), verticalDistAVL);
        }
        else {
            leftPosition = position + sf::Vector2f(-minHorizontalDistAVL / 2, verticalDistAVL);
            rightPosition = position + sf::Vector2f(minHorizontalDistAVL / 2, verticalDistAVL);
        }
        if (nodes[id].leftNode != -1 && nodes.find(nodes[id].leftNode) != nodes.end()) {
            arrange(nodes[id].leftNode, leftPosition, depth + 1);
        }
        if (nodes[id].rightNode != -1 && nodes.find(nodes[id].rightNode) != nodes.end()) {
            arrange(nodes[id].rightNode, rightPosition, depth + 1);
        } 
    }
}

void HeapGraph::arrangeAVLTrees() {
    if (root != -1) {
        if (nodes.find(root) != nodes.end()) {
            arrange(root, startPosition, 1);
        }
    }
}

HeapGraph HeapGraph::execAnimation(std::vector <Animation> animations) {
    HeapGraph tmp = *this;
    std::sort(animations.begin(), animations.end());
    //std::cout << "------\n";
    for (int i = 0; i < animations.size(); i++) {
        //std::cout << "before " << i << ":" << animations[i].animationType << " " << animations[i].id1 << "\n";
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
            case AddNode: {
                tmp.nodes[animations[i].id1] = AVLNode({0, 0}, animations[i].nextValue, font);
                tmp.arrangeAVLTrees();
                tmp.nodes[animations[i].id1].setSize(1.f);
                break;
            }
            case DeleteNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes.erase(animations[i].id1);
                //std::cout << "HAHA! " << tmp.nodes.size() << "\n";
                tmp.arrangeAVLTrees();
                break;
            }
            case SetLeftNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].leftNode = animations[i].nextValue;
                tmp.arrangeAVLTrees();
                break;
            }
            case SetRightNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].rightNode = animations[i].nextValue;
                tmp.arrangeAVLTrees();
                break;
            }
            case SetLeftEdgeColorType: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].leftEdgeType = AVL::ColorType(animations[i].nextValue);
                tmp.arrangeAVLTrees();
                break;
            }
            case SetRightEdgeColorType: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].rightEdgeType = AVL::ColorType(animations[i].nextValue);
                tmp.arrangeAVLTrees();
                break;
            }
            case SetRoot: {
                tmp.root = animations[i].nextValue;
                tmp.arrangeAVLTrees();
                break;
            }
        }
        //std::cout << "after " << i << ":" << animations[i].animationType << " " << tmp.nodes.size() << "\n";
    }
    return tmp;
}

void HeapGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            x->second.draw(window, theme);
            if (x->second.leftNode != -1) {
                sf::RectangleShape line = getEdgeLine(x->second.getPosition(), nodes[x->second.leftNode].getPosition(), 1.f);
                if (line.getSize() != sf::Vector2f()) {
                    line.setFillColor(AVL::color[theme][x->second.leftEdgeType].outlineColor);
                    window.draw(line);
                }
            }
            if (x->second.rightNode != -1) {
                sf::RectangleShape line = getEdgeLine(x->second.getPosition(), nodes[x->second.rightNode].getPosition(), 1.f);
                if (line.getSize() != sf::Vector2f()) {
                    line.setFillColor(AVL::color[theme][x->second.rightEdgeType].outlineColor);
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
    // std::cout << animationMap.size() << " " << nodes.size() << " " << tmp.nodes.size() << "\n";
    //Edges
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        int idU = x->first;
        sf::Vector2f startPositionU = x->second.getPosition();
        sf::Vector2f goalPositionU;
        if (tmp.nodes.find(idU) == tmp.nodes.end()) {
            goalPositionU = x->second.getPosition();
        }
        else {
            goalPositionU = tmp.nodes[idU].getPosition();
        }
        if (nodes[idU].leftNode != -1) {//old left edge disappear
            int idV = nodes[idU].leftNode;
            sf::Vector2f startPositionV = nodes[idV].getPosition();
            sf::Vector2f goalPositionV = tmp.nodes.find(idV) == tmp.nodes.end() ? nodes[idV].getPosition() : tmp.nodes[idV].getPosition();
            if (tmp.nodes.find(idU) == tmp.nodes.end() || tmp.nodes[idU].leftNode != nodes[idU].leftNode) {
                sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
                sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
                sf::RectangleShape line = getEdgeLine(uPosition, vPosition, 1 - percent);
                if (tmp.nodes.find(idU) == tmp.nodes.end()) {
                    line.setFillColor(AVL::color[theme][nodes[idU].leftEdgeType].outlineColor);
                }
                else {
                    line.setFillColor(AVL::color[theme][tmp.nodes[idU].leftEdgeType].outlineColor);
                }
                window.draw(line);
            }
        }
        if (nodes[idU].rightNode != -1) {//old right edge disappear
            int idV = nodes[idU].rightNode;
            sf::Vector2f startPositionV = nodes[idV].getPosition();
            sf::Vector2f goalPositionV = tmp.nodes.find(idV) == tmp.nodes.end() ? nodes[idV].getPosition() : tmp.nodes[idV].getPosition();
            if (tmp.nodes.find(idU) == tmp.nodes.end() || tmp.nodes[idU].rightNode != nodes[idU].rightNode) {
                sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
                sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
                sf::RectangleShape line = getEdgeLine(uPosition, vPosition, 1 - percent);
                if (tmp.nodes.find(idU) == tmp.nodes.end()) {
                    line.setFillColor(AVL::color[theme][nodes[idU].rightEdgeType].outlineColor);
                }
                else {
                    line.setFillColor(AVL::color[theme][tmp.nodes[idU].rightEdgeType].outlineColor);
                }
                window.draw(line);
            }
        }
        if (tmp.nodes.find(idU) != tmp.nodes.end() && tmp.nodes[idU].leftNode != nodes[idU].leftNode && tmp.nodes[idU].leftNode != -1) {//new left edge appear
            int idV = tmp.nodes[idU].leftNode;
            sf::Vector2f startPositionV = nodes.find(idV) == nodes.end() ? tmp.nodes[idV].getPosition() : nodes[idV].getPosition();
            sf::Vector2f goalPositionV = tmp.nodes[idV].getPosition();
            sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
            sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
            sf::RectangleShape line = getEdgeLine(uPosition, vPosition, percent);
            line.setFillColor(AVL::color[theme][tmp.nodes[idU].leftEdgeType].outlineColor);
            window.draw(line);
        }
        if (tmp.nodes.find(idU) != tmp.nodes.end() && tmp.nodes[idU].rightNode != nodes[idU].rightNode && tmp.nodes[idU].rightNode != -1) {//new right edge appear
            int idV = tmp.nodes[idU].rightNode;
            sf::Vector2f startPositionV = nodes.find(idV) == nodes.end() ? tmp.nodes[idV].getPosition() : nodes[idV].getPosition();
            sf::Vector2f goalPositionV = tmp.nodes[idV].getPosition();
            sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
            sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
            sf::RectangleShape line = getEdgeLine(uPosition, vPosition, percent);
            line.setFillColor(AVL::color[theme][tmp.nodes[idU].rightEdgeType].outlineColor);
            window.draw(line);
        }
        if (tmp.nodes.find(idU) != tmp.nodes.end() && tmp.nodes[idU].leftNode == nodes[idU].leftNode && nodes[idU].leftNode != -1) {//same left edge
            int idV = nodes[idU].leftNode;
            sf::Vector2f startPositionV = nodes[idV].getPosition();
            sf::Vector2f goalPositionV = tmp.nodes[idV].getPosition();
            sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
            sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
            sf::RectangleShape line = getEdgeLine(uPosition, vPosition, 1);
            line.setFillColor(AVL::color[theme][tmp.nodes[idU].leftEdgeType].outlineColor);
            window.draw(line);
        }
        if (tmp.nodes.find(idU) != tmp.nodes.end() && tmp.nodes[idU].rightNode == nodes[idU].rightNode && nodes[idU].rightNode != -1) {//same right edge
            int idV = nodes[idU].rightNode;
            sf::Vector2f startPositionV = nodes[idV].getPosition();
            sf::Vector2f goalPositionV = tmp.nodes[idV].getPosition();
            sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
            sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
            sf::RectangleShape line = getEdgeLine(uPosition, vPosition, 1);
            line.setFillColor(AVL::color[theme][tmp.nodes[idU].rightEdgeType].outlineColor);
            window.draw(line);
        }
    }
    //Draw nodes
     for (auto x = animationMap.begin(); x != animationMap.end(); x++) {
        int id = x->first;
        if (nodes.find(id) == nodes.end()) { //New node
            tmp.nodes[id].draw(window, theme, totalTime, timePassed, x->second);
        }
        else { //Old node
            nodes[id].draw(window, theme, totalTime, timePassed, x->second);
        }
    }
}