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

double dist2p(double x1, double y1, double x2, double y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

sf::Time min(const sf::Time& a, const sf::Time& b) {
	return a < b ? a : b;
}

sf::Time max(const sf::Time& a, const sf::Time& b) {
	return a > b ? a : b;
}

void MovePoint(double& x1, double& y1, double x2, double y2, double dist) {
	double hypo = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	if (hypo < epsilonDouble) {
		x1 -= abs(dist);
		return;
	}
	double dx = (x2 - x1) * dist / hypo;
	double dy = (y2 - y1) * dist / hypo;
	x1 += dx;
	y1 += dy;
}

void MovePointParallel(double& x, double& y, double x1, double y1, double x2, double y2) {
	double dx = x2 - x1, dy = y2 - y1;
	x += dx; 
	y += dy;
}

void MovePointUpward(double& x1, double& y1, double x2, double y2, double h) {
	double hypo = dist2p(x1, y1, x2, y2);
	if (hypo == 0) {
		y1 -= abs(h);
		return;
	}
	double dx = (y2 - y1) * h / hypo;
	double dy = (x2 - x1) * h / hypo;
	x1 -= dx;
	y1 += dy;
}

sf::Cursor arrowCursor, handCursor, waitCursor, textCursor;
std::vector <sf::Font> listFont;

sf::Font* font(fontType id) {
	return &listFont[id];
}

double area(double x1, double y1, double x2, double y2, double x3, double y3) {
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
}


bool isInsideTriangle(double x1, double y1, double x2, double y2, double x3, double y3, double x, double y)
{
	double A = area(x1, y1, x2, y2, x3, y3);
	double A1 = area(x, y, x2, y2, x3, y3);
	double A2 = area(x1, y1, x, y, x3, y3);
	double A3 = area(x1, y1, x2, y2, x, y);
	return abs(A - (A1 + A2 + A3)) < epsilonDouble;
}

void RotatePoint(double& x, double& y, double cx, double cy, double angle) {
	angle = angle * (PI / 180); // Convert to radians
	double rotatedX = cos(angle) * (x - cx) - sin(angle) * (y - cy) + cx;
	double rotatedY = sin(angle) * (x - cx) + cos(angle) * (y - cy) + cy;
	x = rotatedX;
	y = rotatedY;
}

sf::Text CompressWords(std::string cur, double x, double y, double width, double height, sf::Font* font, double characterSize, sf::Color color) {
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