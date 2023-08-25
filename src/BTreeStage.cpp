#include <iostream>
#include "BTreeStage.h"
#include "Template.h"

BTreeStage::BTreeStage(sf::RenderWindow& window, ColorTheme theme) :
	Stage(window, { "Create", "Insert", "Delete", "Search"},
		{
			{"Random", "Empty", "Fixed Size", "Manual", "Upload File"},
			{"Random", "v = ?"},
			{"v = ?"},
			{"v = ?"}
		},
		{
			{
				{},
				{},
				{"n ="},
				{"v[] ="},
				{}
			},
			{
				{},
				{"v ="}
			},
			{
				{"v ="}
			},
			{
				{"v ="}
			}
		},
		{
			{
				{},
				{},
				{singleNumber},
				{multipleNumber},
				{}
			},
			{
				{},
				{singleNumber},
			},
			{
				{singleNumber}
			},
			{
				{singleNumber}
			}
		},
		{
			{
				{},
				{},
				{{&oneInt, &maxSizeDataBTree}}, 
				{{&zeroInt, &maxValueDataBTree}},
				{}
			},
			{
				{},
				{{&zeroInt, &maxValueDataBTree}}
			},
			{
				{{&zeroInt, &maxValueDataBTree}}
			},
			{
				{{&zeroInt, &maxValueDataBTree}}
			}
		},
		{
			{
				{},
				{},
				{},
				{},
				{}
			},
			{{
					"if (node == NULL):",
					"	insert node",
					"if (node.isFull()):",
					"	split node",
					"search for the node"
				},
				{
					"if (node == NULL):",
					"	insert node",
					"if (node.isFull()):",
					"	split node",
					"search for the node"
				}
			},
			{
				{
					"if (node.isLeaf()):",
					"	if (node.find(k) != NULL): node.delete(k)",
					"	return",
					"if (node.find(k) != NULL):",
					"	find sibling with at least 2 key",
					"	if (sibling != NULL): node.key = sibling.findNearest(k), delete(sibling, node.key)",
					"	else: merge(node), delete(node.left, k)",
					"else:",
					"	nextNode = node.findNextNode(k)",
					"	if (nextNode.numKey == 1):",
					"		find sibling with at least 2 key",
					"		if (sibling != NULL): rotate(node)",
					"		else: merge(node)",
					"	delete(nextNode, k)"
				}
			},
			{
				{
					"if (node == NULL):",
					"	return NULL",
					"else:",
					"	return this",
					"search for the node"
				}
			}
		},
		&maxSizeDataBTree, &maxValueDataBTree, ListInput,
		theme)
{
    rootPosition = sf::Vector2f((WIDTH_RES - 2 * widthBox) / 2 + 2 * widthBox, HEIGHT_RES / 4);
	setDSName("2-3-4 Tree");
	BTreeList.push_back(BTreeGraph(rootPosition, font(fontType::Arial)));
}

void BTreeStage::setDefaultView() {
	std::vector <Animation> animations;
	for (auto x = BTreeList.back().groups.begin(); x != BTreeList.back().groups.end(); x++) {
		int id = x->first;
		// std::vector <std::string> variables = BTreeList.back().nodes[i].getVariables();
		// deleteVariable(animations, i, variables);
		// insertVariable(animations, i, { intToString(i) });
		setColorType(animations, id, BTree::ColorType::normal);
	}
	addAnimationStep(animations, stepTime, -1, "Reformat for visualization");
}

