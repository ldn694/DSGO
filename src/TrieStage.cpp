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
				{"a ="},
				{},
				{"n =", "l =", "a ="},
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
				{singleNumber},
				{},
				{singleNumber, singleNumber, singleNumber},
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
				{{&oneInt, &maxAlphabetSizeTrie}},
				{},
				{{&zeroInt, &maxSizeDataTrie}, {&oneInt, &maxLengthDataTrie}, {&oneInt, &maxAlphabetSizeTrie}},
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
					"cur = root",
					"for (char x : str):",
    				"	if (cur->next[x] == NULL):",
        			"		cur->next[x] = new TrieNode()",
    				"	cur = cur->next[x]",
					"cur->isWord = true"
				}
			},
			{
				{
					"if depth == str.size():",
    				"	if !cur->isWord: return",
					"	cur->isWord = false",
    				"	if cur->next.empty():",
        			"		delete cur",
    				"	return",
					"char x = str[depth]",
					"if (cur->next[x] == NULL): return",
					"erase(cur->next[x], str, depth + 1)",
					"if (!cur->isWord && cur->next.empty()):",
    				"	delete cur"
				}
			},
			{
				{
					"cur = root",
					"for (char x : str):",
    				"	if (cur->next[x] == NULL): return false;",
    				"	cur = cur->next[x]",
					"return cur->isWord"
				}
			}
		},
		&maxSizeDataTrie, &maxLengthDataTrie, ListInput,
		theme)
{
    rootPosition = sf::Vector2f((WIDTH_RES - 2 * widthBox) / 2 + 2 * widthBox, HEIGHT_RES / 6);
	setDSName("Trie Tree");
	TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
}

void TrieStage::setDefaultView() {
	std::vector <Animation> animations;
	for (auto x = TrieList.back().nodes.begin(); x != TrieList.back().nodes.end(); x++) {
		int id = x->first;
		std::vector <std::string> variables = TrieList.back().nodes[id].getVariables();
		deleteVariable(animations, id, variables);
		// insertVariable(animations, i, { intToString(i) });
		if (x->second.getState() == NOTWORD) {
			setColorType(animations, id, Trie::ColorType::normal);
		}
		else {
			setColorType(animations, id, Trie::ColorType::highlight2);
		}
	}
	addAnimationStep(animations, stepTime, -1, "Reformat for visualization");
}

void TrieStage::insertString(std::string str) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;

	int cur = TrieList.back().root;
	animations.clear();
	setColorType(animations, cur, Trie::ColorType::highlight);
	insertVariable(animations, cur, {"cur"});
	addAnimationStep(animations, stepTime, 0, "Start from root");

	for (char x : str) {
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "Inserting " + charToString(x));

		int id;
		if (TrieList.back().findEdge(cur, charToString(x)) == -1) {// no edge
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "cur has no edge " + charToString(x));

			id = TrieList.back().getMexID();
			animations.clear();
			addNode(animations, id, charToString(x));
			setColorType(animations, id, Trie::ColorType::highlight);
			insertEdge(animations, cur, id, charToString(x));
			addAnimationStep(animations, stepTime, 3, "Create new node " + charToString(x));
		}
		else {//have edge
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "cur has edge " + charToString(x));

			id = TrieList.back().findEdge(cur, charToString(x));
		}
		animations.clear();
		setColorType(animations, id, Trie::ColorType::highlight);
		setColorType(animations, cur, Trie::ColorType::lowlight);
		deleteVariable(animations, cur, {"cur"});
		insertVariable(animations, id, {"cur"});
		addAnimationStep(animations, stepTime, 4, "Go to node " + charToString(x));
		cur = id;
	}
	animations.clear();
	setState(animations, cur, ISWORD);
	addAnimationStep(animations, stepTime, 5, "Set cur->isWord = true");

	setDefaultView();
}

