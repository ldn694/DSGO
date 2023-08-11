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

//-----------------------------------------------------------------------------------

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

const Hash::Color Hash::color[numColorTheme][numColorType] =
{
	{
		Color(LavenderBushColor, BlackColor, BlackColor, RedColor),
		Color(OrangeColor, OrangeColor, LavenderBushColor, RedColor),
		Color(LavenderBushColor, OrangeColor, OrangeColor, RedColor),
		Color(LightGreenColor, LightGreenColor, LavenderBushColor, RedColor)
	} ,
	{
		Color(EerieBlackColor, WhiteColor, WhiteColor, WhiteColor),
		Color(MediumSlateBlueColor, MediumSlateBlueColor, EerieBlackColor, WhiteColor),
		Color(EerieBlackColor, MediumSlateBlueColor, MediumSlateBlueColor, WhiteColor),
		Color(VistaBlueColor, VistaBlueColor, EerieBlackColor, WhiteColor)
	}
};

//-----------------------------------------------------------------------------------

AVL::Color::Color(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _valueColor, sf::Color _variableColor) :
    fillColor(_fillColor), outlineColor(_outlineColor), valueColor(_valueColor), variableColor(_variableColor) {}

AVL::Color AVL::fadingColorType(AVL::ColorType before, AVL::ColorType after, ColorTheme theme, float percent) {
	return AVL::Color(
		fadingColor(Hash::color[theme][before].fillColor, Hash::color[theme][after].fillColor, percent),
		fadingColor(Hash::color[theme][before].outlineColor, Hash::color[theme][after].outlineColor, percent),
		fadingColor(Hash::color[theme][before].valueColor, Hash::color[theme][after].valueColor, percent),
		fadingColor(Hash::color[theme][before].variableColor, Hash::color[theme][after].variableColor, percent)
	);
}

const AVL::Color AVL::color[numColorTheme][numColorType] =
{
	{
		Color(LavenderBushColor, BlackColor, BlackColor, RedColor),
		Color(OrangeColor, OrangeColor, LavenderBushColor, RedColor),
		Color(LavenderBushColor, OrangeColor, OrangeColor, RedColor),
		Color(LightGreenColor, LightGreenColor, LavenderBushColor, RedColor)
	} ,
	{
		Color(EerieBlackColor, WhiteColor, WhiteColor, WhiteColor),
		Color(MediumSlateBlueColor, MediumSlateBlueColor, EerieBlackColor, WhiteColor),
		Color(EerieBlackColor, MediumSlateBlueColor, MediumSlateBlueColor, WhiteColor),
		Color(VistaBlueColor, VistaBlueColor, EerieBlackColor, WhiteColor)
	}
};

//-----------------------------------------------------------------------------------

Heap::Color::Color(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _valueColor, sf::Color _variableColor) :
    fillColor(_fillColor), outlineColor(_outlineColor), valueColor(_valueColor), variableColor(_variableColor) {}

Heap::Color Heap::fadingColorType(Heap::ColorType before, Heap::ColorType after, ColorTheme theme, float percent) {
	return Heap::Color(
		fadingColor(Hash::color[theme][before].fillColor, Hash::color[theme][after].fillColor, percent),
		fadingColor(Hash::color[theme][before].outlineColor, Hash::color[theme][after].outlineColor, percent),
		fadingColor(Hash::color[theme][before].valueColor, Hash::color[theme][after].valueColor, percent),
		fadingColor(Hash::color[theme][before].variableColor, Hash::color[theme][after].variableColor, percent)
	);
}

const Heap::Color Heap::color[numColorTheme][numColorType] =
{
	{
		Color(LavenderBushColor, BlackColor, BlackColor, RedColor),
		Color(OrangeColor, OrangeColor, LavenderBushColor, RedColor),
		Color(LavenderBushColor, OrangeColor, OrangeColor, RedColor),
		Color(LightGreenColor, LightGreenColor, LavenderBushColor, RedColor)
	} ,
	{
		Color(EerieBlackColor, WhiteColor, WhiteColor, WhiteColor),
		Color(MediumSlateBlueColor, MediumSlateBlueColor, EerieBlackColor, WhiteColor),
		Color(EerieBlackColor, MediumSlateBlueColor, MediumSlateBlueColor, WhiteColor),
		Color(VistaBlueColor, VistaBlueColor, EerieBlackColor, WhiteColor)
	}
};

//-----------------------------------------------------------------------------------