void BTreeStage::insertValue(int value) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	setOperationName("Insert " + intToString(value));

	std::vector <Animation> animations;

	if (BTreeList.back().nodes.empty()) {
		animations.clear();
		int idNode = BTreeList.back().getMexNodeID();
		int idGroup = BTreeList.back().getMexGroupID();
		addNode(animations, idNode, value);
		addGroup(animations, idGroup);
		insertNodeToGroup(animations, idGroup, idNode);
		setColorType(animations, idGroup, BTree::ColorType::highlight);
		setRoot(animations, idGroup);
		addAnimationStep(animations, stepTime, 1, "Tree is empty, init new node");

		setDefaultView();
		return;
	}
	int idGroup = BTreeList.back().root, par = - 1;
	while (true) {
		if (BTreeList.back().groups[idGroup].nodes.size() < 3 && idGroup != par) {
			int nextIDGroup = BTreeList.back().findEdge(idGroup, value);
			if (nextIDGroup == -1) {
				animations.clear();
				int idNode = BTreeList.back().getMexNodeID();
				addNode(animations, idNode, value);
				insertNodeToGroup(animations, idGroup, idNode);
				setColorType(animations, idGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 1, "Insert " + intToString(value) + " to this node");
				break;
			}
			else {
				animations.clear();
				setColorType(animations, idGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 4, "Inserting " + intToString(value));
				animations.clear();
				setColorType(animations, idGroup, BTree::ColorType::lowlight);
				setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 4, "Go to next node");
				par = idGroup;
				idGroup = nextIDGroup;
			}
		}
		else if (par == -1) {
			animations.clear();
			std::vector <int> newGroupID = BTreeList.back().getMexGroupIDs(2);
			int idLeftGroup = newGroupID[0], idRightGroup = newGroupID[1];
			std::vector <int> edges = BTreeList.back().getEdges(idGroup);
			addGroup(animations, idLeftGroup);
			addGroup(animations, idRightGroup);
			deleteNodeFromGroup(animations, idGroup, BTreeList.back().groups[idGroup].nodes[0]);
			deleteNodeFromGroup(animations, idGroup, BTreeList.back().groups[idGroup].nodes[2]);
			insertNodeToGroup(animations, idLeftGroup, BTreeList.back().groups[idGroup].nodes[0]);
			insertNodeToGroup(animations, idRightGroup, BTreeList.back().groups[idGroup].nodes[2]);
			setEdge(animations, idGroup, 0, idLeftGroup);
			setEdge(animations, idGroup, 1, idRightGroup);
			setEdge(animations, idLeftGroup, 0, edges[0]);
			setEdge(animations, idLeftGroup, 1, edges[1]);
			setEdge(animations, idRightGroup, 0, edges[2]);
			setEdge(animations, idRightGroup, 1, edges[3]);
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::highlight);
			setColorType(animations, idRightGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, 3, "Spliting this node");

			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::lowlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::normal);
			setColorType(animations, idRightGroup, BTree::ColorType::normal);
			addAnimationStep(animations, stepTime, 4, "Continuing");
		}
		else if (idGroup != par) {
			animations.clear();
			std::vector <int> newGroupID = BTreeList.back().getMexGroupIDs(2);
			int idLeftGroup = newGroupID[0], idRightGroup = newGroupID[1];
			std::vector <int> edges = BTreeList.back().getEdges(idGroup);
			std::vector <int> parEdges = BTreeList.back().getEdges(par);
			int idEdgePos = BTreeList.back().findEdgePos(par, value);
			addGroup(animations, idLeftGroup);
			addGroup(animations, idRightGroup);
			deleteGroup(animations, idGroup);
			insertNodeToGroup(animations, par, BTreeList.back().groups[idGroup].nodes[1]);
			insertNodeToGroup(animations, idLeftGroup, BTreeList.back().groups[idGroup].nodes[0]);
			insertNodeToGroup(animations, idRightGroup, BTreeList.back().groups[idGroup].nodes[2]);
			parEdges[idEdgePos] = idLeftGroup;
			parEdges.insert(parEdges.begin() + idEdgePos + 1, idRightGroup);
			for (int i = 0; i < parEdges.size(); i++) {
				setEdge(animations, par, i, parEdges[i]);
			}
			setEdge(animations, idLeftGroup, 0, edges[0]);
			setEdge(animations, idLeftGroup, 1, edges[1]);
			setEdge(animations, idRightGroup, 0, edges[2]);
			setEdge(animations, idRightGroup, 1, edges[3]);
			setColorType(animations, par, BTree::ColorType::lowlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::highlight);
			setColorType(animations, idRightGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, 3, "Spliting this node");

			animations.clear();
			setColorType(animations, par, BTree::ColorType::lowlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::normal);
			setColorType(animations, idRightGroup, BTree::ColorType::normal);
			addAnimationStep(animations, stepTime, 4, "Continuing");
			idGroup = par;
		} else {
			int nextIDGroup = BTreeList.back().findEdge(idGroup, value);
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, 4, "Inserting " + intToString(value));
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::lowlight);
			setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, 4, "Go to next node");
			par = idGroup;
			idGroup = nextIDGroup;
		}
	}
	setDefaultView();
}

