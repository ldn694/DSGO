#pragma once
#include <SFML/Graphics.hpp>
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <cmath>

namespace SFML {
	typedef sf::RectangleShape Rectangle;
	// Alternatively, you can use "using Rectangle = sf::RectangleShape;" in C++11 or later.
}

namespace WinAPI {
	typedef ::RECT Rectangle;
}

extern const int WIDTH_RES;
extern const int HEIGHT_RES;


extern const bool ADD_EDGE;
extern const bool ERASE_EDGE;

extern const int maxID;
extern const int pointCountCircle;
extern const float epsilonFloat;
extern const float PI;
extern const sf::Time epsilonTime;
extern const sf::Time timePerFrame;
extern const sf::Time infTime;
extern const sf::Time stepTime;
extern const sf::Time delayTime;
extern const sf::Time flickeringTime;
extern const sf::Time errorDisplayTime;
extern const sf::Time switchTime;

extern const float heightBox;
extern const float widthBox;
extern const float outlineBox;
extern const float speedList[];
extern const float sizeLetterDescription;
extern const float sizeLetterError;

enum TypingBoxMode {
	singleNumber, multipleNumber, string
};
const int numTypingBoxMode = 3;

const int  typingModeMaxCharacter[numTypingBoxMode] = { 3, 20, 20 };

extern const float sizeValueLetterHash;
extern const float radiusHash;
extern const float thicknessHash;
extern int maxSizeDataHash;
extern int maxValueDataHash;

extern const bool LEFT;
extern const bool RIGHT;
extern const int maxHeightAVL;
extern const float sizeValueLetterAVL;
extern const float radiusAVL;
extern const float thicknessAVL;
extern const float minHorizontalDistAVL;
extern const float verticalDistAVL;
extern int maxSizeDataAVL;
extern int maxValueDataAVL;


extern const int maxHeightHeap;
extern const float sizeValueLetterHeap;
extern const float radiusHeap;
extern const float thicknessHeap;
extern const float minHorizontalDistHeap;
extern const float verticalDistHeap;
extern int maxSizeDataHeap;
extern int maxValueDataHeap;

extern const float sizeValueLetterBTree;
extern const float edgeBTree;
extern const float thicknessBTree;
extern const float minHorizontalDistBTree;
extern const float verticalDistBTree;
extern const int maxDegreeBTree;
extern int maxSizeDataBTree;
extern int maxValueDataBTree;

extern const bool ISWORD;
extern const bool NOTWORD;
extern const float sizeValueLetterTrie;
extern const float radiusTrie;
extern const float thicknessTrie;
extern const float minHorizontalDistTrie;
extern const float verticalDistTrie;
extern int maxSizeDataTrie;
extern int maxLengthDataTrie;
extern int maxAlphabetSizeTrie;

extern const float sizeValueLetterGraph;
extern const float edgeGraph;
extern const float radiusGraph;
extern const float thicknessGraph;
extern int maxSizeDataGraph;
extern int maxValueDataGraph;
extern const float idealLength;
extern const float damperingConst;
extern const int maxStep;

extern const int UNKOWN;

extern const int maxLetter;
extern int zeroInt;
extern int oneInt;

extern const float heightScrubber;
extern const float zipWidth;

enum EdgeType {
	Undirected, SinglyDirected, DoublyDirected
};

std::string intToString(int a);
int stringToInt(std::string a);

std::string charToString(char a);

float dist2p(float x1, float y1, float x2, float y2);

sf::Time min(const sf::Time& a, const sf::Time& b);
sf::Time max(const sf::Time& a, const sf::Time& b);

bool diffSign(int a, int b);

float area(float x1, float y1, float x2, float y2, float x3, float y3);

bool isInsideTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y);

void RotatePoint(float& x, float& y, float cx, float cy, float angle);

void MovePoint(float& x1, float& y1, float x2, float y2, float dist);

void MovePointUpward(float& x1, float& y1, float x2, float y2, float h);

void MovePointParallel(float& x, float& y, float x1, float y1, float x2, float y2); //move point A(x, y) to a A'(x', y') so that AA' is parallel to BC in which B is (x1, y1) and C is (x2, y2) and |AA'|=|BC|

float length(sf::Vector2f a);

sf::Vector2f normalize(sf::Vector2f a);

const std::string fontName[] = { "Fonts/arial.ttf", "Fonts/Consolas.ttf", "Fonts/Prototype.ttf"};
const std::string fontAlternativeName[] = { "..\\..\\Fonts\\Arial.ttf", "..\\..\\Fonts\\Consolas.ttf", "..\\..\\Fonts\\Prototype.ttf" };
const int numFont = 3;
enum fontType {
	Arial, Consolas, Prototype
};
extern std::vector <sf::Font> listFont;
extern sf::Cursor arrowCursor, handCursor, waitCursor, textCursor;
sf::Font* font(fontType id);

sf::Text CompressWords(std::string cur, float x, float y, float width, float height, sf::Font* font, float characterSize, sf::Color color); //return cur with \n so that the width of sf::Text is not greater than width

enum AnimationType {
	AddNode, SwapNode, SetLeftNode, SetRightNode,
	AddGroup, InsertNodeToGroup, SetEdgeType,
	SetRoot, SetValue, SetState, SetSize,
	SetColorType, Move, DeleteVariable, InsertVariable,
	SetLeftEdgeColorType, SetRightEdgeColorType, 
	DeleteEdge,
	DeleteNode, DeleteNodeFromGroup, DeleteGroup,
	SetEdge, InsertEdge
};
struct Animation {
	AnimationType animationType;
	int id1 = -1, id2 = 0;
	int nextValue = 0;
	std::string nextString = "";
	float nextPercent = 0.f;
	std::vector <std::string> variableList;
	sf::Vector2f nextPosition;
	bool operator < (const Animation& other) const;
};

struct AnimationStep {
	std::vector <Animation> animations;
	sf::Time time;
	int line;
	std::string description;
	AnimationStep(std::vector <Animation> animations, sf::Time time, int line, std::string description);
};

enum AnimatingDirection {
	Pause, Continuous, Forward, Backward, Home, End
};