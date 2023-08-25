#include "TrieGraph.h"

TrieGraph::TrieGraph(sf::Vector2f startPosition, sf::Font* font) : startPosition(startPosition), font(font) {
    nodes = std::map <int, TrieNode>();
    root = 0;
    nodes[0] = TrieNode(startPosition, "", font);
    nodes[0].setState(ISWORD);
}

int TrieGraph::getMexID() {
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

int TrieGraph::findEdge(int id, std::string weight) {
    if (nodes.find(id) == nodes.end()) {
        assert(false);
    }
    for (auto x : nodes[id].edges) {
        if (x.second == weight) {
            return x.first;
        }
    }
    return -1;
}

int TrieGraph::getParent(int id) {
    if (id == -1) {
        return -1;
    }
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        for (auto edge : x->second.edges) {
            if (edge.first == id) {
                return x->first;
            }
        }
    }
    return -1;
}

int TrieGraph::countString() {
    int cnt = 0;
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        if (x->second.state == ISWORD) {
            cnt++;
        }
    }
    return cnt;
}

std::pair <sf::RectangleShape, sf::Text> TrieGraph::getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, std::string weight, float percent) {
    sf::Vector2f diff = endPosition - startPosition;
    if (length(diff) < 2 * (radiusTrie + thicknessTrie)) {
        return {sf::RectangleShape(), sf::Text()};
    }
    sf::RectangleShape line(sf::Vector2f((length(diff) - 2 * (radiusTrie + thicknessTrie)) * percent, thicknessTrie));
    line.setOrigin(0, thicknessTrie / 2);
    line.setPosition(startPosition + normalize(diff) * (radiusTrie + thicknessTrie));
    line.setRotation(atan2(diff.y, diff.x) * 180 / PI);
    sf::Text text;
    text.setFont(*font);
    text.setString(weight);
    text.setCharacterSize(sizeValueLetterTrie);
    text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, text.getLocalBounds().top + text.getLocalBounds().height / 2);
    text.setPosition(startPosition + normalize(diff) * (length(diff) / 2) + normalize(sf::Vector2f(-diff.y, diff.x)) * (thicknessTrie * 3));
    return {line, text};
}

void TrieGraph::setThemeEdge(std::pair <sf::RectangleShape, sf::Text> &edge, Trie::ColorType type, ColorTheme theme) {
    if (type != Trie::ColorType::highlight2) {
        edge.first.setFillColor(Trie::color[theme][type].outlineColor);
        edge.second.setFillColor(Trie::color[theme][type].outlineColor);
    }
    else {
        edge.first.setFillColor(Trie::color[theme][Trie::ColorType::normal].outlineColor);
        edge.second.setFillColor(Trie::color[theme][Trie::ColorType::normal].outlineColor);
    }
}

void TrieGraph::DFS(int id, int height, std::vector <std::pair <int, int> > &tour) {
    if (nodes.find(id) == nodes.end()) {
        assert(false);
    }
    tour.push_back(std::make_pair(id, height));
    std::vector <std::pair<int, std::string>> children;
    for (auto& child : nodes[id].edges) {
        if (child.first != -1) {
            children.push_back(child);
        }
    }
    std::function <bool(std::pair<int, std::string>, std::pair<int, std::string>)> cmp = [&](std::pair<int, std::string> a, std::pair<int, std::string> b) {
        return a.second > b.second;
    };
    sort(children.begin(), children.end(), cmp);
    for (auto& child : children) {
        DFS(child.first, height + 1, tour);
    }
}

void TrieGraph::arrangeTrieTree() {
    if (root == -1) {
        return;
    }
    std::vector <std::pair <int, int> > tour;
    DFS(root, 0, tour);
    float totalWidth = 0;
    for (auto x : tour) {
        int u = x.first;
        if (nodes.find(u) == nodes.end()) {
            assert(false);
        }
        if (nodes[u].edges.empty()) {//leaf
            totalWidth += 2 * (radiusTrie + thicknessTrie) + minHorizontalDistTrie;
        }
    }
    totalWidth -= minHorizontalDistTrie;
    float startX = startPosition.x - totalWidth / 2;
    for (auto x = tour.rbegin(); x != tour.rend(); x++) {
        int u = x->first, h = x->second;
        if (nodes.find(u) == nodes.end()) {
            assert(false);
        }
        if (nodes[u].edges.empty()) {//leaf
            nodes[u].setPosition(sf::Vector2f(startX + radiusTrie + thicknessTrie, startPosition.y + h * verticalDistAVL));
                startX += 2 * (radiusTrie + thicknessTrie) + minHorizontalDistTrie;
        }
        else {//not leaf
            float totalX = 0;
            for (auto y : nodes[u].edges) {
                int v = y.first;
                if (nodes.find(v) == nodes.end()) {
                    assert(false);
                }
                totalX += nodes[v].getPosition().x;
            }
            nodes[u].setPosition(sf::Vector2f(totalX / nodes[u].edges.size(), startPosition.y + h * verticalDistAVL));
        }
    }
}

