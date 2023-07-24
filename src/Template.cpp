#include <string>
#include "Template.h"



int stringToInt(std::string a) {
	int res = 0;
	for (int i = 0; i < (int)a.size(); i++) {
		res = res * 10 + (a[i] - '0');
	}
	return res;
}

std::string intToString(int a) {
	std::string res;
	while (a > 0) {
		res.push_back(char(a % 10 + '0'));
		a /= 10;
	}
	reverse(res.begin(), res.end());
	if (res.empty()) {
		res.push_back('0');
	}
	return res;
}

float dist2p(float x1, float y1, float x2, float y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

sf::Time min(const sf::Time& a, const sf::Time& b) {
	return a < b ? a : b;
}

sf::Time max(const sf::Time& a, const sf::Time& b) {
	return a > b ? a : b;
}

void MovePoint(float& x1, float& y1, float x2, float y2, float dist) {
	float hypo = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	if (hypo < epsilonFloat) {
		x1 -= abs(dist);
		return;
	}
	float dx = (x2 - x1) * dist / hypo;
	float dy = (y2 - y1) * dist / hypo;
	x1 += dx;
	y1 += dy;
}

void MovePointParallel(float& x, float& y, float x1, float y1, float x2, float y2) {
	float dx = x2 - x1, dy = y2 - y1;
	x += dx; 
	y += dy;
}

void MovePointUpward(float& x1, float& y1, float x2, float y2, float h) {
	float hypo = dist2p(x1, y1, x2, y2);
	if (hypo == 0) {
		y1 -= abs(h);
		return;
	}
	float dx = (y2 - y1) * h / hypo;
	float dy = (x2 - x1) * h / hypo;
	x1 -= dx;
	y1 += dy;
}

sf::Cursor arrowCursor, handCursor, waitCursor, textCursor;
std::vector <sf::Font> listFont;

sf::Font* font(fontType id) {
	return &listFont[id];
}

float area(float x1, float y1, float x2, float y2, float x3, float y3) {
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}


bool isInsideTriangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
	float A = area(x1, y1, x2, y2, x3, y3);
	float A1 = area(x, y, x2, y2, x3, y3);
	float A2 = area(x1, y1, x, y, x3, y3);
	float A3 = area(x1, y1, x2, y2, x, y);
	return abs(A - (A1 + A2 + A3)) < epsilonFloat;
}

void RotatePoint(float& x, float& y, float cx, float cy, float angle) {
	angle = angle * (PI / 180); // Convert to radians
	float rotatedX = cos(angle) * (x - cx) - sin(angle) * (y - cy) + cx;
	float rotatedY = sin(angle) * (x - cx) + cos(angle) * (y - cy) + cy;
	x = rotatedX;
	y = rotatedY;
}

sf::Text CompressWords(std::string cur, float x, float y, float width, float height, sf::Font* font, float characterSize, sf::Color color) {
	std::string resText, tmp;
	sf::Text tmpText;
	tmpText.setFont(*font);
	tmpText.setCharacterSize(characterSize);
	std::vector <std::string> words;
	cur = cur + " ";
	for (char x : cur) {
		if (x == ' ') {
			words.push_back(tmp);
			tmp.clear();
		}
		else {
			tmp.push_back(x);
		}
	}
	cur.clear();
	tmp.clear();
	for (int i = 0; i < words.size(); i++) {
		tmp = cur + " " + words[i];
		tmpText.setString(tmp);
		if (tmpText.getLocalBounds().width > width * 0.9) {
			resText = resText + cur + "\n";
			cur = words[i];
		}
		else {
			cur = tmp;
		}
	}
	resText = resText + cur;
	while (!resText.empty() && resText[0] == ' ') {
		resText.erase(0, 1);
	}
	tmpText.setString(resText);
	tmpText.setOrigin(tmpText.getLocalBounds().left + tmpText.getLocalBounds().width / 2.0f, tmpText.getLocalBounds().top + tmpText.getLocalBounds().height / 2.0f);
	tmpText.setPosition(x + width / 2.0f, y + height / 2.0f);
	tmpText.setFillColor(color);
	return tmpText;
}