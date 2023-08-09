#include "BTreeGraph.h"

BTreeGraph::BTreeGraph(sf::Vector2f startPosition, sf::Font* font) : startPosition(startPosition), font(font) {
    root = -1;
}

int BTreeGraph::getGroupID(int id) {
    for (auto& group : groups) {
        for (auto& node : group.second.nodes) {
            if (node == id) return group.first;
        }
    }
    return -1;
}

int BTreeGraph::getParentGroup(int id) {
    int groupID = getGroupID(id);
    if (groupID == -1) return -1;
    for (auto& group : groups) {
        for (auto& edge : group.second.listEdge) {
            if (edge == groupID) return group.first;
        }
    }
    return -1;
}

int BTreeGraph::getMexNodeID() {
    std::set <int> idList;
    for (auto& node : nodes) {
        idList.insert(node.first);
    }
    int mex = 0;
    while (true) {
        if (idList.find(mex) == idList.end()) return mex;
        mex++;
    }
}

int BTreeGraph::getMexGroupID() {
    std::set <int> idList;
    for (auto& group : groups) {
        idList.insert(group.first);
    }
    int mex = 0;
    while (true) {
        if (idList.find(mex) == idList.end()) return mex;
        mex++;
    }
}

std::vector <int> BTreeGraph::getMexNodeIDs(int size) {
    std::set <int> idList;
    for (auto& node : nodes) {
        idList.insert(node.first);
    }
    std::vector <int> mexList;
    int mex = 0;
    while (mexList.size() < size) {
        if (idList.find(mex) == idList.end()) mexList.push_back(mex);
        mex++;
    }
    return mexList;
}

std::vector <int> BTreeGraph::getMexGroupIDs(int size) {
    std::set <int> idList;
    for (auto& group : groups) {
        idList.insert(group.first);
    }
    std::vector <int> mexList;
    int mex = 0;
    while (mexList.size() < size) {
        if (idList.find(mex) == idList.end()) mexList.push_back(mex);
        mex++;
    }
    return mexList;
}

sf::RectangleShape BTreeGraph::getEdgeLine(sf::Vector2f startPosition, sf::Vector2f endPosition, float percent) {
    sf::Vector2f diff = endPosition - startPosition;
    if (length(diff) < epsilonFloat) {
        return sf::RectangleShape();
    }
    sf::RectangleShape line(sf::Vector2f(length(diff) * percent, thicknessBTree));
    line.setOrigin(0, thicknessBTree / 2);
    line.setPosition(startPosition);
    line.setRotation(atan2(diff.y, diff.x) * 180 / PI);
    return line;
}

std::vector <int> BTreeGraph::getEdges(int idGroup) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    return groups[idGroup].listEdge;
}

void BTreeGraph::setPosition(int idGroup, sf::Vector2f position) {
    if (groups.find(idGroup) == groups.end()) {
        return;
    }
    auto& group = groups[idGroup];
    if (group.nodes.empty()) {
        return;
    }
    int mid = group.nodes.size() / 2;
    if (group.nodes.size() % 2 == 0) {
        nodes[group.nodes[mid - 1]].setPosition(position - sf::Vector2f(edgeBTree / 2, 0));
        nodes[group.nodes[mid]].setPosition(position + sf::Vector2f(edgeBTree / 2, 0));
        for (int i = mid - 2; i >= 0; i--) {
            nodes[group.nodes[i]].setPosition(nodes[group.nodes[i + 1]].getPosition() - sf::Vector2f(edgeBTree, 0));
        }
        for (int i = mid + 1; i < group.nodes.size(); i++) {
            nodes[group.nodes[i]].setPosition(nodes[group.nodes[i - 1]].getPosition() + sf::Vector2f(edgeBTree, 0));
        }
    }
    else {
        nodes[group.nodes[mid]].setPosition(position);
        for (int i = mid - 1; i >= 0; i--) {
            nodes[group.nodes[i]].setPosition(nodes[group.nodes[i + 1]].getPosition() - sf::Vector2f(edgeBTree, 0));
        }
        for (int i = mid + 1; i < group.nodes.size(); i++) {
            nodes[group.nodes[i]].setPosition(nodes[group.nodes[i - 1]].getPosition() + sf::Vector2f(edgeBTree, 0));
        }
    }
}

void BTreeGraph::setEdge(int idGroup, int pos, int id) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    if (pos < 0 || pos > group.nodes.size()) {
        assert(false);
    }
    group.setEdge(pos, id);
}

