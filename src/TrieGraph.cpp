#include "TrieGraph.h"

TrieGraph::TrieGraph(sf::Vector2f startPosition, sf::Font* font) : startPosition(startPosition), font(font) {
    nodes = std::map <int, TrieNode>();
    root = 0;
    nodes[0] = TrieNode(startPosition, "", font);
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

int TrieGraph::getParent(int id) {
    if (id == -1) {
        return -1;
    }
    for (auto x = nodes.begin(); x != nodes.end(); x++) {
        for (auto edge : x->second.edges) {
            if (edge == id) {
                return x->first;
            }
        }
    }
    return -1;
}

sf::RectangleShape TrieGraph::getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent) {
    sf::Vector2f diff = endPosition - startPosition;
    if (length(diff) < 2 * (radiusTrie + thicknessTrie)) {
        return sf::RectangleShape();
    }
    sf::RectangleShape line(sf::Vector2f((length(diff) - 2 * (radiusTrie + thicknessTrie)) * percent, thicknessTrie));
    line.setOrigin(0, thicknessTrie / 2);
    line.setPosition(startPosition + normalize(diff) * (radiusTrie + thicknessTrie));
    line.setRotation(atan2(diff.y, diff.x) * 180 / PI);
    return line;
}

void TrieGraph::DFS(int id, int height, std::vector <std::vector <int> > &tour) {
    if (nodes.find(id) == nodes.end()) {
        assert(false);
    }
    tour[height].push_back(id);
    for (auto& child : nodes[id].edges) {
        if (child != -1) {
            DFS(child, height + 1, tour);
        }
    }
}

void TrieGraph::arrangeTrieTree() {
    if (root == -1) {
        return;
    }
    std::vector <std::vector <int> > tour(nodes.size() + 1);
    DFS(root, 0, tour);
    int leafHeight = -1;
    while (leafHeight + 1 < tour.size() && tour[leafHeight + 1].size() > 0) {
        leafHeight++;
    }
    float totalWidth = 0;
    for (int h = leafHeight; h >= 0; h--)  {
        for (int i = 0; i < tour[h].size(); i++) {
            int u = tour[h][i];
            if (nodes.find(u) == nodes.end()) {
                assert(false);
            }
            if (nodes[u].edges.empty()) {//leaf
                totalWidth += 2 * (radiusTrie + thicknessTrie) + minHorizontalDistTrie;
            }
        }
    }
    totalWidth -= minHorizontalDistTrie;
    float startX = startPosition.x - totalWidth / 2;
    float startY = startPosition.y + leafHeight * verticalDistAVL;
    for (int h = leafHeight; h >= 0; h--)  {
        for (int i = 0; i < tour[h].size(); i++) {
            int u = tour[h][i];
            if (nodes.find(u) == nodes.end()) {
                assert(false);
            }
            if (nodes[u].edges.empty()) {//leaf
                nodes[u].setPosition(sf::Vector2f(startX + radiusTrie + thicknessTrie, startY));
                startX += 2 * (radiusTrie + thicknessTrie) + minHorizontalDistTrie;
            }
        }
    }
    for (int h = leafHeight; h >= 0; h--)  {
        for (int i = 0; i < tour[h].size(); i++) {
            int u = tour[h][i];
            if (nodes.find(u) == nodes.end()) {
                assert(false);
            }
            if (!nodes[u].edges.empty()) {//not leaf
                float totalX = 0;
                for (int v : nodes[u].edges) {
                    totalX += nodes[v].getPosition().x;
                }
                nodes[u].setPosition(sf::Vector2f(totalX / nodes[u].edges.size(), startPosition.y + h * verticalDistAVL));
            }
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
                tmp.nodes[animations[i].id1].insertEdge(animations[i].id2);
                tmp.arrangeTrieTree();
            }
            case DeleteEdge: {
                tmp.nodes[animations[i].id1].deleteEdge(animations[i].id2);
                tmp.arrangeTrieTree();
            }
        }
    }
    return tmp;
}

void TrieGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            x->second.draw(window, theme);
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