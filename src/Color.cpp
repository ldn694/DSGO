#include "Color.h"

sf::Color fadingColor(sf::Color before, sf::Color after, float percent) {
	return sf::Color(
		before.r + (after.r - before.r) * percent,
		before.g + (after.g - before.g) * percent,
		before.b + (after.b - before.b) * percent
	);
}

ColorNode::ColorNode(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _valueColor, sf::Color _variableColor) :
	fillColor(_fillColor), outlineColor(_outlineColor), valueColor(_valueColor), variableColor(_variableColor) {}

ColorBox::ColorBox(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _textColor) :
	fillColor(_fillColor), outlineColor(_outlineColor), textColor(_textColor) {}


Hash::Color::Color(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _valueColor, sf::Color _variableColor) :
    fillColor(_fillColor), outlineColor(_outlineColor), valueColor(_valueColor), variableColor(_variableColor) {}

Hash::Color Hash::fadingColorType(Hash::ColorType before, Hash::ColorType after, ColorTheme theme, float percent) {
	return Hash::Color(
		fadingColor(Hash::color[theme][before].fillColor, Hash::color[theme][after].fillColor, percent),
		fadingColor(Hash::color[theme][before].outlineColor, Hash::color[theme][after].outlineColor, percent),
		fadingColor(Hash::color[theme][before].valueColor, Hash::color[theme][after].valueColor, percent),
		fadingColor(Hash::color[theme][before].variableColor, Hash::color[theme][after].variableColor, percent)
	);
}