void BTreeStage::deleteValue(int value) {
	#define tree BTreeList.back()
	resetAnimation();
	setAnimatingDirection(Continuous);
	setOperationName("Delete " + intToString(value));

	std::vector <Animation> animations;

	int preID = -1;
	int idGroup = tree.root;
	if (idGroup == -1) {
		animations.clear();
		addAnimationStep(animations, stepTime, -1, "Tree is empty");
		return;
	}
	int k = value;
	while (true) {
		if (tree.groups[idGroup].isLeaf()) {
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			if (preID != -1) {
				setColorType(animations, preID, BTree::ColorType::lowlight);
			}
			addAnimationStep(animations, stepTime, 0, "Reached a leaf node");

			int curNode = tree.findNode(idGroup, k);
			if (curNode != -1) {
				animations.clear();
				deleteNode(animations, curNode);
				deleteNodeFromGroup(animations, idGroup, curNode);
				if (BTreeList.back().groups[idGroup].nodes.size() == 1) {
					deleteGroup(animations, idGroup);
					if (idGroup == BTreeList.back().root) {
						setRoot(animations, -1);
					}
				}
				addAnimationStep(animations, stepTime, 1, "Found " + intToString(k) + " in this node, so we delete " + intToString(k) + " from this node");
			}
			else {
				animations.clear();
				addAnimationStep(animations, stepTime, 1, "Not found " + intToString(k) + " in this node, no deleting");
			}
			setDefaultView();
			return;
		}
		int curNode = tree.findNode(idGroup, k);
		int pos = tree.findNodePos(idGroup, k);
		if (curNode != -1) {
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			if (preID != -1) {
				setColorType(animations, preID, BTree::ColorType::lowlight);
			}
			addAnimationStep(animations, stepTime, 3, "Found " + intToString(k) + " in this node");

			int leftIDGroup = tree.groups[idGroup].listEdge[pos];
			int rightIDGroup = tree.groups[idGroup].listEdge[pos + 1];

			if (leftIDGroup == -1 || rightIDGroup == -1) {
				std::cout << "Not a leaf but missing edge";
				assert(false);
			}

			if (tree.groups[leftIDGroup].nodes.size() >= 2) {
				animations.clear();
				setColorType(animations, leftIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 4, "Left node has at least 2 keys");

				int idGroupPredecessor = leftIDGroup, preID = -1;
				while (true) {
					if (tree.groups[idGroupPredecessor].isLeaf()) {
						animations.clear();
						setColorType(animations, idGroupPredecessor, BTree::ColorType::highlight2);
						if (preID != -1) {
							setColorType(animations, preID, BTree::ColorType::normal);
						}
						addAnimationStep(animations, stepTime, 5, "Found predecessor");

						int predecessor = tree.nodes[tree.groups[idGroupPredecessor].nodes.back()].getValue();

						animations.clear();
						setColorType(animations, idGroup, BTree::ColorType::highlight2);
						setColorType(animations, idGroupPredecessor, BTree::ColorType::normal);
						setValue(animations, curNode, predecessor);
						addAnimationStep(animations, stepTime, 5, "Replace " + intToString(k) + " with predecessor");

						idGroup = leftIDGroup;
						k = predecessor;
						break;
					}
					else {
						animations.clear();
						setColorType(animations, idGroupPredecessor, BTree::ColorType::highlight);
						if (preID != -1) {
							setColorType(animations, preID, BTree::ColorType::normal);
						}
						addAnimationStep(animations, stepTime, 5, "Searching predecessor");

						preID = idGroupPredecessor;
						idGroupPredecessor = tree.groups[idGroupPredecessor].listEdge.back();
					}
				}
			}
			else if (tree.groups[rightIDGroup].nodes.size() > 1) {
				animations.clear();
				setColorType(animations, rightIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 5, "Right node has at least 2 keys");

				int idGroupSuccessor = rightIDGroup, preID = -1;
				while (true) {
					if (tree.groups[idGroupSuccessor].isLeaf()) {
						animations.clear();
						setColorType(animations, idGroupSuccessor, BTree::ColorType::highlight2);
						if (preID != -1) {
							setColorType(animations, preID, BTree::ColorType::normal);
						}
						addAnimationStep(animations, stepTime, 5, "Found successor");

						int successor = tree.nodes[tree.groups[idGroupSuccessor].nodes[0]].getValue();

						animations.clear();
						setColorType(animations, idGroup, BTree::ColorType::highlight2);
						setColorType(animations, idGroupSuccessor, BTree::ColorType::normal);
						setValue(animations, curNode, successor);
						addAnimationStep(animations, stepTime, 5, "Replace " + intToString(k) + " with successor");

						idGroup = rightIDGroup;
						k = successor;
						break;
					}
					else {
						animations.clear();
						setColorType(animations, idGroupSuccessor, BTree::ColorType::highlight);
						if (preID != -1) {
							setColorType(animations, preID, BTree::ColorType::normal);
						}
						addAnimationStep(animations, stepTime, 5, "Searching successor");

						preID = idGroupSuccessor;
						idGroupSuccessor = tree.groups[idGroupSuccessor].listEdge[0];
					}
				}
			}
			else {
				animations.clear();
				setColorType(animations, leftIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 5, "Both left and right node have 1 key");

				int a = tree.groups[leftIDGroup].nodes[0];
				int b = tree.groups[rightIDGroup].nodes[0];

				std::vector <int> preListEdge;
				for (int i = 0; i <= pos; i++) {
					preListEdge.push_back(tree.groups[idGroup].listEdge[i]);
				}
				for (int i = pos + 2; i < tree.groups[idGroup].listEdge.size(); i++) {
					preListEdge.push_back(tree.groups[idGroup].listEdge[i]);
				}
				animations.clear();
				deleteNodeFromGroup(animations, idGroup, curNode);
				deleteNodeFromGroup(animations, rightIDGroup, b);
				deleteGroup(animations, rightIDGroup);
				insertNodeToGroup(animations, leftIDGroup, curNode);
				insertNodeToGroup(animations, leftIDGroup, b);
				int x = tree.groups[leftIDGroup].listEdge[0];
				int y = tree.groups[leftIDGroup].listEdge[1];
				int z = tree.groups[rightIDGroup].listEdge[0];
				int w = tree.groups[rightIDGroup].listEdge[1];
				setEdge(animations, leftIDGroup, 0, x);
				setEdge(animations, leftIDGroup, 1, y);
				setEdge(animations, leftIDGroup, 2, z);
				setEdge(animations, leftIDGroup, 3, w);
				for (int i = 0; i < preListEdge.size(); i++) {
					setEdge(animations, idGroup, i, preListEdge[i]);
				}
				addAnimationStep(animations, stepTime, 6, "Merging");
				idGroup = leftIDGroup;
			}
		}
		else {
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			if (preID != -1) {
				setColorType(animations, preID, BTree::ColorType::lowlight);
			}
			addAnimationStep(animations, stepTime, 7, "Not found " + intToString(k) + " in this node");

			int nextIDGroup = tree.findEdge(idGroup, k);
			int nextPos = tree.findEdgePos(idGroup, k);
			if (tree.groups[nextIDGroup].nodes.size() != 1) {
				animations.clear();
				setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 9, "Next node has at least 2 keys, so we can continue");

				preID = idGroup;
				idGroup = nextIDGroup;
			}
			else {
				animations.clear();
				setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 9, "Next node has only 1 key");

				bool found = false;
				for (int delta = -1; delta <= 1; delta += 2) {
					int siblingPos = nextPos + delta;
					if (siblingPos < 0 || siblingPos > tree.groups[idGroup].nodes.size()) {
						continue;
					}
					int siblingIDGroup = tree.groups[idGroup].listEdge[siblingPos];
					if (tree.groups[siblingIDGroup].nodes.size() >= 2) {
						animations.clear();
						setColorType(animations, siblingIDGroup, BTree::ColorType::highlight);
						addAnimationStep(animations, stepTime, 10, "Sibling has at least 2 keys");

						int idNodeStolen = tree.groups[siblingIDGroup].nodes[delta == 1 ? 0 : tree.groups[siblingIDGroup].nodes.size() - 1];
						int curNode = tree.groups[idGroup].nodes[delta == -1 ? nextPos - 1 : nextPos];
						std::vector <int> preCurListEdge, preSiblingListEdge, preNextListEdge;
						preCurListEdge = tree.groups[idGroup].listEdge;
						preSiblingListEdge = tree.groups[siblingIDGroup].listEdge;
						preSiblingListEdge.erase(preSiblingListEdge.begin() + (delta == 1 ? 0 : tree.groups[siblingIDGroup].listEdge.size() - 1));
						preNextListEdge = tree.groups[nextIDGroup].listEdge;
						preNextListEdge.insert(preNextListEdge.begin() + (delta == -1 ? 0 : tree.groups[nextIDGroup].listEdge.size()), tree.groups[siblingIDGroup].listEdge[delta == 1 ? 0 : tree.groups[siblingIDGroup].nodes.size()]);

						animations.clear();
						deleteNodeFromGroup(animations, siblingIDGroup, idNodeStolen);
						deleteNodeFromGroup(animations, idGroup, curNode);
						insertNodeToGroup(animations, idGroup, idNodeStolen);
						insertNodeToGroup(animations, nextIDGroup, curNode);
						for (int i = 0; i < preCurListEdge.size(); i++) {
							setEdge(animations, idGroup, i, preCurListEdge[i]);
						}
						for (int i = 0; i < preSiblingListEdge.size(); i++) {
							setEdge(animations, siblingIDGroup, i, preSiblingListEdge[i]);
						}
						for (int i = 0; i < preNextListEdge.size(); i++) {
							setEdge(animations, nextIDGroup, i, preNextListEdge[i]);
						}
						setColorType(animations, siblingIDGroup, BTree::ColorType::normal);
						addAnimationStep(animations, stepTime, 11, "Stealing from sibling");
						found = true;
						break;
					}
				}
				if (!found) {
					animations.clear();
					addAnimationStep(animations, stepTime, 10, "No sibling has at least 2 keys");

					int siblingIDGroup = nextPos == 0 ? tree.groups[idGroup].listEdge[1] : tree.groups[idGroup].listEdge[nextPos - 1];
					int deleteCurEdgePos = nextPos == 0 ? 1 : nextPos - 1;
					int idCurNode = tree.groups[idGroup].nodes[nextPos == 0 ? 0 : nextPos - 1];
					int idSiblingNode = tree.groups[siblingIDGroup].nodes[0];
					std::vector <int> preCurListEdge, preNextListEdge;
					preCurListEdge = tree.groups[idGroup].listEdge;
					preCurListEdge.erase(preCurListEdge.begin() + deleteCurEdgePos);
					int a, b, c, d;
					if (deleteCurEdgePos < nextPos) {
						a = tree.groups[siblingIDGroup].listEdge[0];
						b = tree.groups[siblingIDGroup].listEdge[1];
						c = tree.groups[nextIDGroup].listEdge[0];
						d = tree.groups[nextIDGroup].listEdge[1];
					}
					else {
						a = tree.groups[nextIDGroup].listEdge[0];
						b = tree.groups[nextIDGroup].listEdge[1];
						c = tree.groups[siblingIDGroup].listEdge[0];
						d = tree.groups[siblingIDGroup].listEdge[1];
					}
					preNextListEdge = { a, b, c, d };

					animations.clear();
					deleteNodeFromGroup(animations, idGroup, idCurNode);
					deleteNodeFromGroup(animations, siblingIDGroup, idSiblingNode);
					deleteGroup(animations, siblingIDGroup);
					insertNodeToGroup(animations, nextIDGroup, idSiblingNode);
					insertNodeToGroup(animations, nextIDGroup, idCurNode);
					setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
					if (tree.groups[idGroup].nodes.size() == 1) {
						deleteGroup(animations, idGroup);
						setRoot(animations, nextIDGroup);
						idGroup = -1;
					}
					else {
						for (int i = 0; i < preCurListEdge.size(); i++) {
							setEdge(animations, idGroup, i, preCurListEdge[i]);
						}
					}
					for (int i = 0; i < preNextListEdge.size(); i++) {
						setEdge(animations, nextIDGroup, i, preNextListEdge[i]);
					}
					addAnimationStep(animations, stepTime, 12, "Merging");
				}
				animations.clear();
				setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, 13, "Now we can continue");
				preID = idGroup;
				idGroup = nextIDGroup;
			}
		}
	}
}

