#pragma once
#include <SFML/Graphics.hpp>

//-----------------------------------------------------------------------------------
//Color Table

const sf::Color SilverColor(185, 180, 174);
const sf::Color DimGrayColor(105, 105, 105);
const sf::Color RedColor(255, 0, 0);
const sf::Color GreenColor(0, 255, 0);
const sf::Color BlueColor(0, 0, 255);
const sf::Color BlackColor(0, 0, 0);
const sf::Color WhiteColor(255, 255, 255);
const sf::Color OrangeColor(255, 128, 0);
const sf::Color YellowColor(255, 255, 0);
const sf::Color LightGreenColor(82, 188, 105);
const sf::Color LightYellowColor(255, 217, 61);

const sf::Color LavenderBushColor(255, 245, 248);
const sf::Color EerieBlackColor(30, 30, 30);
const sf::Color AntiFlashWhiteColor(235, 235, 235);

const sf::Color RoyalPurpleColor(108, 74, 182);
const sf::Color VistaBlueColor(185, 224, 255);
const sf::Color MediumSlateBlueColor(141, 114, 225);
const sf::Color UranianBlueColor(141, 158, 255);
const sf::Color DarkCyanColor(14, 131, 136);

const sf::Color LightRedColor(255, 148, 148);
const sf::Color TeaRoseColor(255, 209, 209);
const sf::Color BrightPinkColor(238, 75, 106);

sf::Color fadingColor(sf::Color before, sf::Color after, float percent);

//-----------------------------------------------------------------------------------
//Color theme

const int numColorTheme = 2;
enum ColorTheme {
	LightTheme, DarkTheme
};

//-----------------------------------------------------------------------------------
//Hash Table

namespace Hash {

    enum ColorType {
        normal, highlight, lowlight, highlight2
    };

    const int numColorType = 4;
    
    struct Color {
        sf::Color fillColor, outlineColor, valueColor, variableColor;
        Color(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
    };
	
	Color fadingColorType(ColorType before, ColorType after, ColorTheme theme, float percent);

    extern const Color color[numColorTheme][numColorType];
}


//-----------------------------------------------------------------------------------

namespace AVL {

    enum ColorType {
        normal, highlight, lowlight, highlight2
    };

    const int numColorType = 4;
    
    struct Color {
        sf::Color fillColor, outlineColor, valueColor, variableColor;
        Color(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
    };
	
	Color fadingColorType(ColorType before, ColorType after, ColorTheme theme, float percent);

    extern const Color color[numColorTheme][numColorType];
}

//-----------------------------------------------------------------------------------

namespace Heap {

    enum ColorType {
        normal, highlight, lowlight, highlight2
    };

    const int numColorType = 4;
    
    struct Color {
        sf::Color fillColor, outlineColor, valueColor, variableColor;
        Color(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
    };
	
	Color fadingColorType(ColorType before, ColorType after, ColorTheme theme, float percent);

    extern const Color color[numColorTheme][numColorType];
}

//-----------------------------------------------------------------------------------

namespace BTree {

    enum ColorType {
        normal, highlight, lowlight, highlight2
    };

    const int numColorType = 4;
    
    struct Color {
        sf::Color fillColor, outlineColor, valueColor, variableColor;
        Color(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
    };
	
	Color fadingColorType(ColorType before, ColorType after, ColorTheme theme, float percent);

    extern const Color color[numColorTheme][numColorType];
}

//-----------------------------------------------------------------------------------

namespace Trie {

    enum ColorType {
        normal, highlight, lowlight, highlight2
    };

    const int numColorType = 4;
    
    struct Color {
        sf::Color fillColor, outlineColor, valueColor, variableColor;
        Color(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
    };
	
	Color fadingColorType(ColorType before, ColorType after, ColorTheme theme, float percent);

    extern const Color color[numColorTheme][numColorType];
}

//-----------------------------------------------------------------------------------

namespace General {

    enum ColorType {
        normal, lowlight, highlight,  
        highlight2, highlight3, highlight4, highlight5, 
        highlight6, highlight7, highlight8, highlight9, highlight10,
    };

    const int numColorType = 12;
    
    struct Color {
        sf::Color fillColor, outlineColor, valueColor, variableColor;
        Color(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
    };
	
