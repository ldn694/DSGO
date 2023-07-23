#include "Game.h"
#include "Template.h"

Game::Game(sf::ContextSettings settings) :
	window(sf::VideoMode(WIDTH_RES, HEIGHT_RES), "DSGO", sf::Style::Close, settings),
    shape(100)
{
	window.setMouseCursor(arrowCursor);
	window.setFramerateLimit(60);
    shape.setOrigin(100, 100);
    shape.setPosition(WIDTH_RES / 2, HEIGHT_RES / 2);
    shape.setFillColor(sf::Color::Red);
}

void Game::processEvents() {
    sf::Event event;
	while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time deltaT) {
	
}

void Game::render() {
	window.clear(sf::Color::White);
	window.draw(shape);
	window.display();
}


void Game::run() {
	window.setMouseCursor(arrowCursor);
	while (window.isOpen())
	{
		processEvents();
		update(timePerFrame);
		render();
	}
}