BTree::Color::Color(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _valueColor, sf::Color _variableColor) :
    fillColor(_fillColor), outlineColor(_outlineColor), valueColor(_valueColor), variableColor(_variableColor) {}

BTree::Color BTree::fadingColorType(BTree::ColorType before, BTree::ColorType after, ColorTheme theme, float percent) {
	return BTree::Color(
		fadingColor(BTree::color[theme][before].fillColor, BTree::color[theme][after].fillColor, percent),
		fadingColor(BTree::color[theme][before].outlineColor, BTree::color[theme][after].outlineColor, percent),
		fadingColor(BTree::color[theme][before].valueColor, BTree::color[theme][after].valueColor, percent),
		fadingColor(BTree::color[theme][before].variableColor, BTree::color[theme][after].variableColor, percent)
	);
}

const BTree::Color BTree::color[numColorTheme][numColorType] =
{
	{
		Color(LavenderBushColor, BlackColor, BlackColor, RedColor),
		Color(OrangeColor, OrangeColor, LavenderBushColor, RedColor),
		Color(LavenderBushColor, OrangeColor, OrangeColor, RedColor),
		Color(LightGreenColor, LightGreenColor, LavenderBushColor, RedColor)
	} ,
	{
		Color(EerieBlackColor, WhiteColor, WhiteColor, WhiteColor),
		Color(MediumSlateBlueColor, MediumSlateBlueColor, EerieBlackColor, WhiteColor),
		Color(EerieBlackColor, MediumSlateBlueColor, MediumSlateBlueColor, WhiteColor),
		Color(VistaBlueColor, VistaBlueColor, EerieBlackColor, WhiteColor)
	}
};

//-----------------------------------------------------------------------------------

Trie::Color::Color(sf::Color _fillColor, sf::Color _outlineColor, sf::Color _valueColor, sf::Color _variableColor) :
    fillColor(_fillColor), outlineColor(_outlineColor), valueColor(_valueColor), variableColor(_variableColor) {}

Trie::Color Trie::fadingColorType(Trie::ColorType before, Trie::ColorType after, ColorTheme theme, float percent) {
	return Trie::Color(
		fadingColor(Trie::color[theme][before].fillColor, Trie::color[theme][after].fillColor, percent),
		fadingColor(Trie::color[theme][before].outlineColor, Trie::color[theme][after].outlineColor, percent),
		fadingColor(Trie::color[theme][before].valueColor, Trie::color[theme][after].valueColor, percent),
		fadingColor(Trie::color[theme][before].variableColor, Trie::color[theme][after].variableColor, percent)
	);
}

const Trie::Color Trie::color[numColorTheme][numColorType] =
{
	{
		Color(LavenderBushColor, BlackColor, BlackColor, RedColor),
		Color(OrangeColor, OrangeColor, LavenderBushColor, RedColor),
		Color(LavenderBushColor, OrangeColor, OrangeColor, RedColor),
		Color(LightGreenColor, LightGreenColor, LavenderBushColor, RedColor)
	} ,
	{
		Color(EerieBlackColor, WhiteColor, WhiteColor, WhiteColor),
		Color(MediumSlateBlueColor, MediumSlateBlueColor, EerieBlackColor, WhiteColor),
		Color(EerieBlackColor, MediumSlateBlueColor, MediumSlateBlueColor, WhiteColor),
		Color(VistaBlueColor, VistaBlueColor, EerieBlackColor, WhiteColor)
	}
};

//-----------------------------------------------------------------------------------

const sf::Color toolBoxColor[numColorTheme] = { AntiFlashWhiteColor, DimGrayColor };