void BTreeStage::searchValue(int value) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	setOperationName("Search " + intToString(value));

	std::vector <Animation> animations;

	int idGroup = BTreeList.back().root;
	if (idGroup == -1) {
		animations.clear();
		addAnimationStep(animations, stepTime, -1, "Tree is empty");
		return;
	}
	while (true) {
		if (idGroup == -1) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "Value not found");

			setDefaultView();
			return;
		}
		animations.clear();
		setColorType(animations, idGroup, BTree::ColorType::highlight);
		addAnimationStep(animations, stepTime, 4, "Searching " + intToString(value));

		for (auto x : BTreeList.back().groups[idGroup].nodes) {
			if (BTreeList.back().nodes[x].getValue() == value) {
				animations.clear();
				setColorType(animations, idGroup, BTree::ColorType::highlight2);
				addAnimationStep(animations, stepTime, 3, "Found " + intToString(value));
				
				setDefaultView();
				return;
			}
		}
		int nextIDGroup = BTreeList.back().findEdge(idGroup, value);
		animations.clear();
		setColorType(animations, idGroup, BTree::ColorType::lowlight);
		if (nextIDGroup != -1) {
			setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
		}
		addAnimationStep(animations, stepTime, 4, "Go to next node");
		idGroup = nextIDGroup;
	}
}

