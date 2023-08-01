#include "AVLGraph.h"

AVLGraph::AVLGraph(sf::Vector2f startPosition, sf::Font* font) : startPosition(startPosition), font(font) {
    nodes = std::map <int, AVLNode>();
    root = -1;
}

int AVLGraph::getMexID() {
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

sf::RectangleShape AVLGraph::getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent) {
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

void AVLGraph::arrange(int id, sf::Vector2f position, int depth) {
    nodes[id].setPosition(position);
    if (depth < maxHeightAVL) {
        sf::Vector2f leftPosition = position + sf::Vector2f(-minHorizontalDistAVL / 2 * (1 << (maxHeightAVL - depth - 1)), verticalDistAVL);
        sf::Vector2f rightPosition = position + sf::Vector2f(minHorizontalDistAVL / 2 * (1 << (maxHeightAVL - depth - 1)), verticalDistAVL);
        if (nodes[id].leftNode != -1) {
            arrange(nodes[id].leftNode, leftPosition, depth + 1);
        }
        if (nodes[id].rightNode != -1) {
            arrange(nodes[id].rightNode, rightPosition, depth + 1);
        }
    }    
}

void AVLGraph::arrangeAVLTrees() {
    if (root != -1) {
        arrange(root, startPosition, 1);
    }
}

AVLGraph AVLGraph::execAnimation(std::vector <Animation> animations) {
    AVLGraph tmp = *this;
    std::sort(animations.begin(), animations.end());
    for (int i = 0; i < animations.size(); i++) {
        switch (animations[i].animationType) {
            case InsertVariable: {
                tmp.nodes[animations[i].id1].insertVariable(animations[i].variableList);
                break;
            }
            case DeleteVariable: {
                tmp.nodes[animations[i].id1].deleteVariable(animations[i].variableList);
                break;
            }
            case SetColorType: {
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
                tmp.arrangeAVLTrees();
                break;
            }
            case SetLeftNode: {
                tmp.nodes[animations[i].id1].leftNode = animations[i].nextValue;
                tmp.arrangeAVLTrees();
                break;
            }
            case SetRightNode: {
                tmp.nodes[animations[i].id1].rightNode = animations[i].nextValue;
                tmp.arrangeAVLTrees();
                break;
            }
            case SetLeftEdgeColorType: {
                tmp.nodes[animations[i].id1].leftEdgeType = AVL::ColorType(animations[i].nextValue);
            }
            case SetRightEdgeColorType: {
                tmp.nodes[animations[i].id1].rightEdgeType = AVL::ColorType(animations[i].nextValue);
            }
            case SetRoot: {
                tmp.root = animations[i].nextValue;
                tmp.arrangeAVLTrees();
                break;
            }
        }
    }
    return tmp;
}

void AVLGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            x->second.draw(window, theme);
        }
    }
    float percent = (totalTime < epsilonTime ? 1.f : timePassed / totalTime);
    AVLGraph tmp = execAnimation(animations);
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
        animationMap[animations[i].id1].push_back(animations[i]);
    }
    for (auto x = animationMap.begin(); x != animationMap.end(); x++) {
        int id = x->first;
        if (nodes.find(id) == nodes.end()) { //New node
            tmp.nodes[id].draw(window, theme, totalTime, timePassed, x->second);
        }
        else { //Old node
            nodes[id].draw(window, theme, totalTime, timePassed, x->second);
        }
    }
    // //Edges
    // for (auto x = nodes.begin(); x != nodes.end(); x++) {
    //     int idU = x->first;
    //     sf::Vector2f startPositionU = x->second.getPosition();
    //     sf::Vector2f goalPositionU;
    //     if (tmp.nodes.find(idU) == tmp.nodes.end()) {
    //         goalPositionU = x->second.getPosition();
    //     }
    //     else {
    //         goalPositionU = tmp.nodes[idU].getPosition();
    //     }
    //     if (nodes[idU].leftNode != -1) {
    //         //old edge disappear
    //         int idV = nodes[idU].leftNode;
    //         sf::Vector2f startPositionV = nodes[idV].getPosition();
    //         sf::Vector2f goalPositionV = tmp.nodes.find(idV) == tmp.nodes.end() ? nodes[idV].getPosition() : tmp.nodes[idV].getPosition();
    //         if (tmp.nodes.find(idU) == tmp.nodes.end() || tmp.nodes[idU].leftNode != nodes[idU].leftNode) {
    //             sf::Vector2f uPosition = startPositionU + (goalPositionU - startPositionU) * percent;
    //             sf::Vector2f vPosition = startPositionV + (goalPositionV - startPositionV) * percent;
    //             sf::RectangleShape line = getEdgeLine(uPosition, vPosition, 1 - percent);
    //             line.setFillColor(AVL::color[])
    //         }
    //     }
    // }
}