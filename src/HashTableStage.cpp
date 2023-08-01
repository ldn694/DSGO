#include <iostream>
#include "HashTableStage.h"
#include "Template.h"

HashTableStage::HashTableStage(sf::RenderWindow& window, ColorTheme theme) :
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
				{"n ="},
				{"n ="},
				{"n =", "v[] ="},
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
				{singleNumber},
				{singleNumber},
				{singleNumber, multipleNumber},
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
				{{&oneInt, &maxSizeDataHash}},
				{{&oneInt, &maxSizeDataHash}}, 
				{{&oneInt, &maxSizeDataHash}, {&zeroInt, &maxValueDataHash}},
				{}
			},
			{
				{{&zeroInt, &maxValueDataHash}}
			},
			{
				{{&zeroInt, &maxValueDataHash}}
			},
			{
				{{&zeroInt, &maxValueDataHash}}
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
					"int id = value % n, cnt = 0;",
					"while (table[id].state == full && cnt < n) {",
					"	id = (id + 1) % n; cnt++;",
					"}",
					"if (cnt == n) return;",
					"table[id].value = {value, full};"
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
		&maxSizeDataHash, &maxValueDataHash, HashTableInput,
		theme)
{
	setDSName("Hash Table");
	hashList.push_back(HashTable(1, font(fontType::Prototype)));
	size = 1;
}

void HashTableStage::setDefaultView() {
	std::vector <Animation> animations;
	for (int i = 0; i < size; i++) {
		std::vector <std::string> variables = hashList.back().cells[i].getVariables();
		deleteVariable(animations, i, variables);
		insertVariable(animations, i, { intToString(i) });
		setColorType(animations, i, Hash::ColorType::normal);
	}
	addAnimationStep(animations, stepTime, -1, "Reformat for visualization");
}

void HashTableStage::insertValue(int value) {
	resetAnimation();

	setAnimatingDirection(Continuous);
	int id = value % size, cnt = 0;
	std::vector <Animation> animations;

	animations.clear();
	deleteVariable(animations, id, {intToString(id)});
	insertVariable(animations, id, {"id = " + intToString(id), "cnt = 0"});
	setColorType(animations, id, Hash::ColorType::highlight);
	addAnimationStep(animations, stepTime, 0, "id = " + intToString(value) + " % " + intToString(size) + " = " + intToString(id) + ", cnt = 0");

	while (true) {
		if (hashList.back().cells[id].state != Hash::State::full) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "table[" + intToString(id) + "].state != full, the condition is false, so we break the loop.");
			break;
		}
		if (cnt >= size) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "cnt = " + intToString(cnt) + " >= n = " + intToString(size) + ", the condition is false, so we break the loop.");
			break;
		}
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "table[" + intToString(id) + "].state == full and cnt = " + intToString(cnt) + " < n = " + intToString(size) + ", the condition is true, so we continue the loop.");

		animations.clear();
		int nextId = (id + 1) % size;
		int nextCnt = cnt + 1;
		if (id != nextId) {
			setColorType(animations, id, Hash::ColorType::lowlight);
			deleteVariable(animations, id, {"id = " + intToString(id), "cnt = " + intToString(cnt)});
			insertVariable(animations, id, {intToString(id)});
			deleteVariable(animations, nextId, {intToString(nextId)});
			insertVariable(animations, nextId, {"id = " + intToString(nextId), "cnt = " + intToString(nextCnt)});
		}
		else {
			deleteVariable(animations, id, {"id = " + intToString(id), "cnt = " + intToString(cnt)});
			insertVariable(animations, nextId, {"id = " + intToString(nextId), "cnt = " + intToString(nextCnt)});
		}
		setColorType(animations, nextId, Hash::ColorType::highlight);
		addAnimationStep(animations, stepTime, 2, "id = (id + 1) % n = (" + intToString(id) + " + 1) % " + intToString(size) + " = " + intToString(nextId) + ", cnt = " + intToString(nextCnt));
		id = nextId;
		cnt = nextCnt;
	}
	if (cnt == size) {
		animations.clear();
		addAnimationStep(animations, stepTime, 4, "cnt == n == " + intToString(size) + ", the condition is true, so we stop the function.");

		setDefaultView();
		return;
	}
	animations.clear();
	setValue(animations, id, value);
	setState(animations, id, Hash::State::full);
	setColorType(animations, id, Hash::ColorType::highlight);
	addAnimationStep(animations, stepTime, 5, "table[" + intToString(id) + "] = {" + intToString(value) + ", full}");

	setDefaultView();
}