void BTreeGraph::insertNode(int idGroup, int idNode) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    if (nodes.find(idNode) == nodes.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    int res = -1;
    for (int i = 0; i < group.nodes.size(); i++) {
        if (nodes[group.nodes[i]].getValue() >= nodes[idNode].getValue()) {
            res = i;
            break;
        }
    }
    if (res == -1) {
        group.nodes.push_back(idNode);
    }
    else {
        group.nodes.insert(group.nodes.begin() + res, idNode);
    }
    group.listEdge.assign(group.nodes.size() + 1, -1);
}

void BTreeGraph::deleteNode(int idGroup, int idNode) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    if (nodes.find(idNode) == nodes.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    for (int i = 0; i < group.nodes.size(); i++) {
        if (group.nodes[i] == idNode) {
            group.nodes.erase(group.nodes.begin() + i);
            group.listEdge.assign(group.nodes.size() + 1, -1);
            return;
        }
    }
    assert(false);
}

sf::Vector2f BTreeGraph::getPosition(int idGroup) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    if (group.nodes.empty()) {
        return {0, 0};
    }
    int mid = group.nodes.size() / 2;
    if (group.nodes.size() % 2 == 0) {
        return (nodes[group.nodes[mid - 1]].getPosition() + nodes[group.nodes[mid]].getPosition()) / 2.f;
    }
    else {
        return nodes[group.nodes[mid]].getPosition();
    }
}

int BTreeGraph::findEdge(int idGroup, int value) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    for (int i = 0; i < group.nodes.size(); i++) {
        if (nodes[group.nodes[i]].getValue() >= value) {
            return group.listEdge[i];
        }
    }
    return group.listEdge.back();
}

int BTreeGraph::findEdgePos(int idGroup, int value) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    for (int i = 0; i < group.nodes.size(); i++) {
        if (nodes[group.nodes[i]].getValue() >= value) {
            return i;
        }
    }
    return group.nodes.size();
}

sf::Vector2f BTreeGraph::getStartEdgePosition(int idGroup, int pos, sf::Vector2f curPosition) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    auto& group = groups[idGroup];
    if (pos < 0 || pos > group.nodes.size()) {
        assert(false);
    }
    sf::Vector2f diff = curPosition - getPosition(idGroup);
    if (pos == 0) {
        return nodes[group.nodes[0]].getPosition() + sf::Vector2f(-edgeBTree / 2, edgeBTree / 2) + diff;
    }
    else if (pos == group.nodes.size()) {
        return nodes[group.nodes.back()].getPosition() + sf::Vector2f(edgeBTree / 2, edgeBTree / 2) + diff;
    }
    else {
        return (nodes[group.nodes[pos - 1]].getPosition() + nodes[group.nodes[pos]].getPosition()) / 2.f + sf::Vector2f(0, edgeBTree / 2) + diff;
    }
}

sf::Vector2f BTreeGraph::getEndEdgePosition(int idGroup, sf::Vector2f curPosition) {
    if (groups.find(idGroup) == groups.end()) {
        assert(false);
    }
    if (groups[idGroup].nodes.empty()) {
        return {0, 0};
    }
    return curPosition + sf::Vector2f(0, -edgeBTree / 2); 
}

void BTreeGraph::DFS(int id, int height, std::vector <std::vector <std::vector <int> > > &tour) {
    bool flag = false;
    if (groups.find(id) == groups.end()) {
        assert(false);
    }
    for (auto& child : groups[id].listEdge) {
        if (child != -1) {
            if (!flag) {
                tour[height + 1].push_back(std::vector <int>());
                flag = true;
            }
            tour[height + 1].back().push_back(child);
            DFS(child, height + 1, tour);
        }
    }
}

