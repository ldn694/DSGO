#include "GeneralGraph.h"

GeneralGraph::GeneralGraph(std::vector <GeneralEdge> edges, sf::FloatRect viewRect, sf::Font* font) : font(font), viewRect(viewRect) {
    setEdges(edges);
}

void GeneralGraph::setEdges(std::vector <GeneralEdge> edges, bool directed, int maxSize) {
    this->isDirected = directed;
    this->edges.clear();
    for (auto x : edges) {
        this->edges.insert(x);
        maxSize = std::max(maxSize, std::max(x.from, x.to));
    }
    nodes.clear();
    for (int i = 1; i <= maxSize; i++) {
        nodes[i] = GeneralNode({0, 0}, i, font);
    }
    int numNode = nodes.size();
    float angle = 2 * PI / numNode;
    std::vector <int> idList;
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        idList.push_back(x->first);
    }
    //std::random_shuffle(idList.begin(), idList.end());
    if (idList.size() == 1) {
        nodes[idList[0]].setPosition(sf::Vector2f(viewRect.left + viewRect.width / 2, viewRect.top + viewRect.height / 2));
        return;
    }
    if (idList.size() == 2) {
        nodes[idList[0]].setPosition(sf::Vector2f(viewRect.left + viewRect.width / 2 - viewRect.width / 4, viewRect.top + viewRect.height / 2));
        nodes[idList[1]].setPosition(sf::Vector2f(viewRect.left + viewRect.width / 2 + viewRect.width / 4, viewRect.top + viewRect.height / 2));
        return;
    }
    for (int i = 0; i < idList.size(); i++) {
        int x = idList[i];
        nodes[x].setPosition(sf::Vector2f(viewRect.left + viewRect.width / 2 + viewRect.width * 0.5f * cos(angle * i), viewRect.top + viewRect.height / 2 + viewRect.height * 0.5f * sin(angle * i)));
    }
    // if (edges.size() <= maxSize * 2) {
    //     arrangeGraph();
    // }
    arrangeGraph();
}

sf::Vector2f GeneralGraph::springForce(sf::Vector2f stablePosition, sf::Vector2f mobilePosition, float idealLength) {
    sf::Vector2f delta = mobilePosition - stablePosition;
    float diff = length(delta) - idealLength;
    return -springConstant * diff * normalize(delta);
}

sf::Vector2f GeneralGraph::repelForce(sf::Vector2f stablePosition, sf::Vector2f mobilePosition, float idealLength) {
    sf::Vector2f delta = mobilePosition - stablePosition;
    float diff = length(delta) - idealLength;
    if (diff > 0) {
        return sf::Vector2f(0, 0);
    }
    return -springConstant * diff * normalize(delta);
}


