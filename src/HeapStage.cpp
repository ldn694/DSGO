#include <iostream>
#include "HeapStage.h"
#include "Template.h"

HeapStage::HeapStage(sf::RenderWindow& window, ColorTheme theme, bool isMinHeap) :
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
				{{&oneInt, &maxSizeDataHeap}}, 
				{{&zeroInt, &maxValueDataHeap}},
				{}
			},
			{
				{{&zeroInt, &maxValueDataHeap}}
			},
			{
				{{&zeroInt, &maxValueDataHeap}}
			},
			{
				{{&zeroInt, &maxValueDataHeap}}
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
					"A[++len] = v",
					"i = A.length",
					(isMinHeap ? "while ((i > 1) && (A[i] < A[parent(i)])):" : "while ((i > 1) && (A[i] < A[parent(i)]))"),
  					"	swap(A[i], A[parent(i)]); i = parent(i)"
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
		&maxSizeDataHeap, &maxValueDataHeap, ListInput,
		theme)
{
    rootPosition = sf::Vector2f((WIDTH_RES - 2 * widthBox) / 2 + 2 * widthBox, HEIGHT_RES / 4);
	if (isMinHeap) {
		setDSName("Min Heap");
		compare = [](int a, int b) { return a < b; };
		comparisonStr = "<";
		negativeComparisonStr = ">=";
	}
	else {
		setDSName("Max Heap");
		compare = [](int a, int b) { return a > b; };
		comparisonStr = ">";
		negativeComparisonStr = "<=";
	}
	HeapList.push_back(HeapGraph(rootPosition, font(fontType::Arial)));
}

void HeapStage::setDefaultView() {
	std::vector <Animation> animations;
	for (auto x = HeapList.back().nodes.begin(); x != HeapList.back().nodes.end(); x++) {
		int id = x->first;
		std::vector <std::string> variables = HeapList.back().nodes[id].getVariables();
		deleteVariable(animations, id, variables);
		insertVariable(animations, id, { intToString(id) });
		setColorType(animations, id, Heap::ColorType::normal);
		setLeftEdgeColorType(animations, id, Heap::ColorType::normal);
		setRightEdgeColorType(animations, id, Heap::ColorType::normal);
	}
	addAnimationStep(animations, stepTime, -1, "Reformat for visualization");
}

void HeapStage::insertValue(int value) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	int size = HeapList.back().nodes.size();
	std::vector <Animation> animations;
	if (size == maxSizeDataHeap) {
		animations.clear();
		addAnimationStep(animations, stepTime, -1, "Heap reached maximum number of " + intToString(maxSizeDataHeap) + " elements, so no inserting");
		return;
	}
	if (size == 0) {
		animations.clear();
		addNode(animations, 1, value);
		setRoot(animations, 1);
		setColorType(animations, 1, Heap::ColorType::highlight);
		insertVariable(animations, 1, { intToString(1) });
		addAnimationStep(animations, stepTime, 0, "Insert " + intToString(value));

		setDefaultView();

		return;
	}
	int id = size + 1;
	int par = HeapList.back().getParent(id);
	if (id % 2 == 0) {// Left node
		animations.clear();
		addNode(animations, id, value);
		insertVariable(animations, id, { intToString(id) });
		setColorType(animations, id, Heap::ColorType::highlight);
		if (par != -1) {
			setLeftNode(animations, par, id);
		}
		addAnimationStep(animations, stepTime, 0, "Insert " + intToString(value));
	}
	else {//Right node
		animations.clear();
		addNode(animations, id, value);
		insertVariable(animations, id, { intToString(id) });
		setColorType(animations, id, Heap::ColorType::highlight);
		if (par != -1) {
			setRightNode(animations, par, id);
		}
		addAnimationStep(animations, stepTime, 0, "Insert " + intToString(value));
	}
	animations.clear();
	insertVariable(animations, id, {"i = " + intToString(id)});
	addAnimationStep(animations, stepTime, 1, "Set i = " + intToString(id));

	int i = id;
	while (true) {
		if (i == 1) {
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "i == 1, so the loop stops here");

			setDefaultView();
			return;
		}
		animations.clear();
		int par = i / 2;
		setColorType(animations, i, Heap::ColorType::highlight);
		setColorType(animations, par, Heap::ColorType::highlight2);
		addAnimationStep(animations, stepTime, 2, "Comparing A[" + intToString(i) + "] and A[" + intToString(par) + "]");

		if (compare(HeapList.back().nodes[i].value, HeapList.back().nodes[par].value)) {
			animations.clear();
			setValue(animations, i, HeapList.back().nodes[par].value);
			setValue(animations, par, HeapList.back().nodes[i].value);
			setColorType(animations, i, Heap::ColorType::highlight2);
			setColorType(animations, par, Heap::ColorType::highlight);
			deleteVariable(animations, i, {"i = " + intToString(i) });
			insertVariable(animations, par, {"i = " + intToString(par)});
			addAnimationStep(animations, stepTime, 2, "A[" + intToString(i) + "] " + comparisonStr + " A[" + intToString(par) + "], so swap them");

			animations.clear();
			setColorType(animations, i, Heap::ColorType::normal);
			addAnimationStep(animations, stepTime, 3, "Continue loop");

			i = par;
		}
		else {
			animations.clear();
			setColorType(animations, i, Heap::ColorType::normal);
			setColorType(animations, par, Heap::ColorType::normal);
			addAnimationStep(animations, stepTime, 3, intToString(i) + " " + negativeComparisonStr + " " + intToString(par) + ", so stop loop");

			setDefaultView();
			return;
		}
	}
	setDefaultView();
}