void BTreeGraph::arrangeBTree() {
    if (root == -1) {
        return;
    }
    std::vector <std::vector <std::vector <int> > > tour(groups.size() + 1);
    tour[0].push_back({root});
    DFS(root, 0, tour);
    int leafHeight = -1;
    while (true) {
        if (tour.size() <= leafHeight + 1) {
            break;
        }
        if (tour[leafHeight + 1].empty()) {
            break;
        }
        leafHeight++;
    }
    //init leaf position
    float totalWidth = 0;
    //we divided groups by theirs parents
    for (int i = 0; i < tour[leafHeight].size(); i++) {//for each component of groups having same parent
        for (int j = 0; j < tour[leafHeight][i].size(); j++) {//for each group in am component
            totalWidth += groups[tour[leafHeight][i][j]].getSize().x;
            if (j < tour[leafHeight][i].size() - 1) {
                totalWidth += minHorizontalDistBTree;
            }
        }
        if (i < tour[leafHeight].size() - 1) {
            totalWidth += minHorizontalDistBTree;
        }
    }
    float startX = startPosition.x - totalWidth / 2;
    float startY = startPosition.y + leafHeight * verticalDistBTree;
    for (int i = 0; i < tour[leafHeight].size(); i++) {//for each component of groups having same parent
        for (int j = 0; j < tour[leafHeight][i].size(); j++) {//for each group in am component
            sf::Vector2f here = groups[tour[leafHeight][i][j]].getSize();
            setPosition(tour[leafHeight][i][j], sf::Vector2f(startX + here.x / 2, startY));
            startX += here.x;
            if (j < tour[leafHeight][i].size() - 1) {
                startX += minHorizontalDistBTree;
            }
        }
        if (i < tour[leafHeight].size() - 1) {
            startX += minHorizontalDistBTree;
        }
    }
    for (int h = leafHeight - 1; h >= 0; h--) {
        startY -= verticalDistBTree;
        for (int i = 0; i < tour[h].size(); i++) {
            for (int j = 0; j < tour[h][i].size(); j++) {
                int idGroup = tour[h][i][j];
                if (groups[idGroup].listEdge.empty()) {
                    continue;
                }
                float totalPositionX = 0;
                for (auto& edge : groups[idGroup].listEdge) {
                    if (edge == -1) continue;
                    totalPositionX += getPosition(edge).x;
                }
                setPosition(idGroup, sf::Vector2f(totalPositionX / groups[idGroup].listEdge.size(), startY));
            }
        }
    }
}

