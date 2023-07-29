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
extern const float sizeValueLetter;
extern const float radiusHash;
extern const float thicknessHash;

extern const int UNKOWN;

extern const int maxLetter;
extern int maxSizeDataHash;
extern int maxValueDataHash;
extern int zeroInt;
extern int oneInt;

extern const float heightScrubber;
extern const float zipWidth;

enum EdgeType {
	Undirected, SinglyDirected, DoublyDirected
};

std::string intToString(int a);
int stringToInt(std::string a);

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
	SetColorType, Move, InsertVariable, DeleteVariable, SetValue, SetState
};
struct Animation {
	AnimationType animationType;
	int id1, id2 = 0;
	int nextColorType, nextValue, nextState;
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