void HeapStage::deleteValue(int value) {
	
}

void HeapStage::searchValue(int value) {
	
}

std::pair<bool, ColorTheme> HeapStage::processEvents() {
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
				int num = rand() % maxSizeDataHeap;
				HeapList.clear();
				HeapList.push_back(HeapGraph(rootPosition, font(fontType::Arial)));
				for (int i = 0; i < num; i++) {
					insertValue(rand() % (maxValueDataHeap + 1));
				}
				resetAnimation();
			}
			if (modeString == "Empty") {
				HeapList.clear();
				HeapList.push_back(HeapGraph(rootPosition, font(fontType::Arial)));
				resetAnimation();
			}
			if (modeString == "Fixed Size") {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					HeapList.clear();
					HeapList.push_back(HeapGraph(rootPosition, font(fontType::Arial)));
					while (HeapList.back().nodes.size() < v) {
						insertValue(rand() % (maxValueDataHeap + 1));
					}
					resetAnimation();
				}
			}
			if (modeString == "Manual") {
				std::vector <int> values = valueTypingBox[0].getListInt();
				if (values.empty() || values[0] >= 0) {
					HeapList.clear();
					HeapList.push_back(HeapGraph(rootPosition, font(fontType::Arial)));
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
					HeapList.clear();
					HeapList.push_back(HeapGraph(rootPosition, font(fontType::Arial)));
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

void HeapStage::resetAnimation() {
	animationList.clear();
	curTime = sf::Time::Zero;
	previousStep = UNKOWN;
	HeapGraph tmp = HeapList.back();
	HeapList.clear();
	HeapList.push_back(tmp);
}

void HeapStage::addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description) {
	sort(animations.begin(), animations.end());
	if (!ingameSettings.getSkipAnimation()) {
		animationList.push_back(AnimationStep(animations, time, line, description));
		HeapList.push_back(HeapList.back().execAnimation(animations));
	}
	else {
		HeapGraph tmp = HeapList.back();
		HeapList.clear();
		HeapList.push_back(tmp.execAnimation(animations));
	}
}

void HeapStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void HeapStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	draw();
	if (HeapList.size() == 1) {
		HeapList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
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
			HeapList[curStep].draw(window, theme, animationList[curStep].time, curTime - getPrefixTime(curStep), animationList[curStep].animations);
		}
		else {
			HeapList[curStep].draw(window, theme) ;
		}
	}
	window.display();
}

ColorTheme HeapStage::run() {
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