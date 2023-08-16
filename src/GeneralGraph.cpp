#include "GeneralGraph.h"

GeneralGraph::GeneralGraph(std::vector <GeneralEdge> edges, sf::FloatRect viewRect, sf::Font* font) : font(font), viewRect(viewRect) {
    setEdges(edges);
}

void GeneralGraph::setEdges(std::vector <GeneralEdge> edges, bool directed, int maxSize) {
    this->isDirected = directed;
    // std::cout << "------------\n";
    // for (auto x : edges) {
    //     std::cout << x.from << " " << x.to << " " << x.weight << "\n";
    // }
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
    std::random_shuffle(idList.begin(), idList.end());
    for (int i = 0; i < idList.size(); i++) {
        int x = idList[i];
        nodes[x].setPosition(sf::Vector2f(viewRect.left + viewRect.width / 2 + idealLength * cos(angle * i), viewRect.top + viewRect.height / 2 + idealLength * sin(angle * i)));
    }
    if (edges.size() <= maxSize * 2) {
        arrangeGraph();
    }
}

float GeneralGraph::fAttract(float x) {
    return x * x / idealLength;
}
float GeneralGraph::fRepel(float x) {
    return idealLength * idealLength / x;
}


void GeneralGraph::arrangeGraph() {
    std::vector <int> idList;
    for (auto x : nodes) {
        idList.push_back(x.first);
    }
    if (idList.empty()) {
        return;
    }
    DSU F(idList.back());
    for (auto x : edges) {
        // std::cout << x.from << " -> " << x.to << " " << x.weight << "\n";
        F.join(x.from, x.to);
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
        std::map <int, sf::Vector2f> disp;
        std::map <int, std::map <int, sf::Vector2f> > differentCCDisp;
        nextPos.clear();
        for (int v : idList) {
            nextPos[v] = pos[v];
        }
        for (int v : idList) {
            for (int u : idList) {
                if (u == v) continue;
                if (F.find(u) == F.find(v)) {
                    sf::Vector2f delta = pos[v] - pos[u];
                    disp[v] += normalize(delta) * fRepel(length(delta));
                }
                else {
                    sf::Vector2f delta = pos[v] - pos[u];
                    //differentCCDisp[F.find(v)][F.find(u)] += normalize(delta) * fRepel(length(delta));
                    disp[v] += normalize(delta) * fRepel(length(delta));
                }
            }
        }
        // for (int v : idList) {
        //     for (int u : idList) {
        //         if (u != F.find(u)) continue;
        //         if (F.find(u) != F.find(v)) {
        //             if (F.getSize(u) == 0) {
        //                 assert(false);
        //             }
        //             disp[v] += differentCCDisp[F.find(v)][F.find(u)];
        //         }
        //     }   
        // }
        for (auto x : edges) {
            int v = x.from;
            int u = x.to;
            sf::Vector2f delta = pos[v] - pos[u];
            disp[v] -= normalize(delta) * fAttract(length(delta));
            disp[u] += normalize(delta) * fAttract(length(delta));
        }
        bool flag = false;
        for (int v : idList) {
            nextPos[v] += normalize(disp[v]) * std::min(length(disp[v]), maximumDisp);
            if (length(disp[v]) > epsilonFloat) {
                flag = true;
            }
            nextPos[v].x = std::max(viewRect.left, std::min(viewRect.left + viewRect.width, nextPos[v].x));
            nextPos[v].y = std::max(viewRect.top, std::min(viewRect.top + viewRect.height, nextPos[v].y));
            maximumDisp *= damperingConst;
        }
        pos = nextPos;
        if (!flag) {
            break;
        }
    }
    for (auto &x : nodes) {
        // std::cout << x.first << " " << pos[x.first].x << " " << pos[x.first].y << "\n";
        x.second.setPosition(pos[x.first]);
    }
    // std::cout << "DONE!\n";
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
                    for (auto line : lines) {
                        line.setFillColor(General::color[theme][y->type].outlineColor);
                        window.draw(line);
                    }
                    std::vector <sf::Text> texts = getEdgeWeightText(x->second.getPosition(), nodes[y->to].getPosition(), y->weight, upward);
                    for (auto text : texts) {
                        text.setFillColor(General::color[theme][y->type].outlineColor);
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