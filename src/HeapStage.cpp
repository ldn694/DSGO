#include <iostream>
#include "HeapStage.h"
#include "Template.h"

HeapStage::HeapStage(sf::RenderWindow& window, ColorTheme theme, bool isMinHeap) :
	Stage(window, { "Create", "Insert", "Delete", "Get Top", "Get Size"},
		{
			{"Random", "Empty", "Fixed Size", "Manual", "Upload File"},
			{"v = ?"},
			{"i = ?"},
			{""},
			{""}
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
				{"i ="}
			},
			{
				{}
			},
			{
				{}
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
				{}
			},
			{
				{}
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
				{{&oneInt, &maxValueDataHeap}}
			},
			{
				{}
			},
			{
				{}
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
					"A[i] = A[1]+1;",
					"shiftup(i);",
					"getTop()"
				}
			},
			{
				{
					"get A[1]",
					"swap(A[1], A[A.length]))",
					"i = 1; A.length--",
					"while (i * 2 <= A.length)",
  					(isMinHeap ? "	if A[i] > (L = the smaller of i's children)" : "	if A[i] < (L = the larger of i's children)"),
    				"		swap(A[i], L); i = L's index"
				}
			},
			{
				{
					"return A.length"
				}
			}
		},
		&maxSizeDataHeap, &maxValueDataHeap, ListInput,
		theme), isMinHeap(isMinHeap)
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
		insertVariable(animations, id, { intToString(HeapList.back().heapID[id]) });
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
	int id = HeapList.back().getMexID();
	animations.clear();
	addNode(animations, id, value);
	insertVariable(animations, id, { intToString(HeapList.back().nodes.size() + 1) });
	setColorType(animations, id, Heap::ColorType::highlight);
	addAnimationStep(animations, stepTime, 0, "Insert " + intToString(value));

	animations.clear();
	insertVariable(animations, id, {"i = " + intToString(HeapList.back().nodes.size())});
	addAnimationStep(animations, stepTime, 1, "Set i = " + intToString(HeapList.back().nodes.size()));

	int i = HeapList.back().nodes.size();
	while (true) {
		if (i == 1) {
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "i == 1, so the loop stops here");

			setDefaultView();
			return;
		}
		int par = i / 2;
		int realI = HeapList.back().getRealID(i);
		int realPar = HeapList.back().getRealID(par);
		
		animations.clear();
		setColorType(animations, realPar, AVL::ColorType::lowlight);
		addAnimationStep(animations, stepTime, 2, "Comparing A[" + intToString(i) + "] and A[" + intToString(par) + "]");

		if (compare(HeapList.back().nodes[realI].value, HeapList.back().nodes[realPar].value)) {
			animations.clear();
			swapNode(animations, realI, realPar);
			deleteVariable(animations, realI, {"i = " + intToString(i), intToString(i) });
			deleteVariable(animations, realPar, {intToString(par)});
			insertVariable(animations, realI, {"i = " + intToString(par), intToString(par)});
			insertVariable(animations, realPar, {intToString(i)});
			addAnimationStep(animations, stepTime, 2, "A[" + intToString(i) + "] " + comparisonStr + " A[" + intToString(par) + "], so swap them");
			

			animations.clear();
			setColorType(animations, realPar, Heap::ColorType::normal);
			addAnimationStep(animations, stepTime, 3, "Continue loop");

			i = par;
		}
		else {
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "A[" + intToString(i) + "] " + negativeComparisonStr + " A[" + intToString(par) + "], so stop loop");

			setDefaultView();
			return;
		}
	}
	setDefaultView();
}