BTreeGraph BTreeGraph::execAnimation(std::vector <Animation> animations) {
    // std::cout << "****************\n";
    BTreeGraph tmp = *this;
    std::sort(animations.begin(), animations.end());
    for (int i = 0; i < animations.size(); i++) {
        // std::cout << "animation " << i << " " << animations[i].animationType << " " << animations[i].id1 << " " << animations[i].id2 << " " << animations[i].nextValue << "\n";
        switch (animations[i].animationType) {
            case SetValue: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes[animations[i].id1].setValue(animations[i].nextValue);
                break;
            }
            case SetColorType: {
                if (tmp.groups.find(animations[i].id1) == tmp.groups.end()) {
                    assert(false);
                }
                for (auto& node : tmp.groups[animations[i].id1].nodes) {
                    tmp.nodes[node].setColorType(BTree::ColorType(animations[i].nextValue));
                }
                break;
            }
            case AddNode: {
                tmp.nodes[animations[i].id1] = BTreeNode({0, 0}, animations[i].nextValue, font);
                tmp.nodes[animations[i].id1].setSize(1.f);
                tmp.arrangeBTree();
                break;
            }
            case DeleteNode: {
                if (tmp.nodes.find(animations[i].id1) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.nodes.erase(animations[i].id1);
                tmp.arrangeBTree();
                break;
            }
            case AddGroup: {
                tmp.groups[animations[i].id1] = BTreeGroup(std::vector <int>());
                tmp.arrangeBTree();
                break;
            }
            case DeleteGroup: {
                if (tmp.groups.find(animations[i].id1) == tmp.groups.end()) {
                    assert(false);
                }
                tmp.groups.erase(animations[i].id1);
                tmp.arrangeBTree();
                break;
            }
            case InsertNodeToGroup: {
                if (tmp.groups.find(animations[i].id1) == tmp.groups.end()) {
                    assert(false);
                }
                if (tmp.nodes.find(animations[i].id2) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.insertNode(animations[i].id1, animations[i].id2);
                tmp.arrangeBTree();
                break;
            }
            case DeleteNodeFromGroup: {
                if (tmp.groups.find(animations[i].id1) == tmp.groups.end()) {
                    assert(false);
                }
                if (tmp.nodes.find(animations[i].id2) == tmp.nodes.end()) {
                    assert(false);
                }
                tmp.deleteNode(animations[i].id1, animations[i].id2);
                tmp.arrangeBTree();
                break;
            }
            case SetEdge: {
                // std::cout << "SetEdge " << animations[i].id1 << " " << animations[i].id2 << " " << animations[i].nextValue << "\n";
                // if (tmp.groups.find(animations[i].id1) == tmp.groups.end()) assert(false);
                // if (tmp.groups[animations[i].id1].listEdge.size() <= animations[i].id2) assert(false);
                // if (animations[i].nextValue != -1 && tmp.groups.find(animations[i].nextValue) == tmp.groups.end()) assert(false);
                tmp.groups[animations[i].id1].setEdge(animations[i].id2, animations[i].nextValue);
                tmp.arrangeBTree();
                break;
            }
            case SetRoot: {
                tmp.root = animations[i].nextValue;
                tmp.arrangeBTree();
                break;
            }
        }
    }
    return tmp;
}

void BTreeGraph::draw(sf::RenderWindow& window, ColorTheme theme, sf::Time totalTime, sf::Time timePassed, std::vector<Animation> animations) {
    if (timePassed < epsilonTime) {
        for (auto x = nodes.begin(); x != nodes.end(); x++) {
            x->second.draw(window, theme);
        }
        for (auto x = groups.begin(); x != groups.end(); x++) {
            int idU = x->first;
            sf::Vector2f curPositionU = getPosition(idU);
            for (int i = 0; i < 4; i++) {
                if (x->second.listEdge.size() <= i || x->second.listEdge[i] == -1) continue;
                int idV = x->second.listEdge[i];
                sf::Vector2f curPositionV = getPosition(idV);
                sf::RectangleShape line = getEdgeLine(getStartEdgePosition(idU, i, curPositionU), getEndEdgePosition(idV, curPositionV), 1.0f);
                line.setFillColor(BTree::color[theme][BTree::ColorType::normal].outlineColor);
                window.draw(line);
            }
        }
    }
    float percent = (totalTime < epsilonTime ? 1.f : timePassed / totalTime);
    BTreeGraph tmp = execAnimation(animations);
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
            if (animations[i].animationType == SetColorType) {
                for (int j = 0; j < nodes.size(); j++) {
                    if (getGroupID(j) == animations[i].id1) {
                        Animation animation;
                        animation.animationType = SetColorType;
                        animation.id1 = j;
                        animation.nextValue = animations[i].nextValue;
                        animationMap[j].push_back(animation);
                    }
                }
                continue;
            }
            if (animations[i].animationType == AddGroup) continue;
            if (animations[i].animationType == DeleteGroup) continue;
            if (animations[i].animationType == InsertNodeToGroup) continue;
            if (animations[i].animationType == DeleteNodeFromGroup) continue;
            if (animations[i].animationType == SetRoot) continue;
            if (animations[i].animationType == InsertVariable) continue;
            if (animations[i].animationType == DeleteVariable) continue;
            if (animations[i].animationType == SetEdge) continue;
            animationMap[animations[i].id1].push_back(animations[i]);
        }
    }
    //Edges
    for (auto x = tmp.groups.begin(); x != tmp.groups.end(); x++) {
        int idU = x->first;
        if (groups.find(idU) == groups.end()) {//new node U
            for (int i = 0; i < x->second.listEdge.size(); i++) {
                int idV = x->second.listEdge[i];
                if (idV == -1) continue;
                if (groups.find(idV) == groups.end()) {//new node V
                    sf::Vector2f curPositionU = tmp.getPosition(idU);
                    sf::Vector2f curPositionV = tmp.getPosition(idV);
                    sf::RectangleShape line = getEdgeLine(tmp.getStartEdgePosition(idU, i, curPositionU), tmp.getEndEdgePosition(idV, curPositionV), percent);
                    line.setFillColor(BTree::color[theme][BTree::ColorType::normal].outlineColor);
                    window.draw(line);
                }
                else {//old node V
                    sf::Vector2f curPositionU = tmp.getPosition(idU);
                    sf::Vector2f curPositionV = getPosition(idV) + (tmp.getPosition(idV) - getPosition(idV)) * percent;
                    sf::RectangleShape line = getEdgeLine(tmp.getStartEdgePosition(idU, i, curPositionU), getEndEdgePosition(idV, curPositionV), percent);
                    line.setFillColor(BTree::color[theme][BTree::ColorType::normal].outlineColor);
                    window.draw(line);
                }
            }
        }
        else {//old node U
            for (int i = 0; i < x->second.listEdge.size(); i++) {
                int idV = x->second.listEdge[i];
                if (idV == -1) continue;
                if (groups.find(idV) == groups.end()) {//new node V
                    sf::Vector2f curPositionU = getPosition(idU) + (tmp.getPosition(idU) - getPosition(idU)) * percent;
                    sf::Vector2f curPositionV = tmp.getPosition(idV);
                    sf::RectangleShape line = getEdgeLine(tmp.getStartEdgePosition(idU, i, curPositionU), tmp.getEndEdgePosition(idV, curPositionV), percent);
                    line.setFillColor(BTree::color[theme][BTree::ColorType::normal].outlineColor);
                    window.draw(line);
                }
                else {//old node V
                    sf::Vector2f curPositionU = getPosition(idU) + (tmp.getPosition(idU) - getPosition(idU)) * percent;
                    sf::Vector2f curPositionV = getPosition(idV) + (tmp.getPosition(idV) - getPosition(idV)) * percent;
                    sf::RectangleShape line = getEdgeLine(tmp.getStartEdgePosition(idU, i, curPositionU), getEndEdgePosition(idV, curPositionV), 1.0f);
                    line.setFillColor(BTree::color[theme][BTree::ColorType::normal].outlineColor);
                    window.draw(line);
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