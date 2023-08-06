#pragma once

#include <SFML/Graphics.hpp>
#include <queue>
#include "Template.h"
#include "Box.h"
#include "TriangleButton.h"
#include "TypingBox.h"
#include "MultipleChoice.h"
#include "BackButton.h"
#include "ReadFromFile.h"
#include "IngameSettings.h"
#include "CodeVisualizer.h"

struct Stage {
protected:
	std::vector <std::string> operationName;
	std::vector <Box> operationBox;
	int curOperation, numOperation;
	bool operationSelecting;
	bool operating;

	sf::Text dsName;

	Box goBox, outerGoBox;

	std::vector <std::vector <std::string> > modeName;
	std::vector <std::vector <Box> > modeBox;
	std::vector <int> numMode;
	int curMode;

	TriangleButton prevModeButton, nextModeButton;
	sf::CircleShape rightwardTriangle;
	bool rightwarding;

	std::vector <std::vector <std::vector <std::string> > > valueName;
	std::vector <std::vector <std::vector <TypingBoxMode> > > typingMode;
	std::vector <std::vector <std::vector <std::pair <int*, int*> > > > valueBound;
	std::vector <BigTypingBox> valueTypingBox;

	std::vector <std::vector <std::vector <std::string> > > codes;
	int highlightLine;

	ReadFromFile readFromFile;
	InputType type;

	std::vector <std::vector <int>> numValue;

	BackButton backButton;
	
	IngameSettings ingameSettings;
	CodeVisualizer codeVisualizer;

	MyShape lightBulb, darkBulb, themeBox;
	ColorTheme theme;
	sf::RenderWindow& window;


	AnimatingDirection animatingDirection;
	sf::Time curTime;
	int previousStep;
	std::vector <AnimationStep> animationList;

	sf::RectangleShape toolBox;

public:
	Stage(sf::RenderWindow &window, std::vector <std::string> operationName, std::vector <std::vector <std::string> > modeName, 
		std::vector <std::vector <std::vector <std::string> > > valueName, 
		std::vector <std::vector <std::vector <TypingBoxMode> > > typingMode,
		std::vector <std::vector <std::vector <std::pair <int*, int*> > > > valueBound,
		std::vector <std::vector <std::vector <std::string> > > codes,
		int* maxSizeData, int* maxValueData, InputType type,
		ColorTheme theme = LightTheme);
	void setDSName(std::string name);
	void updateModeBox(int newMode);
	bool handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y);
	void handleMouseReleased(float x, float y);
	void handleKeyPressed(int key);
	void draw();
	void stageUpdate(sf::Time deltaT);
	void setTheme(ColorTheme newTheme);
	void setAnimatingDirection(AnimatingDirection dir);
	int getCurStep();
	sf::Time getPrefixTime(int step);
	sf::Time getTotalTime();
	void updateCurTime(sf::Time deltaT);
	void setCurTime(sf::Time newTime);

	void insertVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList);
	void deleteVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList);
	void setValue(std::vector <Animation> &animations, int index, int value);
	void setState(std::vector <Animation> &animations, int index, int state);
	void setColorType(std::vector <Animation> &animations, int index, int nextColorType);
	void setRoot(std::vector <Animation> &animations, int value);
	void addNode(std::vector <Animation> &animations, int index, int value);
	void deleteNode(std::vector <Animation> &animations, int index);
	void swapNode(std::vector <Animation> &animations, int index1, int index2);
	void setLeftNode(std::vector <Animation> &animations, int index, int value);
	void setRightNode(std::vector <Animation> &animations, int index, int value);
	void setLeftEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType);
	void setRightEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType);
};