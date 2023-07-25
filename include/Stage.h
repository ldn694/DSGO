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
	sf::CircleShape upwardTriangle;
	bool upwarding;

	std::vector <std::vector <std::vector <std::string> > > valueName;
	std::vector <std::vector <std::vector <TypingBoxMode> > > typingMode;
	std::vector <std::vector <std::vector <std::pair <int, int> > > > valueBound;
	std::vector <BigTypingBox> valueTypingBox;

	ReadFromFile readFromFile;

	std::vector <std::vector <int>> numValue;

	BackButton backButton;
	
	IngameSettings ingameSettings;

	MyShape lightBulb, darkBulb, themeBox;
	ColorTheme theme;
	sf::RenderWindow& window;


	AnimatingDirection animatingDirection;
	sf::Time curTime;
	int previousStep;
	std::vector <std::pair<std::vector <Animation>, sf::Time> > animationList;

public:
	Stage(sf::RenderWindow &window, std::vector <std::string> operationName, std::vector <std::vector <std::string> > modeName, 
		std::vector <std::vector <std::vector <std::string> > > valueName, 
		std::vector <std::vector <std::vector <TypingBoxMode> > > typingMode,
		std::vector <std::vector <std::vector <std::pair <int, int> > > > valueBound,
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
	sf::Time getTotalTime();
	void updateCurTime(sf::Time deltaT);
	void addAnimationStep(std::vector <Animation> animations, sf::Time time);
};