void HashTableStage::deleteValue(int value) {
	resetAnimation();

	setAnimatingDirection(Continuous);
	int id = value % size, cnt = 0;
	std::vector <Animation> animations;

	animations.clear();
	deleteVariable(animations, id, {intToString(id)});
	insertVariable(animations, id, {"id = " + intToString(id), "cnt = 0"});
	setColorType(animations, id, Hash::ColorType::highlight);
	addAnimationStep(animations, stepTime, 0, "id = " + intToString(value) + " % " + intToString(size) + " = " + intToString(id) + ", cnt = 0");

	while (true) {
		if (hashList.back().cells[id].state == Hash::State::empty) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "table[" + intToString(id) + "].state == empty, the condition is false, so we stop the function here as " + intToString(value) + " is not in the table.");
			
			setDefaultView();
			return;
		}
		if (cnt >= size) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "cnt = " + intToString(cnt) + " >= n = " + intToString(size) + ", the condition is false, so we break the loop.");
			
			setDefaultView();
			return;
		}
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "table[" + intToString(id) + "].state != empty and cnt = " + intToString(cnt) + " < n = " + intToString(size) + ", the condition is true, so we continue the loop.");

		if (hashList.back().cells[id].state == Hash::State::full && hashList.back().cells[id].value == value) {
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "table[" + intToString(id) + "].state == full and table[" + intToString(id) + "].value == " + intToString(value) + ", the condition is true.");

			animations.clear();
			setState(animations, id, Hash::State::deleted);
			setColorType(animations, id, Hash::ColorType::lowlight);
			addAnimationStep(animations, stepTime, 3, "Delete " + intToString(value) + " at table[" + intToString(id) + "].");

			setDefaultView();
			return;
		}

		animations.clear();
		int nextId = (id + 1) % size;
		int nextCnt = cnt + 1;
		if (id != nextId) {
			setColorType(animations, id, Hash::ColorType::lowlight);
			setColorType(animations, nextId, Hash::ColorType::highlight);
			deleteVariable(animations, id, {"id = " + intToString(id), "cnt = " + intToString(cnt)});
			insertVariable(animations, id, {intToString(id)});
			deleteVariable(animations, nextId, {intToString(nextId)});
			insertVariable(animations, nextId, {"id = " + intToString(nextId), "cnt = " + intToString(nextCnt)});
		}
		else {
			deleteVariable(animations, id, {"id = " + intToString(id), "cnt = " + intToString(cnt)});
			insertVariable(animations, nextId, {"id = " + intToString(nextId), "cnt = " + intToString(nextCnt)});
		}
		addAnimationStep(animations, stepTime, 6, "id = (" + intToString(id) + " + 1) % " + intToString(size) + " = " + intToString(nextId) + ", cnt = " + intToString(nextCnt));
		id = nextId;
		cnt = nextCnt;
	}
}

