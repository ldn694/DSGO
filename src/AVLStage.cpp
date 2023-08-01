#include <iostream>
#include "AVLStage.h"
#include "Template.h"

AVLStage::AVLStage(sf::RenderWindow& window, ColorTheme theme) :
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
				{{&oneInt, &maxSizeDataAVL}}, 
				{{&zeroInt, &maxValueDataAVL}},
				{}
			},
			{
				{{&zeroInt, &maxValueDataAVL}}
			},
			{
				{{&zeroInt, &maxValueDataAVL}}
			},
			{
				{{&zeroInt, &maxValueDataAVL}}
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
					"check balance factor of this and its children",
    				"	case L: rotateLeft(this)",
    				"	case R: rotateRight(this)",
    				"	case LR: rotateLeft(this.left); rotateRight(this)",
    				"	case RL: rotateRight(this.right); rotateLeft(this)"
				}
			},
			{
				{
					"int index = value % n, cnt = 0;",
					"while (table[index].state != empty && cnt < n) {",
    				"	if (table[index].state == full && table[index].value == value) {",
        			"		table[index].state = deleted;",
        			"		return;",
    				"	}",
    				"	index = (index + 1) % n; cnt++;",
					"}"
				}
			},
			{
				{
					"int index = value % n, cnt = 0;",
					"while (table[index].state != empty && cnt < n) {",
    				"	if (table[index].state == full && table[index].value == value) {",
        			"		return true;",
    				"	}",
    				"	index = (index + 1) % n; cnt++;",
					"}"
				}
			}
		},
		&maxSizeDataAVL, &maxValueDataAVL, ListInput,
		theme)
{
    rootPosition = sf::Vector2f((WIDTH_RES - 2 * widthBox) / 2 + 2 * widthBox, HEIGHT_RES / 4);
	setDSName("AVL Tree");
	AVLList.push_back(AVLGraph(rootPosition, font(fontType::Prototype)));
	size = 0;
}

void AVLStage::setDefaultView() {
	std::vector <Animation> animations;
	for (int i = 0; i < size; i++) {
		// std::vector <std::string> variables = AVLList.back().nodes[i].getVariables();
		// deleteVariable(animations, i, variables);
		// insertVariable(animations, i, { intToString(i) });
		setColorType(animations, i, AVL::ColorType::normal);
	}
	addAnimationStep(animations, stepTime, -1, "Reformat for visualization");
}

void AVLStage::insertValue(int value) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;

	int id = AVLList.back().getMexID();
	if (size == 0) {
		size++;
		animations.clear();
		addNode(animations, id, value);
		setColorType(animations, id, AVL::ColorType::highlight);
		setRoot(animations, id);
		addAnimationStep(animations, stepTime, 0, "Empty tree, insert root = " + intToString(value));
	}
	else {
		int root = AVLList.back().root;
		while (true) {
			animations.clear();
			setColorType(animations, root, AVL::ColorType::highlight);
			addAnimationStep(animations, stepTime, 0, "Compare " + intToString(value) + " with " + intToString(AVLList.back().nodes[root].value));

			if (AVLList.back().nodes[root].value == value) {
				animations.clear();
				setColorType(animations, root, AVL::ColorType::lowlight);
				addAnimationStep(animations, stepTime, 0, "Value " + intToString(value) + " already exists");

				setDefaultView();
				return;
			}
			else if (AVLList.back().nodes[root].value > value) {
				if (AVLList.back().nodes[root].leftNode == -1) {
					size++;
					animations.clear();
					addNode(animations, id, value);
					setColorType(animations, id, AVL::ColorType::highlight);
					setLeftNode(animations, root, id);
					addAnimationStep(animations, stepTime, 0, "Insert " + intToString(value) + " to the left of " + intToString(AVLList.back().nodes[root].value));
					break;
				}
				else {
					animations.clear();
					setColorType(animations, root, AVL::ColorType::lowlight);
					setColorType(animations, AVLList.back().nodes[root].leftNode, AVL::ColorType::highlight);
					addAnimationStep(animations, stepTime, 0, "Go to the left child of " + intToString(AVLList.back().nodes[root].value));
					root = AVLList.back().nodes[root].leftNode;
				}
			}
			else {
				if (AVLList.back().nodes[root].rightNode == -1) {
					size++;
					animations.clear();
					addNode(animations, id, value);
					setColorType(animations, id, AVL::ColorType::highlight);
					setRightNode(animations, root, id);
					addAnimationStep(animations, stepTime, 0, "Insert " + intToString(value) + " to the right of " + intToString(AVLList.back().nodes[root].value));
					break;
				}
				else {
					animations.clear();
					setColorType(animations, root, AVL::ColorType::lowlight);
					setColorType(animations, AVLList.back().nodes[root].rightNode, AVL::ColorType::highlight);
					addAnimationStep(animations, stepTime, 0, "Go to the right child of " + intToString(AVLList.back().nodes[root].value));
					root = AVLList.back().nodes[root].rightNode;
				}
			}
		}
	}
	setDefaultView();
}

void AVLStage::deleteValue(int value) {
	
}

void AVLStage::searchValue(int value) {
	
}

std::pair<bool, ColorTheme> AVLStage::processEvents() {
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
				int num = rand() % maxSizeDataAVL;
				size = 0;
				AVLList.clear();
				AVLList.push_back(AVLGraph(rootPosition, font(fontType::Prototype)));
				for (int i = 0; i < num; i++) {
					insertValue(rand() % (maxValueDataAVL + 1));
				}
				resetAnimation();
			}
			if (modeString == "Empty") {
				size = 0;
				AVLList.clear();
				AVLList.push_back(AVLGraph(rootPosition, font(fontType::Prototype)));
				resetAnimation();
			}
			if (modeString == "Fixed Size") {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					size = 0;
					AVLList.clear();
					AVLList.push_back(AVLGraph(rootPosition, font(fontType::Prototype)));
					for (int i = 0; i < v; i++) {
						insertValue(rand() % (maxValueDataAVL + 1));
					}
					resetAnimation();
				}
			}
			if (modeString == "Manual") {
				std::vector <int> values = valueTypingBox[0].getListInt();
				if (values.empty() || values[0] != -1) {
					size = 0;
					AVLList.clear();
					AVLList.push_back(AVLGraph(rootPosition, font(fontType::Prototype)));
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
					size = 0;
					AVLList.clear();
					AVLList.push_back(AVLGraph(rootPosition, font(fontType::Prototype)));
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

void AVLStage::resetAnimation() {
	animationList.clear();
	curTime = sf::Time::Zero;
	previousStep = UNKOWN;
	AVLGraph tmp = AVLList.back();
	AVLList.clear();
	AVLList.push_back(tmp);
}

void AVLStage::addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description) {
	sort(animations.begin(), animations.end());
	if (!ingameSettings.getSkipAnimation()) {
		animationList.push_back(AnimationStep(animations, time, line, description));
		AVLList.push_back(AVLList.back().execAnimation(animations));
	}
	else {
		AVLGraph tmp = AVLList.back();
		AVLList.clear();
		AVLList.push_back(tmp.execAnimation(animations));
	}
}

void AVLStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void AVLStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	draw();
	if (AVLList.size() == 1) {
		AVLList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
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
			AVLList[curStep].draw(window, theme, animationList[curStep].time, curTime - getPrefixTime(curStep), animationList[curStep].animations);
		}
		else {
			AVLList[curStep].draw(window, theme) ;
		}
	}
	window.display();
}

ColorTheme AVLStage::run() {
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