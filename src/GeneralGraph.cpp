#include "GeneralGraph.h"

GeneralGraph::GeneralGraph(std::vector <GeneralEdge> edges, sf::RectangleShape viewRect, sf::Font* font) : font(font), viewRect(viewRect) {
    setEdges(edges);
}

void GeneralGraph::setEdges(std::vector <GeneralEdge> edges) {
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
    float distX = viewRect.getSize().x / (numCol + 1);
    float distY = viewRect.getSize().y / (numRow + 1);
    int cnt = 0;
    for (auto x : nodes) {
        int row = cnt / numCol;
        int col = cnt % numCol;
        x.second.setPosition(viewRect.getPosition() + sf::Vector2f(distX * (col + 1), distY * (row + 1)));
        cnt++;
    }
    arrangeGraph();
}

sf::Vector2f GeneralGraph::fAttract(int u, int v, std::map <int, sf::Vector2f>& pos) {
    return idealLength / length(pos[v] - pos[u]) * (pos[u] - pos[v]);
}

sf::Vector2f GeneralGraph::fRepel(int u, int v, std::map <int, sf::Vector2f>& pos) {
    float len = length(pos[v] - pos[u]);
    return len * len / idealLength * (pos[v] - pos[u]);
}

void GeneralGraph::arrangeGraph() {
    std::vector <int> idList;
    for (auto x : nodes) {
        idList.push_back(x.first);
    }
    std::map <int, sf::Vector2f> pos, nextPos;
    for (int t = 0; t < maxStep; t++) {
        bool flag = false;
        nextPos = pos;
        for (int i = 0; i < idList.size(); i++) {
            sf::Vector2f fTotal = {0.f, 0.f};
            int u = idList[i];
            for (int j = 0; j < idList.size(); j++) {
                if (j == i) continue;
                int v = idList[j];
                fTotal += fRepel(u, v, pos);
            }
            std::set <int> adj;
            for (auto x : edges) {
                if (x.from == u) {
                    adj.insert(x.to);
                }
                if (x.to == u) {
                    adj.insert(x.from);
                }
            }
            for (auto v : adj) {
                fTotal += fAttract(u, v, pos);
            }
            if (fTotal.x > epsilonFloat || fTotal.y > epsilonFloat) {
                flag = true;
                nextPos[u] = pos[u] + fTotal;
            }
        }
        if (!flag) {
            break;
        }
        else {
            pos = nextPos;
        }
    }
    for (auto x : nodes) {
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
            x->second.draw(window, theme);
        }
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