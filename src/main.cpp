#include <SFML/Graphics.hpp>
#include "Template.h"
#include "Game.h"
#include "cassert"
#include <iostream>

int main()
{
    arrowCursor.loadFromSystem(sf::Cursor::Arrow);
    handCursor.loadFromSystem(sf::Cursor::Hand);
    waitCursor.loadFromSystem(sf::Cursor::Wait);
    textCursor.loadFromSystem(sf::Cursor::Text);
	listFont.resize(numFont);
	for (int i = 0; i < numFont; i++) {
		if (!listFont[i].loadFromFile(fontName[i])) {
			if (!listFont[i].loadFromFile(fontAlternativeName[i])) {
				assert(false);
			}
		}
	}
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	Game game(settings);
	game.run();
}