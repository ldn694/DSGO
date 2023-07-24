#include <iostream>
#include "HashTableStage.h"
#include "Template.h"

HashTableStage::HashTableStage(sf::RenderWindow& window, ColorTheme theme) :
	Stage(window, { "Create", "Insert", "Delete", "Search", "Update", "Access"},
		{
			{"Empty", "Random", "Random Sorted", "Fixed Size", "Manual", "Upload File"},
			{"i = 0 (head)", "i = n (after tail)", "i = 1..n - 1 (middle)"},
			{"i = 0 (head)", "i = n - 1 (tail)", "i = 1..n - 2 (middle)"},
			{"v = ?"},
			{"i = ?, v = ?"},
			{"i = ?"}
		},
		{
			{
				{},
				{},
				{},
				{"n ="},
				{"v[] ="},
				{}
			},
			{
				{"v ="},
				{"v ="},
				{"i =", "v ="}
			},
			{
				{},
				{},
				{"i ="}
			},
			{
				{"v ="}
			},
			{
				{"i =", "v ="}
			},
			{
				{"i ="}
			}
		},
		{
			{
				{},
				{},
				{},
				{singleNumber},
				{multipleNumber},
				{}
			},
			{
				{singleNumber},
				{singleNumber},
				{singleNumber, singleNumber}
			},
			{
				{},
				{},
				{singleNumber}
			},
			{
				{singleNumber}
			},
			{
				{singleNumber, singleNumber}
			},
			{
				{singleNumber}
			}
		},
		{
			{
				{},
				{},
				{},
				{{0, maxSizeData}},
				{{0, maxValueData}},
				{}
			},
			{
				{{0, maxValueData}},
				{{0, maxValueData}},
				{{0, maxSizeData}, {0, maxValueData}}
			},
			{
				{},
				{},
				{{0, maxSizeData}}
			},
			{
				{{0, maxValueData}}
			},
			{
				{{0, maxSizeData}, {0, maxValueData}}
			},
			{
				{{0, maxSizeData}}
			}
		},
		theme),
    hash(1, font(fontType::Consolas))
{
	setDSName("Hash Table");
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
			if (modeString == "Empty") {
				
			}
			if (modeString == "Random") {
				
			}
			if (modeString == "Random Sorted") {
				
			}
			if (modeString == "Fixed Size") {
				
			}
			if (modeString == "Manual") {
				
			}
			if (modeString == "Upload File") {
				
			}
		}
		if (operationName[curOperation] == "Insert") {
			if (modeString == "i = 0 (head)") {
				
			}
			if (modeString == "i = n (after tail)") {
				
			}
			if (modeString == "i = 1..n - 1 (middle)") {
				
			}
		}
		if (operationName[curOperation] == "Delete") {
			if (modeString == "i = 0 (head)") {
				
			}
			if (modeString == "i = n - 1 (tail)") {
				
			}
			if (modeString == "i = 1..n - 2 (middle)") {
				
			}
		}
		if (operationName[curOperation] == "Search") {
			
		}
		if (operationName[curOperation] == "Update") {
			if (modeString == "i = ?, v = ?") {
				
			}
		}
		if (operationName[curOperation] == "Access") {
			if (modeString == "i = ?") {
				
			}
		}
		operating = false;
	}
	return { false, theme };
}

void HashTableStage::update(sf::Time deltaT) {
	stageUpdate(deltaT);
}

void HashTableStage::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
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