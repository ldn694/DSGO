#include "Game.h"

Game::Game(sf::ContextSettings settings) :
	window(sf::VideoMode(WIDTH_RES, HEIGHT_RES), "DSGO", sf::Style::Close, settings), theme(LightTheme),
	lightBulb("Images/full_bulb.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 8 / 46 * 30, widthBox / 8, bulbColor),
	darkBulb("Images/empty_bulb.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 8 / 46 * 30, widthBox / 8, bulbColor),
	themeBox("Images/curved_square.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 4, widthBox / 4, backButtonNormalFillColor)
{
	hashTableBox = Box(410, 400, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "Hash Table", font(fontType::Prototype), 30, NO_BORDER, 3);
	AVLBox = Box(410, 550, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "AVL Tree", font(fontType::Prototype), 30, NO_BORDER, 3);
	minHeapBox = Box(800, 400, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "Min Heap", font(fontType::Prototype), 30, NO_BORDER, 3);
	maxHeapBox = Box(800, 550, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "Max Heap", font(fontType::Prototype), 30, NO_BORDER, 3);
	BTreeBox = Box(1190, 400, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "2-3-4 Tree", font(fontType::Prototype), 30, NO_BORDER, 3);
	TrieBox = Box(1190, 550, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "Trie", font(fontType::Prototype), 30, NO_BORDER, 3);
	GraphBox = Box(800, 700, 360, 120, { CommandBoxNormal, CommandBoxSelected }, "Graph", font(fontType::Prototype), 30, NO_BORDER, 3);
	window.setMouseCursor(arrowCursor);
	window.setFramerateLimit(60);
	srand(time(NULL));
}

void Game::handleMouseMove(float x, float y) {
	if (hashTableBox.handleMouseMove(x, y, window)) return;
	if (AVLBox.handleMouseMove(x, y, window)) return;
	if (minHeapBox.handleMouseMove(x, y, window)) return;
	if (maxHeapBox.handleMouseMove(x, y, window)) return;
	if (BTreeBox.handleMouseMove(x, y, window)) return;
	if (TrieBox.handleMouseMove(x, y, window)) return;
	if (GraphBox.handleMouseMove(x, y, window)) return;
	if (themeBox.handleMouseMove(x, y, window)) return;
	window.setMouseCursor(arrowCursor);
}

void Game::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			return;
			break;
		case sf::Event::MouseButtonPressed:
			if (hashTableBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runHashTable();
			}
			else if (AVLBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runAVL();
			}
			else if (minHeapBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runMinHeap();
			}
			else if (maxHeapBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runMaxHeap();
			}
			else if (BTreeBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runBTree();
			}
			else if (TrieBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runTrie();
			}
			else if (GraphBox.isInside(event.mouseButton.x, event.mouseButton.y)) {
				runGraph();
			}
			else if (themeBox.isMousePressed(event.mouseButton.x, event.mouseButton.y)) {
				theme = ColorTheme((theme + 1) % numColorTheme);
			}
			else {
				handleMouseMove(2000, 2000);
			}
			break;
		case sf::Event::MouseMoved:
			handleMouseMove(event.mouseMove.x, event.mouseMove.y);
			break;
		}
	}
}

void Game::update(sf::Time deltaT) {
	
}

void Game::render() {
	window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	themeBox.draw(window, theme);
	if (theme == LightTheme) {
		lightBulb.draw(window, theme);
	}
	else {
		darkBulb.draw(window, theme);
	}
	hashTableBox.draw(window, theme);
	AVLBox.draw(window, theme);
	minHeapBox.draw(window, theme);
	maxHeapBox.draw(window, theme);
	BTreeBox.draw(window, theme);
	TrieBox.draw(window, theme);
	GraphBox.draw(window, theme);
	window.display();
}

void Game::runHashTable() {
	window.setMouseCursor(waitCursor);
	HashTableStage hashTable(window, theme);
	theme = hashTable.run();
}

void Game::runAVL() {
	window.setMouseCursor(waitCursor);
	AVLStage AVL(window, theme);
	theme = AVL.run();
}

void Game::runMinHeap() {
	window.setMouseCursor(waitCursor);
	HeapStage minHeap(window, theme, true);
	theme = minHeap.run();
}

void Game::runMaxHeap() {
	window.setMouseCursor(waitCursor);
	HeapStage maxHeap(window, theme, false);
	theme = maxHeap.run();
}

void Game::runBTree() {
	window.setMouseCursor(waitCursor);
	BTreeStage BTree(window, theme);
	theme = BTree.run();
}

void Game::runTrie() {
	window.setMouseCursor(waitCursor);
	TrieStage Trie(window, theme);
	theme = Trie.run();
}

void Game::runGraph() {
	window.setMouseCursor(waitCursor);
	GraphStage Graph(window, theme);
	theme = Graph.run();
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