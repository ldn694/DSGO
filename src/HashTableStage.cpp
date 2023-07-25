#include <iostream>
#include "HashTableStage.h"
#include "Template.h"

HashTableStage::HashTableStage(sf::RenderWindow& window, ColorTheme theme) :
	Stage(window, { "Create", "Insert", "Delete", "Search"},
		{
			{"Random", "Empty", "Manual", "Upload File"},
			{"v = ?"},
			{"v = ?"},
			{"v = ?"}
		},
		{
			{
				{},
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
				{{&zeroInt, &maxSizeData}},
				{{&zeroInt, &maxSizeData}, {&zeroInt, &maxValueData}},
				{}
			},
			{
				{{&zeroInt, &maxValueData}}
			},
			{
				{{&zeroInt, &maxValueData}}
			},
			{
				{{&zeroInt, &maxValueData}}
			}
		},
		theme)
{
	setDSName("Hash Table");
	hashList.push_back(HashTable(1, font(fontType::Prototype)));
	size = 1;
}

void HashTableStage::insertValue(int value) {
	resetAnimation();
	int index = value % size;
	std::vector <Animation> animations;
	insertVariable(animations, index, {"index = " + intToString(index)});
	setColorType(animations, index, Hash::ColorType::highlight);
	addAnimationStep(animations, stepTime);
	setAnimatingDirection(Continuous);
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
				
			}
			if (modeString == "Empty") {
				int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					size = v;
					hashList.clear();
					hashList.push_back(HashTable(v, font(fontType::Prototype)));
				}
			}
			if (modeString == "Manual") {
				
			}
			if (modeString == "Upload File") {
				
			}
		}
		if (operationName[curOperation] == "Insert") {
			int v = valueTypingBox[0].getProperInt();
				if (v != -1) {
					resetAnimation();
					insertValue(v);
				}
		}
		if (operationName[curOperation] == "Delete") {
			
		}
		if (operationName[curOperation] == "Search") {
			
		}
		operating = false;
	}
	return { false, theme };
}

void HashTableStage::resetAnimation() {
	animationList.clear();
	curTime = sf::Time::Zero;
	previousStep = -1;
	HashTable hash = hashList.back();
	hashList.clear();
	hashList.push_back(hash);
}

void HashTableStage::addAnimationStep(std::vector <Animation> animations, sf::Time time) {
	animationList.push_back({ animations, time });
	hashList.push_back(hashList.back().execAnimation(animations));
}

void HashTableStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void HashTableStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	if (hashList.size() == 1) {
		hashList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
	}
	else {
		int curStep = getCurStep();
		hashList[curStep].draw(window, theme, animationList[curStep].second, curTime - getPrefixTime(curStep), animationList[curStep].first);
	}
	draw();
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