void HeapStage::getTop(bool isDelete) {
	if (!isDelete) {
		resetAnimation();
		setAnimatingDirection(Continuous);
	}

	std::vector <Animation> animations;

	if (HeapList.back().nodes.size() == 0) {
		animations.clear();
		addAnimationStep(animations, stepTime, -1, "Heap is empty, so no getting top");
		return;
	}

	if (HeapList.back().nodes.size() == 1) {
		animations.clear();
		setColorType(animations, HeapList.back().getRealID(1), Heap::ColorType::highlight);
		addAnimationStep(animations, stepTime, isDelete ? 2 : 0, "Get top");

		animations.clear();
		int topRealID = HeapList.back().getRealID(1);
		setColorType(animations, topRealID, Heap::ColorType::highlight);
		addAnimationStep(animations, stepTime, isDelete ? 2 : 1, "Swap A[1] and A[length]");

		animations.clear();
		deleteNode(animations, topRealID);
		addAnimationStep(animations, stepTime, isDelete ? 2 : 2, "Delete A[length] (which is previously the top), set i = 1");

		animations.clear();
		addAnimationStep(animations, stepTime, isDelete ? 2 : 3, "Empty heap, so stop loop");

		return;
	}

	animations.clear();
	setColorType(animations, HeapList.back().getRealID(1), Heap::ColorType::highlight);
	addAnimationStep(animations, stepTime, isDelete ? 2 : 0, "Get top");

	animations.clear();
	int topRealID = HeapList.back().getRealID(1);
	int botRealID = HeapList.back().getRealID(HeapList.back().nodes.size());
	setColorType(animations, topRealID, Heap::ColorType::lowlight);
	setColorType(animations, botRealID, Heap::ColorType::highlight);
	swapNode(animations, topRealID, botRealID);
	deleteVariable(animations, topRealID, { intToString(1) });
	deleteVariable(animations, botRealID, { intToString(HeapList.back().nodes.size()) });
	insertVariable(animations, topRealID, { intToString(HeapList.back().nodes.size()) });
	insertVariable(animations, botRealID, { intToString(1) });
	addAnimationStep(animations, stepTime, isDelete ? 2 : 1, "Swap A[1] and A[length]");

	animations.clear();
	deleteNode(animations, topRealID);
	insertVariable(animations, botRealID, { "i = " + intToString(1) });
	addAnimationStep(animations, stepTime, isDelete ? 2 : 2, "Delete A[length] (which is previously the top), set i = 1");

	int i = 1;
	while (true) {
		if (i * 2 > HeapList.back().nodes.size()) {
			animations.clear();
			addAnimationStep(animations, stepTime, isDelete ? 2 : 3, "Reached the leaf, so stop loop");

			setDefaultView();
			return;
		}
		animations.clear();
		setColorType(animations, HeapList.back().getRealID(i), Heap::ColorType::highlight);
		addAnimationStep(animations, stepTime, isDelete ? 2 : 3, "Not the leaf, so continue loop");

		animations.clear();
		int realID = HeapList.back().getRealID(i);
		int leftRealID = HeapList.back().getRealID(i * 2);
		int rightRealID = HeapList.back().getRealID(i * 2 + 1);
		int p = i * 2, realP = leftRealID, other = rightRealID;
		setColorType(animations, leftRealID, Heap::ColorType::lowlight);
		if (rightRealID != -1) {
			realP = compare(HeapList.back().nodes[leftRealID].value, HeapList.back().nodes[rightRealID].value) ? leftRealID : rightRealID;
			p = compare(HeapList.back().nodes[leftRealID].value, HeapList.back().nodes[rightRealID].value) ? i * 2 : i * 2 + 1;
			other = compare(HeapList.back().nodes[leftRealID].value, HeapList.back().nodes[rightRealID].value) ? rightRealID : leftRealID;
			setColorType(animations, rightRealID, Heap::ColorType::lowlight);
		}
		addAnimationStep(animations, stepTime, isDelete ? 2 : 4, "Compare A[" + intToString(i) + "] to A[" + intToString(i * 2) + "] and A[" + intToString(i * 2 + 1) + "], choose the " + (isMinHeap ? "smaller" : "bigger") + " one");

		if (!compare(HeapList.back().nodes[realID].value, HeapList.back().nodes[realP].value)) {
			animations.clear();
			swapNode(animations, realID, realP);
			deleteVariable(animations, realID, { "i = " + intToString(i), intToString(i) });
			deleteVariable(animations, realP, { intToString(p) });
			insertVariable(animations, realID, { "i = " + intToString(p), intToString(p) });
			insertVariable(animations, realP, { intToString(i) });
			if (other != -1) {
				setColorType(animations, other, Heap::ColorType::normal);
			}
			addAnimationStep(animations, stepTime, isDelete ? 2 : 5, "A[" + intToString(i) + "] " + (isMinHeap ? ">" : "<") + " A[" + intToString(p) + "], so swap them");
			i = p;
		}
		else {
			animations.clear();
			if (other != -1) {
				setColorType(animations, other, Heap::ColorType::normal);
			}
			addAnimationStep(animations, stepTime, isDelete ? 2 : 5, "A[" + intToString(i) + "] " + (isMinHeap ? "<=" : ">=") + " A[" + intToString(p) + "], so stop loop");
			
			setDefaultView();
			return;
		}
	}
	setDefaultView();
}

void HeapStage::deleteValue(int hID) {
	resetAnimation();
	setAnimatingDirection(Continuous);

	std::vector <Animation> animations;

	if (hID > HeapList.back().nodes.size()) {
		animations.clear();
		addAnimationStep(animations, stepTime, 0, "Invalid index, so stop here");

		return;
	}

	animations.clear();
	int realID = HeapList.back().getRealID(hID);
	int realTopID = HeapList.back().getRealID(1);
	setValue(animations, realID, HeapList.back().nodes[realTopID].value + 1);
	setColorType(animations, realID, Heap::ColorType::highlight);
	addAnimationStep(animations, stepTime, 0, "Set A[" + intToString(hID) + "] = A[1] + 1");

	animations.clear();
	insertVariable(animations, realID, { intToString(hID) });
	addAnimationStep(animations, stepTime, 1, "Set i = " + intToString(hID));
	int i = hID;
	while (true) {
		if (i == 1) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "Reached the top, so stop shifting");
			break;
		}
		else {
			animations.clear();
			int par = i / 2;
			int realID = HeapList.back().getRealID(i);
			int realPar = HeapList.back().getRealID(par);
			if (compare(HeapList.back().nodes[realID].value, HeapList.back().nodes[realPar].value)) {
				swapNode(animations, realID, realPar);
				deleteVariable(animations, realID, { intToString(i) });
				deleteVariable(animations, realPar, { intToString(par) });
				insertVariable(animations, realID, { intToString(par) });
				insertVariable(animations, realPar, { intToString(i) });
				addAnimationStep(animations, stepTime, 1, "A[" + intToString(i) + "] " + (isMinHeap ? "<" : ">") + " A[" + intToString(par) + "], so swap them");
				i = par;
			}
			else {
				addAnimationStep(animations, stepTime, 1, "A[" + intToString(i) + "] " + (isMinHeap ? ">=" : "<=") + " A[" + intToString(par) + "], so stop shifting");
				break;
			}
		}
	}
	getTop(true);
}

void HeapStage::getSize() {
	resetAnimation();
	setAnimatingDirection(Continuous);

	std::vector <Animation> animations;

	animations.clear();
	addAnimationStep(animations, stepTime, 0, "size = " + intToString(HeapList.back().nodes.size()));
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
		if (operationName[curOperation] == "Get Top") {
			getTop();
		}
		if (operationName[curOperation] == "Get Size") {
			getSize();
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