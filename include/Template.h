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
const double epsilonDouble = 1.f / 1000000.f;
const double PI = 3.14159265358979323846;
const sf::Time epsilonTime = sf::seconds(1.f / 1000000.f);
const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
const sf::Time infTime = sf::seconds(1000000.f);
const sf::Time stepTime = sf::seconds(0.3f);
const sf::Time delayTime = sf::seconds(0.2f);
const sf::Time flickeringTime = sf::seconds(0.5f);
const sf::Time errorDisplayTime = sf::seconds(1.0f);
const sf::Time switchTime = sf::seconds(0.2f);

const float heightBox = 80;
const float widthBox = 240;
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

const double heightScrubber = 20;
const double zipWidth = 15;

enum EdgeType {
	Undirected, SinglyDirected, DoublyDirected
};

std::string intToString(int a);
int stringToInt(std::string a);

double dist2p(double x1, double y1, double x2, double y2);

sf::Time min(const sf::Time& a, const sf::Time& b);
sf::Time max(const sf::Time& a, const sf::Time& b);

double area(double x1, double y1, double x2, double y2, double x3, double y3);

bool isInsideTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y);

void RotatePoint(double& x, double& y, double cx, double cy, double angle);

void MovePoint(double& x1, double& y1, double x2, double y2, double dist);

void MovePointUpward(double& x1, double& y1, double x2, double y2, double h);

void MovePointParallel(double& x, double& y, double x1, double y1, double x2, double y2); //move point A(x, y) to a A'(x', y') so that AA' is parallel to BC in which B is (x1, y1) and C is (x2, y2) and |AA'|=|BC|

const std::string fontName[] = { "Fonts/arial.ttf", "Fonts/Consolas.ttf", "Fonts/Prototype.ttf"};
const std::string fontAlternativeName[] = { "..\\..\\Fonts\\Arial.ttf", "..\\..\\Fonts\\Consolas.ttf", "..\\..\\Fonts\\Prototype.ttf" };
const int numFont = 3;
enum fontType {
	Arial, Consolas, Prototype
};
extern std::vector <sf::Font> listFont;
extern sf::Cursor arrowCursor, handCursor, waitCursor, textCursor;
sf::Font* font(fontType id);

sf::Text CompressWords(std::string cur, double x, double y, double width, double height, sf::Font* font, double characterSize, sf::Color color); //return cur with \n so that the width of sf::Text is not greater than width