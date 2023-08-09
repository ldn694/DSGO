#include <iostream>
#include "BTreeStage.h"
#include "Template.h"

BTreeStage::BTreeStage(sf::RenderWindow& window, ColorTheme theme) :
	Stage(window, { "Create", "Insert", "Delete", "Search"},
		{
			{"Random", "Empty", "Fixed Size", "Manual", "Upload File"},
			{"v = ?"},
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
			{
				{
					"insert value",
					"check balance factor of this node",
    				"	case LL: rotateRight(this)",
    				"	case RR: rotateLeft(this)",
    				"	case LR: rotateLeft(this.left); rotateRight(this)",
    				"	case RL: rotateRight(this.right); rotateLeft(this)",
					"	case OK: continue"
				}
			},
			{
				{
					"delete value",
					"check balance factor of this node",
    				"	case LL: rotateRight(this)",
    				"	case RR: rotateLeft(this)",
    				"	case LR: rotateLeft(this.left); rotateRight(this)",
    				"	case RL: rotateRight(this.right); rotateLeft(this)",
					"	case OK: continue"
				}
			},
			{
				{
					"if this == null: return null",
					"else if this.key == search value:return this",
					"else if this.key < search value: search this.right",
					"else search this.left"
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
		addAnimationStep(animations, stepTime, -1, "Tree is empty, init new node");

		setDefaultView();
		return;
	}
	int idGroup = BTreeList.back().root, par = - 1;
	while (true) {
		BTreeGraph& graph = BTreeList.back();
		if (graph.groups[idGroup].nodes.size() < 3 && idGroup != par) {
			int nextIDGroup = graph.findEdge(idGroup, value);
			if (nextIDGroup == -1) {
				animations.clear();
				int idNode = graph.getMexNodeID();
				addNode(animations, idNode, value);
				insertNodeToGroup(animations, idGroup, idNode);
				setColorType(animations, idGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, -1, "Insert " + intToString(value) + " to this node");
				break;
			}
			else {
				animations.clear();
				setColorType(animations, idGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, -1, "Inserting " + intToString(value));
				animations.clear();
				setColorType(animations, idGroup, BTree::ColorType::lowlight);
				setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
				addAnimationStep(animations, stepTime, -1, "Go to next node");
				par = idGroup;
				idGroup = nextIDGroup;
			}
		}
		else if (par == -1) {
			animations.clear();
			std::vector <int> newGroupID = graph.getMexGroupIDs(2);
			int idLeftGroup = newGroupID[0], idRightGroup = newGroupID[1];
			std::vector <int> edges = graph.getEdges(idGroup);
			addGroup(animations, idLeftGroup);
			addGroup(animations, idRightGroup);
			deleteNodeFromGroup(animations, idGroup, graph.groups[idGroup].nodes[0]);
			deleteNodeFromGroup(animations, idGroup, graph.groups[idGroup].nodes[2]);
			insertNodeToGroup(animations, idLeftGroup, graph.groups[idGroup].nodes[0]);
			insertNodeToGroup(animations, idRightGroup, graph.groups[idGroup].nodes[2]);
			setEdge(animations, idGroup, 0, idLeftGroup);
			setEdge(animations, idGroup, 1, idRightGroup);
			setEdge(animations, idLeftGroup, 0, edges[0]);
			setEdge(animations, idLeftGroup, 1, edges[1]);
			setEdge(animations, idRightGroup, 0, edges[2]);
			setEdge(animations, idRightGroup, 1, edges[3]);
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::highlight);
			setColorType(animations, idRightGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, -1, "Spliting this node");

			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::lowlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::normal);
			setColorType(animations, idRightGroup, BTree::ColorType::normal);
			addAnimationStep(animations, stepTime, -1, "Continuing");
		}
		else if (idGroup != par) {
			animations.clear();
			std::vector <int> newGroupID = graph.getMexGroupIDs(2);
			int idLeftGroup = newGroupID[0], idRightGroup = newGroupID[1];
			std::vector <int> edges = graph.getEdges(idGroup);
			std::vector <int> parEdges = graph.getEdges(par);
			int idEdgePos = graph.findEdgePos(par, value);
			addGroup(animations, idLeftGroup);
			addGroup(animations, idRightGroup);
			deleteGroup(animations, idGroup);
			insertNodeToGroup(animations, par, graph.groups[idGroup].nodes[1]);
			insertNodeToGroup(animations, idLeftGroup, graph.groups[idGroup].nodes[0]);
			insertNodeToGroup(animations, idRightGroup, graph.groups[idGroup].nodes[2]);
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
			addAnimationStep(animations, stepTime, -1, "Spliting this node");

			animations.clear();
			setColorType(animations, par, BTree::ColorType::lowlight);
			setColorType(animations, idLeftGroup, BTree::ColorType::normal);
			setColorType(animations, idRightGroup, BTree::ColorType::normal);
			addAnimationStep(animations, stepTime, -1, "Continuing");
			idGroup = par;
		} else {
			int nextIDGroup = graph.findEdge(idGroup, value);
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, -1, "Inserting " + intToString(value));
			animations.clear();
			setColorType(animations, idGroup, BTree::ColorType::lowlight);
			setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
			addAnimationStep(animations, stepTime, -1, "Go to next node");
			par = idGroup;
			idGroup = nextIDGroup;
		}
	}
	setDefaultView();
}

void BTreeStage::deleteValue(int value) {
	
}

void BTreeStage::searchValue(int value) {
	resetAnimation();
	setAnimatingDirection(Continuous);

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
			addAnimationStep(animations, stepTime, -1, "Value not found");

			setDefaultView();
			return;
		}
		animations.clear();
		setColorType(animations, idGroup, BTree::ColorType::highlight);
		addAnimationStep(animations, stepTime, -1, "Searching " + intToString(value));

		BTreeGraph& graph = BTreeList.back();
		for (auto x : graph.groups[idGroup].nodes) {
			if (graph.nodes[x].getValue() == value) {
				animations.clear();
				setColorType(animations, idGroup, BTree::ColorType::highlight2);
				addAnimationStep(animations, stepTime, -1, "Found " + intToString(value));
				
				setDefaultView();
				return;
			}
		}
		int nextIDGroup = graph.findEdge(idGroup, value);
		animations.clear();
		setColorType(animations, idGroup, BTree::ColorType::lowlight);
		if (nextIDGroup != -1) {
			setColorType(animations, nextIDGroup, BTree::ColorType::highlight);
		}
		addAnimationStep(animations, stepTime, -1, "Go to next node");
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
			int v = valueTypingBox[0].getProperInt();
			if (v != -1) {
				insertValue(v);
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