std::pair<bool, ColorTheme> BTreeStage::processEvents() {
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			return { true, theme };
			break;
		case sf::Event::MouseButtonPressed:
			if (handleMousePressed(event.mouseButton.x, event.mouseButton.y)) {
				return { true, theme };
			}
			break;
		case sf::Event::KeyPressed:
			handleKeyPressed(int(event.key.code));
			break;
		case sf::Event::MouseMoved:
			handleMouseMove(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonReleased:
			handleMouseReleased(event.mouseButton.x, event.mouseButton.y);
			break;
		}
	}
	if (operating) {
		std::string modeString = modeName[curOperation][curMode];
		if (operationName[curOperation] == "Create") {
			if (modeString == "Random") {
				int num = rand() % maxSizeDataBTree;
				BTreeList.clear();
				BTreeList.push_back(BTreeGraph(rootPosition, font(fontType::Arial)));
				for (int i = 0; i < num; i++) {
					insertValue(rand() % (maxValueDataBTree + 1));
				}
				resetAnimation();
			}
			if (modeString == "Empty") {
				BTreeList.clear();
				BTreeList.push_back(BTreeGraph(rootPosition, font(fontType::Arial)));
				resetAnimation();
			}
			if (modeString == "Fixed Size") {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					BTreeList.clear();
					BTreeList.push_back(BTreeGraph(rootPosition, font(fontType::Arial)));
					while (BTreeList.back().nodes.size() < v) {
						insertValue(rand() % (maxValueDataBTree + 1));
					}
					resetAnimation();
				}
			}
			if (modeString == "Manual") {
				std::vector <int> values = valueTypingBox[0].getListInt();
				if (values.empty() || values[0] >= 0) {
					BTreeList.clear();
					BTreeList.push_back(BTreeGraph(rootPosition, font(fontType::Arial)));
					for (int i = 0; i < values.size(); i++) {
						insertValue(values[i]);
					}
					resetAnimation();
				}
			}
			if (modeString == "Upload File") {
				std::vector <int> values = readFromFile.getListInt();
				if (!values.empty()) {
					if (values[0] < 0) {
						return { false, theme };
					}
					BTreeList.clear();
					BTreeList.push_back(BTreeGraph(rootPosition, font(fontType::Arial)));
					for (int i = 0; i < values.size(); i++) {
						insertValue(values[i]);
					}
					resetAnimation();
				}
			}
		}
		if (operationName[curOperation] == "Insert") {
			if (modeString == "Random") {
				int v = rand() % (maxValueDataBTree + 1);
				insertValue(v);
			}
			else {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					insertValue(v);
				}
			}
		}
		if (operationName[curOperation] == "Delete") {
			int v = valueTypingBox[0].getProperInt();
			if (v != -1) {
				deleteValue(v);
			}
		}
		if (operationName[curOperation] == "Search") {
			int v = valueTypingBox[0].getProperInt();
			if (v != -1) {
				searchValue(v);
			}
		}
		operating = false;
	}
	return { false, theme };
}

