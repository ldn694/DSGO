#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cassert>
#include <random>
#include "Color.h"
#include "Template.h"
#include "HashTable.h"

struct Game {
private:
	HashTable hash;
    sf::RenderWindow window;
public:
	Game(sf::ContextSettings settings);
	bool handleMousePressed(float x, float y);
	void handleMouseMove(float x, float y);
	void handleMouseReleased(float x, float y);
	void handleKeyPressed(int key);
	bool handlePlayerInput();
	void processEvents();
	void render();
	void update(sf::Time deltaT);
	void run();
};