void GeneralGraph::arrangeGraph() {
    std::vector <int> idList;
    for (auto x : nodes) {
        idList.push_back(x.first);
    }
    if (idList.empty()) {
        return;
    }
    int maxNode = nodes.rbegin()->first;
    std::vector <sf::Vector2f> pos, nextPos, prePos;
    pos.resize(maxNode + 1);
    nextPos.resize(maxNode + 1);
    for (int i = 0; i < idList.size(); i++) {
        pos[idList[i]] = nodes[idList[i]].getPosition();
    }
    prePos = pos;
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
        std::vector <sf::Vector2f> disp;
        disp.resize(maxNode + 1);
        for (int u : idList) {
            nextPos[u] = pos[u];
        }

        for (auto edge : edges) {
            int u = edge.from;
            int v = edge.to;
            sf::Vector2f middle = (pos[u] + pos[v]) / 2.0f;
            disp[u] += springForce(middle, pos[u], idealLength);
            disp[v] += springForce(middle, pos[v], idealLength);
        }
        std::vector <std::vector <bool>> adjacencyMatrix(maxNode + 1, std::vector <bool>(maxNode + 1, false));
        for (auto edge : edges) {
            int u = edge.from;
            int v = edge.to;
            adjacencyMatrix[u][v] = true;
        }
        for (int u : idList) {
            for (int v : idList) {
                if (!adjacencyMatrix[u][v]) {
                    sf::Vector2f middle = (pos[u] + pos[v]) / 2.0f;
                    disp[u] += repelForce(middle, pos[u], idealLength);
                    disp[v] += repelForce(middle, pos[v], idealLength);
                }
            }
        }
        for (auto edge : edges) {
            int u = edge.from;
            int v = edge.to;
            sf::Vector2f middle = (pos[u] + pos[v]) / 2.0f;
            for (int x : idList) {
                if (x == u || x == v) continue;
                disp[x] += repelForce(middle, pos[x], idealLength);
            }
            for (auto otherEdge : edges) {
                int x = otherEdge.from;
                int y = otherEdge.to;
                if (x == u && y == v) continue;
                sf::Vector2f otherMiddle = (pos[x] + pos[y]) / 2.0f;
                disp[x] += repelForce(middle, otherMiddle, idealLength);
                disp[y] += repelForce(middle, otherMiddle, idealLength);
            }
        }
        for (int x : idList) {
            for (auto edge : edges) {
                int u = edge.from;
                int v = edge.to;
                if (u == x || v == x) continue;
                sf::Vector2f middle = (pos[u] + pos[v]) / 2.0f;
                disp[u] += repelForce(pos[x], middle, idealLength);
                disp[v] += repelForce(pos[x], middle, idealLength);
            }
        }
        sf::Vector2f screenCenter = sf::Vector2f(viewRect.left + viewRect.width / 2, viewRect.top + viewRect.height / 2);
        sf::Vector2f totalPos = sf::Vector2f(0, 0);
        for (int u : idList) {
            totalPos += pos[u];
        }
        totalPos *= 1.f / maxNode;
        for (int u : idList) {
            disp[u] += springForce(screenCenter, totalPos, 0);
        }
        
        bool flag = false;
        for (int v : idList) {
            nextPos[v] += normalize(disp[v]) * std::min(length(disp[v]), maximumDisp);
            if (length(disp[v]) > epsilonFloat) {
                flag = true;
            }
            nextPos[v].x = std::max(viewRect.left, std::min(viewRect.left + viewRect.width, nextPos[v].x));
            nextPos[v].y = std::max(viewRect.top, std::min(viewRect.top + viewRect.height, nextPos[v].y));
        }
        maximumDisp *= damperingConst;
        pos = nextPos;
        if (!flag) {
            break;
        }
    }
    bool goodGraph = true;
    for (int u : idList) {
        for (int v : idList) {
            if (u == v) continue;
            if (pos[u] == pos[v]) {
                goodGraph = false;
                break;
            }
        }
        if (!goodGraph) {
            break;
        }
    }
    if (!goodGraph) {
        pos = prePos;
    }
    for (auto &x : nodes) {
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

std::set <GeneralEdge>::iterator GeneralGraph::findEdge(int from, int to) {
    for (auto x = edges.begin(); x != edges.end(); x++) {
        if (x->from == from && x->to == to) {
            return x;
        }
    }
    return edges.end();
}

std::vector <sf::RectangleShape> GeneralGraph::getEdgeLines(sf::Vector2f startPosition, sf::Vector2f endPosition, bool directed, bool upward) {
    sf::Vector2f delta = endPosition - startPosition;
    if (upward) {
        float diff = 3.f * thicknessGraph;
        float x1 = sqrt(radiusGraph * radiusGraph - diff * diff);
        float x2 = radiusGraph - x1;
        startPosition += normalize(sf::Vector2f(-delta.y, delta.x)) * 2.0f * thicknessGraph;
        startPosition -= normalize(delta) * x2;
        endPosition += normalize(sf::Vector2f(-delta.y, delta.x)) * 2.0f * thicknessGraph;
        endPosition += normalize(delta) * x2;
        delta = endPosition - startPosition;
    }
    std::vector <sf::RectangleShape> res;
    sf::RectangleShape line(sf::Vector2f(length(delta) - 2 * (radiusGraph + thicknessGraph), thicknessGraph));
    line.setOrigin(0, thicknessGraph / 2);
    line.setPosition(startPosition + normalize(delta) * (radiusGraph + thicknessGraph));
    line.setRotation(180 / PI * atan2(delta.y, delta.x));
    res.push_back(line);
    if (directed) {
        sf::RectangleShape smallLine1(sf::Vector2f(0.4f * radiusGraph, thicknessGraph));
        smallLine1.setOrigin(0, thicknessGraph / 2);
        smallLine1.setPosition(endPosition - normalize(delta) * (radiusGraph + thicknessGraph));
        smallLine1.setRotation(180 / PI * atan2(delta.y, delta.x) + 145);

        res.push_back(smallLine1);
        sf::RectangleShape smallLine2(sf::Vector2f(0.4f * radiusGraph, thicknessGraph));
        smallLine2.setOrigin(0, thicknessGraph / 2);
        smallLine2.setPosition(endPosition - normalize(delta) * (radiusGraph + thicknessGraph));
        smallLine2.setRotation(180 / PI * atan2(delta.y, delta.x) - 145);
        res.push_back(smallLine2);
    }
    return res;
}

std::vector <sf::Text> GeneralGraph::getEdgeWeightText(sf::Vector2f startPosition, sf::Vector2f endPosition, int weight, bool upward) {
    sf::Vector2f delta = endPosition - startPosition;
    if (upward) {
        float diff = 3.f * thicknessGraph;
        float x1 = sqrt(radiusGraph * radiusGraph - diff * diff);
        float x2 = radiusGraph - x1;
        startPosition += normalize(sf::Vector2f(-delta.y, delta.x)) * 3.f * thicknessGraph;
        startPosition -= normalize(delta) * x2;
        endPosition += normalize(sf::Vector2f(-delta.y, delta.x)) * 3.f * thicknessGraph;
        endPosition += normalize(delta) * x2;
        delta = endPosition - startPosition;
    }
    std::vector <sf::Text> res;
    sf::Text text(std::to_string(weight), *font, 20);
    text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
    text.setPosition(startPosition + normalize(delta) * (length(delta) * 0.5f) + normalize(sf::Vector2f(-delta.y, delta.x)) * (3.0f * thicknessGraph + text.getGlobalBounds().height / 2));
    // text.setRotation(180 / PI * atan2(delta.y, delta.x));
    res.push_back(text);
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
            case SetEdgeType: {
                auto it = tmp.findEdge(animations[i].id1, animations[i].id2);
                if (it == tmp.edges.end()) {
                    assert(false);
                }
                int weight = it->weight;
                tmp.edges.erase(it);
                tmp.edges.insert(GeneralEdge(animations[i].id1, animations[i].id2, weight, General::ColorType(animations[i].nextValue)));
                break;
            }
            case SetOpacity: {
                auto it = tmp.findEdge(animations[i].id1, animations[i].id2);
                if (it == tmp.edges.end()) {
                    assert(false);
                }
                int weight = it->weight;
                General::ColorType type = it->type;
                tmp.edges.erase(it);
                tmp.edges.insert(GeneralEdge(animations[i].id1, animations[i].id2, weight, type, animations[i].nextPercent));
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
                    bool upward = false;
                    if (isDirected) {
                        for (auto z = edges.begin(); z != edges.end(); z++) {
                            if (z->from == y->to && z->to == y->from) {
                                upward = true;
                                break;
                            }
                        }
                    }
                    //std::cout << y->from << " " << y->to << "\n";
                    std::vector <sf::RectangleShape> lines = getEdgeLines(x->second.getPosition(), nodes[y->to].getPosition(), isDirected, upward);
                    sf::Color color = General::color[theme][y->type].outlineColor;
                    color.a = 255 * y->opacity;
                    for (auto line : lines) {
                        line.setFillColor(color);
                        window.draw(line);
                    }
                    std::vector <sf::Text> texts = getEdgeWeightText(x->second.getPosition(), nodes[y->to].getPosition(), y->weight, upward);
                    for (auto text : texts) {
                        text.setFillColor(color);
                        window.draw(text);
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
        if (animations[i].animationType == SetEdgeType) continue;
        if (animations[i].animationType == SetOpacity) continue;
        if (animations[i].id1 != -1) {
            animationMap[animations[i].id1].push_back(animations[i]);
        }
    }
    //Edges
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
            for (auto y = edges.begin(); y != edges.end(); y++) {
                if (y->from == x->first) {
                    if (!isDirected && y->from > y->to) {
                        continue;
                    }
                    auto p = tmp.edges.end();
                    for (auto z = tmp.edges.begin(); z != tmp.edges.end(); z++) {
                        if (z->from == y->from && z->to == y->to) {
                            p = z;
                            break;
                        }
                    }
                    if (p == tmp.edges.end()) {
                        assert(false);
                    }
                    bool upward = false;
                    if (isDirected) {
                        for (auto z = edges.begin(); z != edges.end(); z++) {
                            if (z->from == y->to && z->to == y->from) {
                                upward = true;
                                break;
                            }
                        }
                    }
                    std::vector <sf::RectangleShape> lines = getEdgeLines(x->second.getPosition(), nodes[y->to].getPosition(), isDirected, upward);
                    sf::Color color = General::fadingColorType(y->type, y->opacity, p->type, p->opacity, theme, percent).outlineColor;
                    for (auto line : lines) {
                        line.setFillColor(color);
                        window.draw(line);
                    }
                    std::vector <sf::Text> texts = getEdgeWeightText(x->second.getPosition(), nodes[y->to].getPosition(), y->weight, upward);
                    for (auto text : texts) {
                        text.setFillColor(color);
                        window.draw(text);
                    }
                }
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