const ColorBox colorBox[numColorBoxType][numColorTheme] =
{
	{ //CommandBoxNormal
		ColorBox(LightRedColor , toolBoxColor[0], BlackColor),
		ColorBox(RoyalPurpleColor, toolBoxColor[1], LavenderBushColor),
	},
	{ //CommandBoxSelected
		ColorBox(BrightPinkColor, LavenderBushColor, WhiteColor),
		ColorBox(VistaBlueColor, EerieBlackColor, EerieBlackColor),
	},
	{ //GoBoxNormal
		ColorBox(LavenderBushColor, LavenderBushColor, EerieBlackColor),
		ColorBox(EerieBlackColor, EerieBlackColor, LavenderBushColor)
	},
	{ //GoBoxSelected
		ColorBox(BrightPinkColor, BrightPinkColor, WhiteColor),
		ColorBox(VistaBlueColor, VistaBlueColor, EerieBlackColor)
	},
	{ //TriangleButtonNormal
		ColorBox(EerieBlackColor, LavenderBushColor, BlackColor),
		ColorBox(LavenderBushColor, EerieBlackColor, LavenderBushColor)
	},
	{ //TriangleButtonSelected
		ColorBox(BrightPinkColor, LavenderBushColor, BlackColor),
		ColorBox(VistaBlueColor, EerieBlackColor, LavenderBushColor)
	},
	{ //Typing_Box
		ColorBox(BlackColor, WhiteColor, WhiteColor),
		ColorBox(EerieBlackColor, LavenderBushColor, WhiteColor)
	},
	{ //MediaBox
		ColorBox(LightRedColor, toolBoxColor[0], BlackColor),
		ColorBox(RoyalPurpleColor, toolBoxColor[1], WhiteColor)
	},
	{ //CodeOuterBox
		ColorBox(TeaRoseColor, toolBoxColor[0], WhiteColor),
		ColorBox(UranianBlueColor, toolBoxColor[1], EerieBlackColor)
	},
	{ //UploadBox
		ColorBox(AntiFlashWhiteColor, BlackColor, BlackColor),
		ColorBox(AntiFlashWhiteColor, BlackColor, BlackColor)
	},
	{ //MatrixHeaderBox
		ColorBox(LightRedColor , toolBoxColor[0], BlackColor),
		ColorBox(RoyalPurpleColor, toolBoxColor[1], LavenderBushColor)
	},
	{ //TypingBoxNotDrawable
		ColorBox(BlackColor, WhiteColor, WhiteColor),
		ColorBox(EerieBlackColor, LavenderBushColor, WhiteColor)
	}
};

const sf::Color codeNormalViewColor[numColorTheme] = { BlackColor, EerieBlackColor };
const sf::Color codeHighlightViewColor[numColorTheme] = { WhiteColor, EerieBlackColor };
const sf::Color codeHighlightBackGroundColor[numColorTheme] = { BrightPinkColor, DarkCyanColor };

const sf::Color slideBarBackgroundColor[numColorTheme] = { EerieBlackColor, BlackColor };
const sf::Color slideBarColor[numColorTheme] = { BrightPinkColor, MediumSlateBlueColor };
const sf::Color slideBarOutlineColor[numColorTheme] = { EerieBlackColor, BlackColor };

const sf::Color scrubberZipColor[numColorTheme] = { BrightPinkColor, UranianBlueColor };
const sf::Color scrubberPassedColor[numColorTheme] = { TeaRoseColor, MediumSlateBlueColor };
const sf::Color scrubberRemainColor[numColorTheme] = { BlackColor, EerieBlackColor };

const sf::Color playButtonCircleColor[numColorTheme] = { BrightPinkColor, UranianBlueColor };
const sf::Color playButtonTriangleColor[numColorTheme] = { TeaRoseColor, EerieBlackColor };

const sf::Color choicesColor[numColorTheme] = { EerieBlackColor, LavenderBushColor };
const sf::Color choicesOuterCircleColor[numColorTheme] = { EerieBlackColor,  EerieBlackColor };
const sf::Color choicesInnerCircleColor[numColorTheme] = { TeaRoseColor, UranianBlueColor };

const sf::Color backButtonNormalOutlineColor[numColorTheme] = { LavenderBushColor, EerieBlackColor };
const sf::Color backButtonNormalFillColor[numColorTheme] = { LightRedColor, RoyalPurpleColor };

const sf::Color descriptionFillColor[numColorTheme] = { BrightPinkColor , VistaBlueColor };
const sf::Color descriptionTextColor[numColorTheme] = { WhiteColor, EerieBlackColor };

const sf::Color warningFillColor[numColorTheme] = { TeaRoseColor , MediumSlateBlueColor };
const sf::Color warningTextColor[numColorTheme] = { BlackColor, WhiteColor };

const sf::Color bulbColor[numColorTheme] = { LavenderBushColor, LavenderBushColor };

const sf::Color errorTextColor[numColorTheme] = { RedColor, VistaBlueColor };

const sf::Color switchZipperOnColor[numColorTheme] = { BrightPinkColor, EerieBlackColor };
const sf::Color switchZipperOffColor[numColorTheme] = { TeaRoseColor, VistaBlueColor };
const sf::Color switchOnColor[numColorTheme] = { TeaRoseColor, VistaBlueColor };
const sf::Color switchOffColor[numColorTheme] = { EerieBlackColor, EerieBlackColor };
const sf::Color switchTextColor[numColorTheme] = { EerieBlackColor, LavenderBushColor };