TrieGraph TrieGraph::execAnimation(std::vector <Animation> animations) {
    TrieGraph tmp = *this;
    std::sort(animations.begin(), animations.end());
    for (int i = 0; i < animations.size(); i++) {
        switch (animations[i].animationType) {
            case SetValue: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].setValue(animations[i].nextString);
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
                    std::cout << animations[i].id1 << "doesnt exist\n";
                    assert(false);
                }
                tmp.nodes[animations[i].id1].setColorType(Trie::ColorType(animations[i].nextValue));
                break;
            }
            case AddNode: {
                tmp.nodes[animations[i].id1] = TrieNode({0, 0}, animations[i].nextString, font);
                tmp.arrangeTrieTree();
                tmp.nodes[animations[i].id1].setSize(1.f);
                break;
            }
            case DeleteNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes.erase(animations[i].id1);
                tmp.arrangeTrieTree();
                break;
            }
            case SetRoot: {
                tmp.root = animations[i].nextValue;
                tmp.arrangeTrieTree();
                break;
            }
            case InsertEdge: {
                tmp.nodes[animations[i].id1].insertEdge(animations[i].id2, animations[i].nextString);
                tmp.arrangeTrieTree();
                break;
            }
            case DeleteEdge: {
                tmp.nodes[animations[i].id1].deleteEdge(animations[i].id2);
                tmp.arrangeTrieTree();
                break;
            }
            case SetState: {
                tmp.nodes[animations[i].id1].setState(animations[i].nextValue);
                break;
            }
        }
    }
    return tmp;
}

void TrieGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            x->second.draw(window, theme);
            for (auto y : x->second.edges) {
                if (nodes.find(y.first) == nodes.end()) {
                    assert(false);
                }
                int v = y.first;
                std::string weight = y.second;
                auto edge = getEdgeLine(x->second.getPosition(), nodes[v].getPosition(), weight, 1.f);
                setThemeEdge(edge, nodes[v].type, theme);
                window.draw(edge.first);
                window.draw(edge.second);
            }
        }
    }
    float percent = (totalTime < epsilonTime ? 1.f : timePassed / totalTime);
    TrieGraph tmp = execAnimation(animations);
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
    for (auto x = tmp.nodes.begin(); x != tmp.nodes.end(); x++) {
        int idU = x->first;
        if (nodes.find(idU) == nodes.end()) {//new node U
            for (auto y = x->second.edges.begin(); y != x->second.edges.end(); y++) {
                int idV = y->first;
                std::string weight = y->second;
                if (idV == -1) continue;
                if (nodes.find(idV) == nodes.end()) {//new node V
                    sf::Vector2f curPositionU = tmp.nodes[idU].getPosition();
                    sf::Vector2f curPositionV = tmp.nodes[idV].getPosition();
                    auto edge = getEdgeLine(curPositionU, curPositionV, weight, percent);
                    setThemeEdge(edge, tmp.nodes[idV].type, theme);
                    window.draw(edge.first);
                    window.draw(edge.second);
                }
                else {//old node V
                    sf::Vector2f curPositionU = tmp.nodes[idU].getPosition();
                    sf::Vector2f curPositionV = nodes[idV].getPosition() + (tmp.nodes[idV].getPosition() - nodes[idV].getPosition()) * percent;
                    auto edge = getEdgeLine(curPositionU, curPositionV, weight, percent);
                    setThemeEdge(edge, tmp.nodes[idV].type, theme);
                    window.draw(edge.first);
                    window.draw(edge.second);
                }
            }
        }
        else {//old node U
            for (auto y = x->second.edges.begin(); y != x->second.edges.end(); y++) {
                int idV = y->first;
                std::string weight = y->second;
                if (idV == -1) continue;
                if (nodes.find(idV) == nodes.end()) {//new node V
                    sf::Vector2f curPositionU = nodes[idU].getPosition() + (tmp.nodes[idU].getPosition() - nodes[idU].getPosition()) * percent;
                    sf::Vector2f curPositionV = tmp.nodes[idV].getPosition();
                    auto edge = getEdgeLine(curPositionU, curPositionV, weight, percent);
                    setThemeEdge(edge, tmp.nodes[idV].type, theme);
                    window.draw(edge.first);
                    window.draw(edge.second);
                }
                else {//old node V
                    sf::Vector2f curPositionU = nodes[idU].getPosition() + (tmp.nodes[idU].getPosition() - nodes[idU].getPosition()) * percent;
                    sf::Vector2f curPositionV = nodes[idV].getPosition() + (tmp.nodes[idV].getPosition() - nodes[idV].getPosition()) * percent;
                    auto edge = getEdgeLine(curPositionU, curPositionV, weight, 1.0f);
                    setThemeEdge(edge, tmp.nodes[idV].type, theme);
                    window.draw(edge.first);
                    window.draw(edge.second);
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