	Color fadingColorType(ColorType before, ColorType after, ColorTheme theme, float percent);
    Color fadingColorType(ColorType before, float beforeOpacity, ColorType after, float afterOpacity, ColorTheme theme, float percent);

    extern const Color color[numColorTheme][numColorType];
}

//-----------------------------------------------------------------------------------

enum ColorNodeType {
	normal, highlight, lowlight, highlight2, highlight3, faded
};

const int numColorNodeType = 6;

struct ColorNode {
	sf::Color fillColor, outlineColor, valueColor, variableColor;
	ColorNode(sf::Color fillColor, sf::Color outlineColor, sf::Color valueColor, sf::Color variableColor);
};

const ColorNode colorNode[numColorTheme][numColorNodeType] =
{
	{
		ColorNode(LavenderBushColor, BlackColor, BlackColor, RedColor),
		ColorNode(OrangeColor, OrangeColor, LavenderBushColor, RedColor),
		ColorNode(LavenderBushColor, OrangeColor, OrangeColor, RedColor),
		ColorNode(LightGreenColor, LightGreenColor, LavenderBushColor, RedColor),
		ColorNode(BlueColor, BlueColor, LavenderBushColor, RedColor),
		ColorNode(LavenderBushColor, SilverColor, SilverColor, RedColor)
	} ,
	{
		ColorNode(EerieBlackColor, WhiteColor, WhiteColor, WhiteColor),
		ColorNode(MediumSlateBlueColor, MediumSlateBlueColor, EerieBlackColor, WhiteColor),
		ColorNode(EerieBlackColor, MediumSlateBlueColor, MediumSlateBlueColor, WhiteColor),
		ColorNode(VistaBlueColor, VistaBlueColor, EerieBlackColor, WhiteColor),
		ColorNode(OrangeColor, OrangeColor, LavenderBushColor, WhiteColor),
		ColorNode(EerieBlackColor, DimGrayColor, DimGrayColor, WhiteColor)
	}
};

enum ColorBoxType {
	CommandBoxNormal, CommandBoxSelected, GoBoxNormal, GoBoxSelected, TriangleButtonNormal, TriangleButtonSelected, Typing_Box, MediaBox, CodeOuterBox, UploadBox, MatrixHeaderBox, TypingBoxNotDrawable
};

const int numColorBoxType = 12;

struct ColorBox {
	sf::Color fillColor, outlineColor, textColor;
	ColorBox(sf::Color fillColor, sf::Color outlineColor, sf::Color textColor);
};

extern const ColorBox colorBox[numColorBoxType][numColorTheme];

extern const sf::Color toolBoxColor[numColorTheme];

extern const sf::Color codeNormalViewColor[numColorTheme];
extern const sf::Color codeHighlightViewColor[numColorTheme];
extern const sf::Color codeHighlightBackGroundColor[numColorTheme];

extern const sf::Color slideBarBackgroundColor[numColorTheme];
extern const sf::Color slideBarColor[numColorTheme];
extern const sf::Color slideBarOutlineColor[numColorTheme];

extern const sf::Color scrubberZipColor[numColorTheme];
extern const sf::Color scrubberPassedColor[numColorTheme];
extern const sf::Color scrubberRemainColor[numColorTheme];

extern const sf::Color playButtonCircleColor[numColorTheme];
extern const sf::Color playButtonTriangleColor[numColorTheme];

extern const sf::Color choicesColor[numColorTheme];
extern const sf::Color choicesOuterCircleColor[numColorTheme];
extern const sf::Color choicesInnerCircleColor[numColorTheme];

extern const sf::Color backButtonNormalOutlineColor[numColorTheme];
extern const sf::Color backButtonNormalFillColor[numColorTheme];

extern const sf::Color descriptionFillColor[numColorTheme];
extern const sf::Color descriptionTextColor[numColorTheme];

extern const sf::Color warningFillColor[numColorTheme];
extern const sf::Color warningTextColor[numColorTheme];

extern const sf::Color bulbColor[numColorTheme];

extern const sf::Color errorTextColor[numColorTheme];

extern const sf::Color switchZipperOnColor[numColorTheme];
extern const sf::Color switchZipperOffColor[numColorTheme];
extern const sf::Color switchOnColor[numColorTheme];
extern const sf::Color switchOffColor[numColorTheme];
extern const sf::Color switchTextColor[numColorTheme];