void TrieStage::deleteString(std::string str) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;
	
	int cur = TrieList.back().root;
	animations.clear();
	setColorType(animations, cur, Trie::ColorType::highlight);
	insertVariable(animations, cur, {"cur"});
	addAnimationStep(animations, stepTime, 0, "Start from root");

	std::vector <int> path;
	for (int i = 0; i < str.size(); i++) {
		path.push_back(cur);
		char x = str[i];
		animations.clear();
		addAnimationStep(animations, stepTime, 6, "Searching " + charToString(x));

		int id = TrieList.back().findEdge(cur, charToString(x));

		if (id == -1) {// no edge
			animations.clear();
			addAnimationStep(animations, stepTime, 7, "cur has no edge " + charToString(x) + ", no such string found");

			setDefaultView();
			return;
		}
		else {//have edge
			animations.clear();
			addAnimationStep(animations, stepTime, 7, "cur has edge " + charToString(x));

			animations.clear();
			setColorType(animations, id, Trie::ColorType::highlight);
			setColorType(animations, cur, Trie::ColorType::lowlight);
			addAnimationStep(animations, stepTime, 8, "Go to node " + charToString(x));
		}
		cur = id;
	}
	animations.clear();
	addAnimationStep(animations, stepTime, 0, "Traversed to the end of string");
	if (TrieList.back().nodes[cur].getState() == NOTWORD) {
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "cur->isWord = false, no such string found");

		setDefaultView();
		return;
	}
	else {
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "cur->isWord = true, start deleting string");

		animations.clear();
		setState(animations, cur, NOTWORD);
		setColorType(animations, cur, Trie::ColorType::normal); 
		addAnimationStep(animations, stepTime, 2, "Set cur->isWord = false");

		if (TrieList.back().nodes[cur].edges.empty()) {
			animations.clear();
			addAnimationStep(animations, stepTime, 3, "cur has no edge, deleting cur");

			animations.clear();
			deleteEdge(animations, path.back(), cur);
			deleteNode(animations, cur);
			setColorType(animations, path.back(), Trie::ColorType::highlight);
			addAnimationStep(animations, stepTime, 4, "Delete cur, backtracking to parent");
		}
	}
	while (true) {
		cur = path.back();
		path.pop_back();
		if (TrieList.back().nodes[cur].getState() == NOTWORD && TrieList.back().nodes[cur].edges.empty()) {
			animations.clear();
			addAnimationStep(animations, stepTime, 9, "cur has no edge and not a word, deleting cur");

			animations.clear();
			deleteEdge(animations, path.back(), cur);
			deleteNode(animations, cur);
			setColorType(animations, path.back(), Trie::ColorType::highlight);
			addAnimationStep(animations, stepTime, 10, "Delete cur, backtracking to parent");
		}
		else {
			animations.clear();
			addAnimationStep(animations, stepTime, 9, "Deleting completed");

			setDefaultView();
			return;
		}
	}
	assert(false);

}

void TrieStage::searchString(std::string str) {
	resetAnimation();
	setAnimatingDirection(Continuous);
	std::vector <Animation> animations;

	int cur = TrieList.back().root;
	animations.clear();
	setColorType(animations, cur, Trie::ColorType::highlight);
	insertVariable(animations, cur, {"cur"});
	addAnimationStep(animations, stepTime, 0, "Start from root");

	for (char x : str) {
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "Searching " + charToString(x));

		int id;
		if (TrieList.back().findEdge(cur, charToString(x)) == -1) {// no edge
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "cur has no edge " + charToString(x) + ", no such string found");

			setDefaultView();
		}
		else {//have edge
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "cur has edge " + charToString(x));

			id = TrieList.back().findEdge(cur, charToString(x));
		}
		animations.clear();
		setColorType(animations, id, Trie::ColorType::highlight);
		setColorType(animations, cur, Trie::ColorType::lowlight);
		deleteVariable(animations, cur, {"cur"});
		insertVariable(animations, id, {"cur"});
		addAnimationStep(animations, stepTime, 3, "Go to node " + charToString(x));
		cur = id;
	}

	if (TrieList.back().nodes[cur].getState() == ISWORD) {
		animations.clear();
		setColorType(animations, cur, Trie::ColorType::highlight2);
		addAnimationStep(animations, stepTime, 4, "Found string" + str);
	}
	else {
		animations.clear();
		setColorType(animations, cur, Trie::ColorType::highlight);
		addAnimationStep(animations, stepTime, 4, str + " is not found, but it is a prefix of a string in the trie");
	}

	setDefaultView();
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
				int alphabetSize = valueTypingBox[0].getProperInt();
				if (alphabetSize != -1) {
					int num = rand() % (maxSizeDataTrie + 1);
					int len = rand() % maxLengthDataTrie + 1;
					TrieList.clear();
					TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
					for (int i = 0; i < num; i++) {
						std::string tmp;
						int l = rand() % len + 1;
						for (int j = 0; j < l; j++) {
							tmp += char(rand() % alphabetSize + 'a');
						}
						insertString(tmp);
					}
					resetAnimation();	
				}
				
			}
			if (modeString == "Empty") {
				TrieList.clear();
				TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
				resetAnimation();
			}
			if (modeString == "Fixed Size") {
				int num = valueTypingBox[0].getProperInt();
                int len = valueTypingBox[1].getProperInt();
				int alphabetSize = valueTypingBox[2].getProperInt();
				if (num != -1 && len != -1 && alphabetSize != -1) {
                    TrieList.clear();
                    TrieList.push_back(TrieGraph(rootPosition, font(fontType::Arial)));
                    for (int i = 0; i < num; i++) {
                        std::string tmp;
                        int l = rand() % len + 1;
                        for (int j = 0; j < l; j++) {
                            tmp += char(rand() % alphabetSize + 'a');
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
                searchString(str);
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