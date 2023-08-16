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
#include "GraphMatrixInput.h"
#include "GeneralGraph.h"

struct GraphStage {
private:
    int size;
    MultipleChoice representationChoices, algorithmChoices, directedChoices;
    Box goBox, outerGoBox, createBox, outerCreateBox;

    sf::Text dsName;

    int curMode;
    std::vector <Box> modeBox;
    TriangleButton prevModeButton, nextModeButton;

	BigTypingBox sizeTypingBox, startVertexTypingBox;

    BackButton backButton;

    ReadFromFile readFromFile;
	Box readFromFileBox;
	
	IngameSettings ingameSettings;
	CodeVisualizer codeVisualizer;

	MyShape lightBulb, darkBulb, themeBox;
	ColorTheme theme;
	sf::RenderWindow& window;

	bool isCreating;
	GraphMatrixInput matrixInput;

	sf::FloatRect viewRect;
	std::vector <GeneralGraph> graphList;

	AnimatingDirection animatingDirection;
	sf::Time curTime;
	int previousStep;
	std::vector <AnimationStep> animationList;

public:
    GraphStage(sf::RenderWindow &window, ColorTheme theme = LightTheme);
	void setGraph();
	void setDSName(std::string name);
	void setMode(int newMode);
	bool handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y);
	void handleMouseReleased(float x, float y);
	void handleKeyPressed(int key);
	void draw();
	void graphStageUpdate(sf::Time deltaT);
	void setTheme(ColorTheme newTheme);
	void setAnimatingDirection(AnimatingDirection dir);
	int getCurStep();
	sf::Time getPrefixTime(int step);
	sf::Time getTotalTime();
	void updateCurTime(sf::Time deltaT);
	void setCurTime(sf::Time newTime);

    std::pair<bool, ColorTheme> processEvents();
    void update(sf::Time deltaT);
    void render();
    ColorTheme run();

	void insertVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList);
	void deleteVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList);
	void setValue(std::vector <Animation> &animations, int index, int value);
	void setState(std::vector <Animation> &animations, int index, int state);
	void setColorType(std::vector <Animation> &animations, int index, int nextColorType);
	void setRoot(std::vector <Animation> &animations, int value);
	void addNode(std::vector <Animation> &animations, int index, int value);
	void addNode(std::vector <Animation> &animations, int index, std::string value);
	void deleteNode(std::vector <Animation> &animations, int index);
	void swapNode(std::vector <Animation> &animations, int index1, int index2);
	void setLeftNode(std::vector <Animation> &animations, int index, int value);
	void setRightNode(std::vector <Animation> &animations, int index, int value);
	void setLeftEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType);
	void setRightEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType);
	void insertNodeToGroup(std::vector <Animation> &animations, int idGroup, int idNode);
	void deleteNodeFromGroup(std::vector <Animation> &animations, int idGroup, int idNode);
	void addGroup(std::vector <Animation> &animations, int idGroup);
	void deleteGroup(std::vector <Animation> &animations, int idGroup);
	void setEdge(std::vector <Animation> &animations, int idGroup, int idPos, int value);
	void insertEdge(std::vector <Animation> &animations, int idU, int idV, std::string weight);
	void deleteEdge(std::vector <Animation> &animations, int idU, int idV);

};