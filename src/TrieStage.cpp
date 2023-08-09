#include <iostream>
#include "TrieStage.h"
#include "Template.h"

TrieStage::TrieStage(sf::RenderWindow& window, ColorTheme theme) :
	Stage(window, { "Create", "Insert", "Delete", "Search"},
		{
			{"Random", "Empty", "Fixed Size", "Upload File"},
			{"s = ?"},
			{"s = ?"},
			{"s = ?"}
		},
		{
			{
				{},
				{},
				{"n =", "l ="},
				{}
			},
			{
				{"s ="}
			},
			{
				{"s ="}
			},
			{
				{"s ="}
			}
		},
		{
			{
				{},
				{},
				{singleNumber, singleNumber},
				{}
			},
			{
				{string},
			},
			{
				{string}
			},
			{
				{string}
			}
		},
		{
			{
				{},
				{},
				{{&zeroInt, &maxSizeDataTrie}, {&oneInt, &maxSizeDataTrie}}, 
				{}
			},
			{
				{{&zeroInt, &maxLengthDataTrie}}
			},
			{
				{{&zeroInt, &maxLengthDataTrie}}
			},
			{
				{{&zeroInt, &maxLengthDataTrie}}
			}
		},
		{
			{
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
		&maxSizeDataTrie, &maxLengthDataTrie, ListInput,
		theme)
{
    rootPosition = sf::Vector2f((WIDTH_RES - 2 * widthBox) / 2 + 2 * widthBox, HEIGHT_RES / 4);
	setDSName("Trie Tree");
	TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
}

void TrieStage::setDefaultView() {
	std::vector <Animation> animations;
	for (auto x = TrieList.back().nodes.begin(); x != TrieList.back().nodes.end(); x++) {
		int id = x->first;
		// std::vector <std::string> variables = TrieList.back().nodes[i].getVariables();
		// deleteVariable(animations, i, variables);
		// insertVariable(animations, i, { intToString(i) });
		setColorType(animations, id, Trie::ColorType::normal);
		setLeftEdgeColorType(animations, id, Trie::ColorType::normal);
		setRightEdgeColorType(animations, id, Trie::ColorType::normal);
	}
	addAnimationStep(animations, stepTime, -1, "Reformat for visualization");
}

void TrieStage::insertString(std::string str) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;
	
}

void TrieStage::deleteString(std::string str) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;
	
}

void TrieStage::searchString(std::string str) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;
	
}

std::pair<bool, ColorTheme> TrieStage::processEvents() {
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
				int num = rand() % maxSizeDataTrie;
                int len = rand() % maxLengthDataTrie;
				TrieList.clear();
				TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
				for (int i = 0; i < num; i++) {
					std::string tmp;
                    int l = rand() % len + 1;
                    for (int j = 0; j < l; j++) {
                        tmp += char(rand() % 26 + 'a');
                    }
                    insertString(tmp);
				}
				resetAnimation();
			}
			if (modeString == "Empty") {
				TrieList.clear();
				TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
				resetAnimation();
			}
			if (modeString == "Fixed Size") {
				int num = valueTypingBox[0].getProperInt();
                int len = valueTypingBox[1].getProperInt();
				if (num != -1 && len != -1) {
                    TrieList.clear();
                    TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
                    for (int i = 0; i < num; i++) {
                        std::string tmp;
                        int l = rand() % len + 1;
                        for (int j = 0; j < l; j++) {
                            tmp += char(rand() % 26 + 'a');
                        }
                        insertString(tmp);
                    }
                    resetAnimation();
                }
			}
			if (modeString == "Manual") {
				
			}
			if (modeString == "Upload File") {
				
			}
		}
		if (operationName[curOperation] == "Insert") {
            std::string str = valueTypingBox[0].getText();
			if (!str.empty()) {
                insertString(str);
            }
		}
		if (operationName[curOperation] == "Delete") {
			std::string str = valueTypingBox[0].getText();
			if (!str.empty()) {
                deleteString(str);
            }
		}
		if (operationName[curOperation] == "Search") {
			std::string str = valueTypingBox[0].getText();
			if (!str.empty()) {
                deleteString(str);
            }
		}
		operating = false;
	}
	return { false, theme };
}

void TrieStage::resetAnimation() {
	animationList.clear();
	curTime = sf::Time::Zero;
	previousStep = UNKOWN;
	TrieGraph tmp = TrieList.back();
	TrieList.clear();
	TrieList.push_back(tmp);
}

void TrieStage::addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description) {
	sort(animations.begin(), animations.end());
	if (!ingameSettings.getSkipAnimation()) {
		animationList.push_back(AnimationStep(animations, time, line, description));
		TrieList.push_back(TrieList.back().execAnimation(animations));
	}
	else {
		TrieGraph tmp = TrieList.back();
		TrieList.clear();
		TrieList.push_back(tmp.execAnimation(animations));
	}
}

void TrieStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void TrieStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	draw();
	if (TrieList.size() == 1) {
		TrieList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
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
			TrieList[curStep].draw(window, theme, animationList[curStep].time, curTime - getPrefixTime(curStep), animationList[curStep].animations);
		}
		else {
			TrieList[curStep].draw(window, theme) ;
		}
	}
	window.display();
}

ColorTheme TrieStage::run() {
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