void HashTableStage::searchValue(int value) {
	resetAnimation();

	setAnimatingDirection(Continuous);
	int id = value % size, cnt = 0;
	std::vector <Animation> animations;

	animations.clear();
	deleteVariable(animations, id, {intToString(id)});
	insertVariable(animations, id, {"id = " + intToString(id), "cnt = 0"});
	setColorType(animations, id, Hash::ColorType::highlight);
	addAnimationStep(animations, stepTime, 0, "id = " + intToString(value) + " % " + intToString(size) + " = " + intToString(id) + ", cnt = 0");

	while (true) {
		if (hashList.back().cells[id].state == Hash::State::empty) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "table[" + intToString(id) + "].state == empty, the condition is false, so we stop the function here as " + intToString(value) + " is not in the table.");
			
			setDefaultView();
			return;
		}
		if (cnt >= size) {
			animations.clear();
			addAnimationStep(animations, stepTime, 1, "cnt = " + intToString(cnt) + " >= n = " + intToString(size) + ", the condition is false, so we break the loop.");
			
			setDefaultView();
			return;
		}
		animations.clear();
		addAnimationStep(animations, stepTime, 1, "table[" + intToString(id) + "].state != empty and cnt = " + intToString(cnt) + " < n = " + intToString(size) + ", the condition is true, so we continue the loop.");

		if (hashList.back().cells[id].state == Hash::State::full && hashList.back().cells[id].value == value) {
			animations.clear();
			addAnimationStep(animations, stepTime, 2, "table[" + intToString(id) + "].state == full and table[" + intToString(id) + "].value == " + intToString(value) + ", the condition is true.");

			animations.clear();
			setColorType(animations, id, Hash::ColorType::highlight2);
			addAnimationStep(animations, stepTime, 3, intToString(value) + " is found at table[" + intToString(id) + "].");

			setDefaultView();
			return;
		}

		animations.clear();
		int nextId = (id + 1) % size;
		int nextCnt = cnt + 1;
		if (id != nextId) {
			setColorType(animations, id, Hash::ColorType::lowlight);
			setColorType(animations, nextId, Hash::ColorType::highlight);
			deleteVariable(animations, id, {"id = " + intToString(id), "cnt = " + intToString(cnt)});
			insertVariable(animations, id, {intToString(id)});
			deleteVariable(animations, nextId, {intToString(nextId)});
			insertVariable(animations, nextId, {"id = " + intToString(nextId), "cnt = " + intToString(nextCnt)});
		}
		else {
			deleteVariable(animations, id, {"id = " + intToString(id), "cnt = " + intToString(cnt)});
			insertVariable(animations, nextId, {"id = " + intToString(nextId), "cnt = " + intToString(nextCnt)});
		}
		addAnimationStep(animations, stepTime, 5, "id = (" + intToString(id) + " + 1) % " + intToString(size) + " = " + intToString(nextId) + ", cnt = " + intToString(nextCnt));
		id = nextId;
		cnt = nextCnt;
	}
}

std::pair<bool, ColorTheme> HashTableStage::processEvents() {
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
				size = rand() % maxSizeDataHash + 1;
				int smallSize = rand() % (size + 1);
				hashList.clear();
				hashList.push_back(HashTable(size, font(fontType::Prototype)));
				for (int i = 0; i < smallSize; i++) {
					insertValue(rand() % (maxValueDataHash + 1));
				}
				resetAnimation();
			}
			if (modeString == "Empty") {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					size = v;
					hashList.clear();
					hashList.push_back(HashTable(v, font(fontType::Prototype)));
				}
			}
			if (modeString == "Fixed Size") {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					size = v;
					hashList.clear();
					hashList.push_back(HashTable(v, font(fontType::Prototype)));
					int smallSize = rand() % (size + 1);
					for (int i = 0; i < smallSize; i++) {
						insertValue(rand() % (maxValueDataHash + 1));
					}
					resetAnimation();
				}
			}
			if (modeString == "Manual") {
				int num = valueTypingBox[0].getProperInt();
				std::vector <int> values = valueTypingBox[1].getListInt();
				if (num != -1 && (values.empty() || values[0] != -1)) {
					size = num;
					hashList.clear();
					hashList.push_back(HashTable(num, font(fontType::Prototype)));
					for (int i = 0; i < values.size(); i++) {
						insertValue(values[i]);
					}
					resetAnimation();
				}
			}
			if (modeString == "Upload File") {
				std::vector <int> values = readFromFile.getListInt();
				if (!values.empty()) {
					if (values[0] <= 0 || values.size() - 1 > std::min(values[0], maxSizeDataHash)) {
						return { false, theme };
					}
					size = values[0];
					hashList.clear();
					hashList.push_back(HashTable(size, font(fontType::Prototype)));
					for (int i = 1; i < values.size(); i++) {
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

void HashTableStage::resetAnimation() {
	animationList.clear();
	curTime = sf::Time::Zero;
	previousStep = UNKOWN;
	HashTable hash = hashList.back();
	hashList.clear();
	hashList.push_back(hash);
}

void HashTableStage::addAnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description) {
	if (!ingameSettings.getSkipAnimation()) {
		animationList.push_back(AnimationStep(animations, time, line, description));
		hashList.push_back(hashList.back().execAnimation(animations));
	}
	else {
		HashTable tmp = hashList.back();
		hashList.clear();
		hashList.push_back(tmp.execAnimation(animations));
	}
}

void HashTableStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void HashTableStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	draw();
	if (hashList.size() == 1) {
		hashList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
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
			hashList[curStep].draw(window, theme, animationList[curStep].time, curTime - getPrefixTime(curStep), animationList[curStep].animations);
		}
		else {
			hashList[curStep].draw(window, theme);
		}
	}
	window.display();
}

ColorTheme HashTableStage::run() {
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