void BTreeStage::resetAnimation() {
	setOperationName("");
	animationList.clear();
	curTime = sf::Time::Zero;
	previousStep = UNKOWN;
	BTreeGraph tmp = BTreeList.back();
	BTreeList.clear();
	BTreeList.push_back(tmp);
}

void BTreeStage::addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description) {
	sort(animations.begin(), animations.end());
	if (!ingameSettings.getSkipAnimation()) {
		animationList.push_back(AnimationStep(animations, time, line, description));
		BTreeList.push_back(BTreeList.back().execAnimation(animations));
	}
	else {
		BTreeGraph tmp = BTreeList.back();
		BTreeList.clear();
		BTreeList.push_back(tmp.execAnimation(animations));
	}
}

void BTreeStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void BTreeStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	draw();
	if (BTreeList.size() == 1) {
		BTreeList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
	}
	else {
		int curStep = getCurStep();
		bool exactStep = false;
		if (curStep < 0) {
			curStep = -curStep - 1;
			codeVisualizer.setHighlightLine(animationList[curStep].line);
			codeVisualizer.setDescription(animationList[curStep].description);
		}
		else {
			exactStep = true;
			if (curStep > 0) {
				codeVisualizer.setHighlightLine(animationList[curStep - 1].line);
				codeVisualizer.setDescription(animationList[curStep - 1].description);
			}
			else {
				codeVisualizer.setHighlightLine(-1);
				codeVisualizer.setDescription("");
			}
		}
		if (!exactStep) {
			BTreeList[curStep].draw(window, theme, animationList[curStep].time, curTime - getPrefixTime(curStep), animationList[curStep].animations);
		}
		else {
			BTreeList[curStep].draw(window, theme) ;
		}
	}
	window.display();
}

ColorTheme BTreeStage::run() {
	window.setMouseCursor(arrowCursor);
	while (window.isOpen())
	{
		auto curStatus = processEvents();
		if (curStatus.first) {
			return curStatus.second;
		}
		update(timePerFrame);
		render();
	}
	return theme;
}