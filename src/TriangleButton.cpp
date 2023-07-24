#include "TriangleButton.h"

TriangleButton::TriangleButton(float _x, float _y, float _radius, float _outlineSize, float _rotation) : x(_x), y(_y), radius(_radius), outlineSize(_outlineSize), rotation(_rotation) {
	shape.setOutlineThickness(outlineSize);
	shape.setPointCount(3);
	shape.setOrigin(radius, radius);
	shape.setPosition(x, y);
	shape.setRadius(radius);
	shape.setRotation(rotation);
	type = TriangleButtonNormal;
}

void TriangleButton::draw(sf::RenderWindow& window, ColorTheme theme) {
	shape.setFillColor(colorBox[type][theme].fillColor);
	shape.setOutlineColor(colorBox[type][theme].outlineColor);
	window.draw(shape);
}

bool TriangleButton::isInside(float hereX, float hereY) {
	float x1 = shape.getPoint(0).x, y1 = shape.getPoint(0).y;
	float x2 = shape.getPoint(1).x, y2 = shape.getPoint(1).y;
	float x3 = shape.getPoint(2).x, y3 = shape.getPoint(2).y;
	RotatePoint(x1, y1, radius, radius, rotation);
	RotatePoint(x2, y2, radius, radius, rotation);
	RotatePoint(x3, y3, radius, radius, rotation);
	float dx = x - radius;
	float dy = y - radius;
	x1 += dx; y1 += dy;
	x2 += dx; y2 += dy;
	x3 += dx; y3 += dy;
	return isInsideTriangle(x1, y1, x2, y2, x3, y3, hereX, hereY);
}

void TriangleButton::handleMouseMove(float hereX, float hereY, sf::RenderWindow& window) {
	if (isInside(hereX, hereY)) {
		type = TriangleButtonSelected;
		window.setMouseCursor(handCursor);
	}
	else {
		type = TriangleButtonNormal;
	}
}