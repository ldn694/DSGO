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

const int WIDTH_RES = 1600;
const int HEIGHT_RES = 900;


const bool ADD_EDGE = true;
const bool ERASE_EDGE = false;

const int maxID = int(1e9);
const int pointCountCircle = 30;
const float epsilonFloat = 1.f / 1000000.f;
const float PI = 3.14159265358979323846;
const sf::Time epsilonTime = sf::seconds(1.f / 1000000.f);
const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
const sf::Time infTime = sf::seconds(1000000.f);
const sf::Time stepTime = sf::seconds(0.3f);
const sf::Time delayTime = sf::seconds(0.2f);
const sf::Time flickeringTime = sf::seconds(0.5f);
const sf::Time errorDisplayTime = sf::seconds(1.0f);
const sf::Time switchTime = sf::seconds(0.2f);

const float heightBox = 50;
const float widthBox = 200;
const float outlineBox = 3;
const float speedList[] = { 0.25, 0.5, 1.0, 2.0, 4.0 };
const float sizeLetterDescription = 20;
const float sizeLetterError = 20;
const float sizeValueLetter = 20;
const float radiusHash = 30;
const float thicknessHash = 3;

const int maxLetter = 4;
const int maxSizeData = 15;
const int maxValueData = 99;

const float heightScrubber = 20;
const float zipWidth = 15;

enum EdgeType {
	Undirected, SinglyDirected, DoublyDirected
};

std::string intToString(int a);
int stringToInt(std::string a);

float dist2p(float x1, float y1, float x2, float y2);

sf::Time min(const sf::Time& a, const sf::Time& b);
sf::Time max(const sf::Time& a, const sf::Time& b);

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

enum DSType {
	HashTableDS
};
enum AnimationType {
	colorTypeChanging, Move
};
struct Animation {
	DSType dsType;
	AnimationType animationType;
	int nextColorType;
	sf::Vector2f nextPosition;
	sf::Time totalTime, timePassed;
	Animation(DSType dsType, AnimationType animationType, int nextColorType, sf::Time totalTime, sf::Time timePassed);
	Animation(DSType dsType, AnimationType animationType, sf::Vector2f nextPosition, sf::Time totalTime, sf::Time timePassed);
};

enum AnimatingDirection {
	Pause, Continuous, Forward, Backward
};