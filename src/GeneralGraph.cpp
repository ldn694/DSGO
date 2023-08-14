#include "GeneralGraph.h"

GeneralGraph::GeneralGraph(std::vector <GeneralEdge> edges, sf::FloatRect viewRect, sf::Font* font) : font(font), viewRect(viewRect) {
    setEdges(edges);
}

void GeneralGraph::setEdges(std::vector <GeneralEdge> edges, bool directed) {
    this->isDirected = directed;
    // std::cout << "------------\n";
    // for (auto x : edges) {
    //     std::cout << x.from << " " << x.to << " " << x.weight << "\n";
    // }
    this->edges.clear();
    for (auto x : edges) {
        this->edges.insert(x);
    }
    nodes.clear();
    for (auto x : edges) {
        nodes[x.from] = GeneralNode({0, 0}, x.from, font);
        nodes[x.to] = GeneralNode({0, 0}, x.to, font);
    }
    int numNode = nodes.size();
    int numRow = std::max(int(sqrt(numNode)), 1);
    int numCol = (numNode + numRow - 1) / numRow;
    // std::cout << "numRow = " << numRow << " numCol = " << numCol << "\n";
    float distX = viewRect.getSize().x / (numCol + 1);
    float distY = viewRect.getSize().y / (numRow + 1);
    // std::cout << "distX = " << distX << " distY = " << distY << "\n";
    int cnt = 0;
    for (auto &x : nodes) {
        int row = cnt / numCol;
        int col = cnt % numCol;
        x.second.setPosition(viewRect.getPosition() + sf::Vector2f(distX * (col + 1), distY * (row + 1)));
        // std::cout << "cnt = " << cnt << " row = " << row << " col = " << col << " " << x.second.getPosition().x << " " << x.second.getPosition().y << "\n";
        cnt++;
    }
    arrangeGraph();
}

float GeneralGraph::fAttract(float x) {
    return x * x / idealLength;
}
float GeneralGraph::fRepel(float x) {
    return idealLength * idealLength / x;
}


void GeneralGraph::arrangeGraph() {
    // std::cout << "--------------------\n";
    std::vector <int> idList;
    for (auto x : nodes) {
        idList.push_back(x.first);
    }
    std::map <int, sf::Vector2f> pos, nextPos;
    for (int i = 0; i < idList.size(); i++) {
        pos[idList[i]] = nodes[idList[i]].getPosition();
    }
    float maximumDisp = 0;
    for (auto u : idList) {
        for (auto v : idList) {
            if (u == v) continue;
            sf::Vector2f delta = pos[u] - pos[v];
            maximumDisp = std::max(maximumDisp, length(delta));
        }
    }
    maximumDisp /= 2;
    for (int t = 0; t < maxStep; t++) {
        // std::cout << "iteration " << t << "\n";
        std::map <int, sf::Vector2f> disp;
        // for (int v : idList) {
        //     std::cout << v << " " << pos[v].x << " " << pos[v].y << "\n";
        // }
        nextPos.clear();
        for (int v : idList) {
            nextPos[v] = pos[v];
        }
        for (int v : idList) {
            for (int u : idList) {
                if (u == v) continue;
                sf::Vector2f delta = pos[v] - pos[u];
                disp[v] += normalize(delta) * fRepel(length(delta));
            }
        }
        for (auto x : edges) {
            int v = x.from;
            int u = x.to;
            sf::Vector2f delta = pos[v] - pos[u];
            disp[v] -= normalize(delta) * fAttract(length(delta));
            disp[u] += normalize(delta) * fAttract(length(delta));
        }
        for (int v : idList) {
            nextPos[v] += normalize(disp[v]) * std::min(length(disp[v]), maximumDisp);
            // nextPos[v].x = std::max(viewRect.left, std::min(viewRect.left + viewRect.width, nextPos[v].x));
            // nextPos[v].y = std::max(viewRect.top, std::min(viewRect.top + viewRect.height, nextPos[v].y));
            maximumDisp *= damperingConst;
        }
        pos = nextPos;
    }
    for (auto &x : nodes) {
        // std::cout << x.first << " " << pos[x.first].x << " " << pos[x.first].y << "\n";
        x.second.setPosition(pos[x.first]);
    }
}

int GeneralGraph::getMexID() {
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

std::vector <sf::RectangleShape> GeneralGraph::getEdgeLines(sf::Vector2f startPosition, sf::Vector2f endPosition, bool directed) {
    std::vector <sf::RectangleShape> res;
    sf::Vector2f delta = endPosition - startPosition;
    sf::RectangleShape line(sf::Vector2f(length(delta) - 2 * (radiusGraph + thicknessGraph), thicknessGraph));
    line.setOrigin(0, thicknessGraph / 2);
    line.setPosition(startPosition + normalize(delta) * (radiusGraph + thicknessGraph));
    line.setRotation(180 / PI * atan2(delta.y, delta.x));
    res.push_back(line);
    if (directed) {
        sf::RectangleShape smallLine1(sf::Vector2f(0.5f * radiusGraph, thicknessGraph));
        smallLine1.setOrigin(0, thicknessGraph / 2);
        smallLine1.setPosition(endPosition - normalize(delta) * (radiusGraph + thicknessGraph));
        smallLine1.setRotation(180 / PI * atan2(delta.y, delta.x) + 135);

        res.push_back(smallLine1);
        sf::RectangleShape smallLine2(sf::Vector2f(0.5f * radiusGraph, thicknessGraph));
        smallLine2.setOrigin(0, thicknessGraph / 2);
        smallLine2.setPosition(endPosition - normalize(delta) * (radiusGraph + thicknessGraph));
        smallLine2.setRotation(180 / PI * atan2(delta.y, delta.x) - 135);
        res.push_back(smallLine2);
    }
    return res;
}

GeneralGraph GeneralGraph::execAnimation(std::vector <Animation> animations) {
    GeneralGraph tmp = *this;
    std::sort(animations.begin(), animations.end());
    for (int i = 0; i < animations.size(); i++) {
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
                tmp.nodes[animations[i].id1].setColorType(General::ColorType(animations[i].nextValue));
                break;
            }
        }
    }
    return tmp;
}

void GeneralGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            // std::cout << x->first << " ";
            x->second.draw(window, theme);
            for (auto y = edges.begin(); y != edges.end(); y++) {
                if (y->from == x->first) {
                    if (!isDirected && y->from > y->to) {
                        continue;
                    }
                    //std::cout << y->from << " " << y->to << "\n";
                    std::vector <sf::RectangleShape> lines = getEdgeLines(x->second.getPosition(), nodes[y->to].getPosition(), isDirected);
                    for (auto line : lines) {
                        line.setFillColor(General::color[y->type]->outlineColor);
                        window.draw(line);
                    }
                }
            }
        }
        // std::cout << "\n";
    }
    float percent = (totalTime < epsilonTime ? 1.f : timePassed / totalTime);
    GeneralGraph tmp = execAnimation(animations);
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