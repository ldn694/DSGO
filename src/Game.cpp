#include "Game.h"

Game::Game(sf::ContextSettings settings) :
	window(sf::VideoMode(WIDTH_RES, HEIGHT_RES), "DSGO", sf::Style::Close, settings),
    hash(10, font(Consolas))
{
	window.setMouseCursor(arrowCursor);
	window.setFramerateLimit(60);
	srand(time(NULL));
}

void Game::processEvents() {
    sf::Event event;
	while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::I) {
				int x = rand() % 20;
				hash.insertV(x);
				std::cout << "inserted " << x << "\n";
			}
			if (event.key.code == sf::Keyboard::D) {
				int x = rand() % 20;
				hash.deleteV(x);
				std::cout << "deleted " << x << "\n";
			}
		}
    }
}

void Game::update(sf::Time deltaT) {
	
}

void Game::render() {
	window.clear(LavenderBushColor);
	hash.draw(window, LightTheme);
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