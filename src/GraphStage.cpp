#include <cassert>
#include <iostream>
#include "GraphStage.h"

GraphStage::GraphStage(sf::RenderWindow& _window, ColorTheme _theme) :
	window(_window), theme(_theme),
	backButton(WIDTH_RES - widthBox / 2, 0, widthBox / 4, widthBox / 4),
	readFromFile(widthBox * 0.125f, 5.125 * heightBox, 0.75f * widthBox, 0.75f * heightBox,
				0, 6.0f * heightBox, widthBox * 2, heightBox, font(fontType::Prototype), maxSizeDataGraph, 0, maxValueDataGraph, InputType::MatrixGraphInput),
	lightBulb("Images/full_bulb.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 8 / 46 * 30, widthBox / 8, bulbColor),
	darkBulb("Images/empty_bulb.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 8 / 46 * 30, widthBox / 8, bulbColor),
	themeBox("Images/curved_square.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 4, widthBox / 4, backButtonNormalFillColor),
	ingameSettings(WIDTH_RES - 2 * widthBox, HEIGHT_RES - heightBox * 3, widthBox * 2, heightBox * 3, _theme, &animatingDirection),
	codeVisualizer(WIDTH_RES - 2 * widthBox, HEIGHT_RES - heightBox * 9, widthBox * 2, heightBox * 6, WIDTH_RES - 2 * widthBox, HEIGHT_RES - heightBox * 10.5, widthBox * 2, heightBox * 1.5),
    algorithmChoices(0, 0, widthBox, 1.5f * heightBox, {"Dijkstra", "MST", "Find CC"}, font(fontType::Prototype), 0),
    representationChoices(0, 2.5 * heightBox, widthBox, 1.5f * heightBox, {"Edge List", "Matrix"}, font(fontType::Prototype), 1),
    directedChoices(widthBox, 2.5 * heightBox, widthBox, 1.5f * heightBox, {"Undirected", "Directed"}, font(fontType::Prototype), 0),
	matrixInput(sf::Rect <float> (0, 7 * heightBox, 2 * widthBox, 2 * widthBox), 0, font(fontType::Prototype)),
	isCreating(false)
{

	outerGoBox = Box(widthBox, 0, widthBox, 1.5f * heightBox, { CommandBoxNormal });
	goBox = Box(1.25f * widthBox, 0.375 * heightBox, 0.5f * widthBox, heightBox * 0.75, { GoBoxNormal, GoBoxSelected },
				"GO", font(fontType::Prototype), 20);

    outerCreateBox = Box(widthBox, 5.0 * heightBox, widthBox, heightBox, { CommandBoxNormal });
    createBox = Box(1.25f * widthBox, 5.25f * heightBox, widthBox / 2.0f, heightBox / 2.0f, { GoBoxNormal, GoBoxSelected },
                    "CREATE", font(fontType::Prototype), 20);

	curMode = 0;
    modeBox.resize(3);
    modeBox[0] = Box(0, 4 * heightBox, 2 * widthBox, heightBox, { CommandBoxNormal }, "Random", font(fontType::Prototype), 20);
    modeBox[1] = Box(0, 4 * heightBox, 2 * widthBox, heightBox, { CommandBoxNormal }, "Empty", font(fontType::Prototype), 20);
	modeBox[2] = Box(0, 4 * heightBox, 2 * widthBox, heightBox, { CommandBoxNormal }, "Read From File", font(fontType::Prototype), 20);
	for (int i = 0; i < modeBox.size(); i++) {
		modeBox[i].setDrawable(false);
	}
	readFromFileBox = Box(0, 5.0 * heightBox, widthBox, heightBox, { CommandBoxNormal }, "", font(fontType::Prototype), 20);
	
	setMode(0);


	prevModeButton = TriangleButton(widthBox / 5.0f, heightBox * 4.5f, 12, 3, -90.f);
	nextModeButton = TriangleButton(2 * widthBox - widthBox / 5.0f, heightBox * 4.5f, 12, 3, 90.f);

    sizeTypingBox = BigTypingBox(0, 5.0 * heightBox, widthBox, heightBox, widthBox / 5, outlineBox, "n =",
				0, 6.0f * heightBox, widthBox * 2, heightBox,
				singleNumber, font(fontType::Prototype), 2, 0, maxSizeDataGraph);
    sizeTypingBox.setDrawable(true);
    
    startVertexTypingBox = BigTypingBox(widthBox, 0, 0.5f * widthBox, 1.5f * heightBox, widthBox / 5, outlineBox, "s =",
				0, 1.5f * heightBox, widthBox * 2, heightBox,
				singleNumber, font(fontType::Prototype), 2, 0, maxSizeDataGraph);
    startVertexTypingBox.setDrawable(false);

    setDSName("Graph");

	viewRect = sf::FloatRect(widthBox * 3, HEIGHT_RES / 4, WIDTH_RES - 6 * widthBox, HEIGHT_RES / 2);
    graphList.push_back(GeneralGraph({}, viewRect, font(fontType::Arial)));
}

void GraphStage::setGraph() {
	GeneralGraph graph = graphList.back();
	matrixInput.setDirected(directedChoices.getChoice());
	graph.setEdges(matrixInput.getEdges(), directedChoices.getChoice(), matrixInput.size);
	graphList.clear();
	graphList.push_back(graph);
}

void GraphStage::setDSName(std::string name) {
	dsName.setString(name);
	dsName.setFont(*font(fontType::Prototype));
	dsName.setCharacterSize(50);
	dsName.setOrigin(dsName.getLocalBounds().left + dsName.getLocalBounds().width / 2, dsName.getLocalBounds().top + dsName.getLocalBounds().height / 2);
	dsName.setPosition(WIDTH_RES / 2, 50);
}

void GraphStage::setMode(int newMode) {
	modeBox[curMode].setDrawable(false);
	if (modeBox[curMode].getText() == "Read From File") {
		readFromFile.setDisplaying(false);
		readFromFileBox.setDrawable(false);
		sizeTypingBox.setDrawable(true);
	}
	curMode = newMode;
	modeBox[curMode].setDrawable(true);
	if (modeBox[curMode].getText() == "Read From File") {
		readFromFile.setDisplaying(true);
		readFromFileBox.setDrawable(true);
		sizeTypingBox.setDrawable(false);
	}
}

bool GraphStage::handleMousePressed(float x, float y) {
	handleMouseMove(x, y);
	ingameSettings.handleMousePressed(x, y);
	codeVisualizer.handleMousePressed(x, y);
    algorithmChoices.handleMousePressed(x, y);
    representationChoices.handleMousePressed(x, y);
    if (directedChoices.handleMousePressed(x, y)) {
		setGraph();
	}
    sizeTypingBox.clickOn(x, y);
    startVertexTypingBox.clickOn(x, y);
	matrixInput.handleMousePressed(x, y);

	if (prevModeButton.isInside(x, y)) {
		setMode((curMode - 1 + modeBox.size()) % modeBox.size());
	}
	if (nextModeButton.isInside(x, y)) {
		setMode((curMode + 1 + modeBox.size()) % modeBox.size());
	}
	if (createBox.isInside(x, y)) {
		isCreating = true;
	}
	readFromFile.handleMousePressed(x, y);
	if (themeBox.isMousePressed(x, y)) {
		setTheme(ColorTheme((theme + 1) % numColorTheme));
	}
	return backButton.handleMousePressed(x, y);
}

void GraphStage::handleKeyPressed(int key) {
	if (key == int(sf::Keyboard::Enter)) {
		//operating = true;
	}
	if (key == int(sf::Keyboard::Space)) {
		//toggleIsAnimating();	
	}
	ingameSettings.handleKeyPressed(key);
    if (sizeTypingBox.isReading()) {
        sizeTypingBox.readKey(key);
    }
    if (startVertexTypingBox.isReading()) {
        startVertexTypingBox.readKey(key);
    }
	if (matrixInput.handleKeyPressed(key)) {
		setGraph();
	}
}

void GraphStage::handleMouseMove(float x, float y) {
	// for (int i = 0; i < numValue[curOperation][curMode]; i++) {
	// 	if (valueTypingBox[i].handleMouseMove(x, y, window)) {
	// 		return;
	// 	}
	// }
    algorithmChoices.handleMouseMove(x, y, window);
    representationChoices.handleMouseMove(x, y, window);
    directedChoices.handleMouseMove(x, y, window);
	window.setMouseCursor(arrowCursor);
	goBox.handleMouseMove(x, y, window);
	createBox.handleMouseMove(x, y, window);
	prevModeButton.handleMouseMove(x, y, window);
	nextModeButton.handleMouseMove(x, y, window);
	themeBox.handleMouseMove(x, y, window);
	backButton.handleMouseMove(x, y, window);
	readFromFile.handleMouseMove(x, y, window);
	ingameSettings.handleMouseMove(x, y, window);
	codeVisualizer.handleMouseMoved(x, y, window);
    sizeTypingBox.handleMouseMove(x, y, window);
    startVertexTypingBox.handleMouseMove(x, y, window);
	matrixInput.handleMouseMoved(x, y, window);
}

void GraphStage::handleMouseReleased(float x, float y) {
	ingameSettings.handleMouseReleased(x, y);
	codeVisualizer.handleMouseReleased(x, y);
}

void GraphStage::draw() {
	ingameSettings.draw(window, theme);
	codeVisualizer.draw(window, theme, ingameSettings.getIsDescription());
    representationChoices.draw(window, theme);
    algorithmChoices.draw(window, theme);
    directedChoices.draw(window, theme);
	themeBox.draw(window, theme);
	if (theme == LightTheme) {
		lightBulb.draw(window, theme);
		dsName.setFillColor(BlackColor);
	}
	else {
		darkBulb.draw(window, theme);
		dsName.setFillColor(WhiteColor);
	}
	if (algorithmChoices.getChoice() == 0) {
		goBox.setSize(0.25f * widthBox, 0.75f * heightBox);
		goBox.setPosition(1.625f * widthBox, 0.375f * heightBox);
		outerGoBox.setSize(0.5f * widthBox, 1.5f * heightBox);
		outerGoBox.setPosition(1.5f * widthBox, 0);
		startVertexTypingBox.setDrawable(true);
	}
	else {
		goBox.setSize(0.5f * widthBox, heightBox * 0.75);
		goBox.setPosition(1.25f * widthBox, 0.375 * heightBox);
		outerGoBox.setSize(widthBox, 1.5f * heightBox);
		outerGoBox.setPosition(widthBox, 0);
		startVertexTypingBox.setDrawable(false);
	}
	window.draw(dsName);
	outerGoBox.draw(window, theme);
	goBox.draw(window, theme);
    outerCreateBox.draw(window, theme);
    createBox.draw(window, theme);
	for (int i = 0; i < modeBox.size(); i++) {
		modeBox[i].draw(window, theme);
	}
	prevModeButton.draw(window, theme);
	nextModeButton.draw(window, theme);
    startVertexTypingBox.drawAll(window, theme);
	matrixInput.draw(window, theme);
	readFromFileBox.draw(window, theme);
	readFromFile.draw(window, theme);
    sizeTypingBox.drawAll(window, theme);
	backButton.draw(window, theme);
}

sf::Time GraphStage::getPrefixTime(int step) {
	if (step >= animationList.size()) {
		return getTotalTime();
	}
	sf::Time prefixTime = sf::Time::Zero;
	for (int i = 0; i < step; i++) {
		prefixTime += animationList[i].time;
	}
	return prefixTime;
}

sf::Time GraphStage::getTotalTime() {
	sf::Time totalTime = sf::Time::Zero;
	for (int i = 0; i < animationList.size(); i++) {
		totalTime += animationList[i].time;
	}
	return totalTime;
}

int GraphStage::getCurStep() {
	sf::Time totalTime = sf::Time::Zero;
	if (curTime - sf::Time::Zero < epsilonTime && curTime - sf::Time::Zero > -epsilonTime) {
		return 0;
	}
	for (int i = 0; i < animationList.size(); i++) {
		totalTime += animationList[i].time;
		if ((curTime - totalTime < epsilonTime && curTime - totalTime > -epsilonTime)) {
			return i + 1;
		}
		if (curTime < totalTime) {
			return -(i + 1);
		}
	}
	return animationList.size();
}

void GraphStage::updateCurTime(sf::Time deltaT) {
	curTime += deltaT;
	if (curTime > getTotalTime()) {
		curTime = getTotalTime();
		animatingDirection = Pause;
	}
	if (curTime < sf::Time::Zero) {
		curTime = sf::Time::Zero;
	}
	ingameSettings.setPercent(curTime.asSeconds() / getTotalTime().asSeconds());
}

void GraphStage::setCurTime(sf::Time newTime) {
	curTime = newTime;
	ingameSettings.setPercent(curTime.asSeconds() / getTotalTime().asSeconds());
}

void GraphStage::graphStageUpdate(sf::Time deltaT) {
	sizeTypingBox.update(deltaT);
	startVertexTypingBox.update(deltaT);
	matrixInput.update(deltaT);
	deltaT *= ingameSettings.getSpeed();
	if (ingameSettings.getIsChangingFrame()) {
		setAnimatingDirection(Pause);
		setCurTime(sf::seconds(ingameSettings.getPercent() * getTotalTime().asSeconds()));
	}
	else {
		if (animatingDirection != Pause) {
			if (animatingDirection == Continuous) {
				updateCurTime(deltaT);
			}
			if (animatingDirection == Forward || animatingDirection == Backward) {
				if (previousStep == UNKOWN) {
						previousStep = getCurStep();
						if (previousStep < 0) {
							if (animatingDirection == Forward) {
							updateCurTime(min(deltaT, getPrefixTime(-previousStep) - curTime));
							}
							else {
								updateCurTime(-min(deltaT, curTime - getPrefixTime(-previousStep - 1)));
							}
						}
						else {
							updateCurTime(deltaT * float(animatingDirection == Forward ? 1 : -1));
						}
				}
				else {
					int curStep = getCurStep();
					if (previousStep < 0 && curStep >= 0) {
						animatingDirection = Pause;
						previousStep = UNKOWN;
					}
					else {
						if (curStep < 0) {
							if (animatingDirection == Forward) {
							updateCurTime(min(deltaT, getPrefixTime(-curStep) - curTime));
							}
							else {
								updateCurTime(-min(deltaT, curTime - getPrefixTime(-curStep - 1)));
							}
						}
						else {
							updateCurTime(deltaT * float(animatingDirection == Forward ? 1 : -1));
						}
						previousStep = curStep;
					}
				}
			}
			if (animatingDirection == Home) {
				setCurTime(sf::Time::Zero);
				animatingDirection = Pause;
			}
			if (animatingDirection == End) {
				setCurTime(getTotalTime());
				animatingDirection = Pause;
			}
		}
	}
	ingameSettings.update(deltaT);
}

void GraphStage::render() {
    window.clear(theme == LightTheme ? LavenderBushColor : EerieBlackColor);
	draw();
	if (graphList.size() == 1) {
		graphList[0].draw(window, theme, sf::Time::Zero, sf::Time::Zero, {});
	}
	else {
		int curStep = getCurStep();
		bool exactStep = false;
		if (curStep < 0) {
			curStep = -curStep - 1;
			codeVisualizer.setHighlightLine(animationList[curStep].line);
			codeVisualizer.setDescription(animationList[curStep].description);
		}
		else {
			exactStep = true;
			if (curStep > 0) {
				codeVisualizer.setHighlightLine(animationList[curStep - 1].line);
				codeVisualizer.setDescription(animationList[curStep - 1].description);
			}
			else {
				codeVisualizer.setHighlightLine(-1);
				codeVisualizer.setDescription("");
			}
		}
		if (!exactStep) {
			graphList[curStep].draw(window, theme, animationList[curStep].time, curTime - getPrefixTime(curStep), animationList[curStep].animations);
		}
		else {
			graphList[curStep].draw(window, theme);
		}
	}
	window.display();
}

std::pair<bool, ColorTheme> GraphStage::processEvents() {
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type) {
		case sf::Event::Closed:
			window.close();
			return { true, theme };
			break;
		case sf::Event::MouseButtonPressed:
			if (handleMousePressed(event.mouseButton.x, event.mouseButton.y)) {
				return { true, theme };
			}
			break;
		case sf::Event::KeyPressed:
			handleKeyPressed(int(event.key.code));
			break;
		case sf::Event::MouseMoved:
			handleMouseMove(event.mouseMove.x, event.mouseMove.y);
			break;
		case sf::Event::MouseButtonReleased:
			handleMouseReleased(event.mouseButton.x, event.mouseButton.y);
			break;
		}
	}
	if (isCreating) {
		if (modeBox[curMode].getText() == "Read From File") {
			auto edgeList = readFromFile.getListEdge();
			if (!edgeList.first.empty()) {
				matrixInput.setEdges(edgeList.first, directedChoices.getChoice(), edgeList.second);
				setGraph();
			}
		}
		else {
			int x = sizeTypingBox.getProperInt();
			if (x != -1) {
				matrixInput.setSize(x);
				setGraph();
			}
			if (modeBox[curMode].getText() == "Random") {
				matrixInput.createRandom();
				setGraph();
			}
		}
		isCreating = false;
	}
	return { false, theme };
}

void GraphStage::update(sf::Time deltaT) {
	graphStageUpdate(deltaT);
}

ColorTheme GraphStage::run() {
	window.setMouseCursor(arrowCursor);
	while (window.isOpen())
	{
		auto curStatus = processEvents();
		if (curStatus.first) {
			return curStatus.second;
		}
		update(timePerFrame);
		render();
	}
	return theme;
}

void GraphStage::setAnimatingDirection(AnimatingDirection dir) {
	animatingDirection = dir;
}

void GraphStage::setTheme(ColorTheme newTheme) {
	theme = newTheme;
}

void GraphStage::insertVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList) {
	Animation tmp;
	tmp.animationType = InsertVariable;
	tmp.id1 = index;
	tmp.variableList = variableList;
	animations.push_back(tmp);
}

void GraphStage::deleteVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList) {
	Animation tmp;
	tmp.animationType = DeleteVariable;
	tmp.id1 = index;
	tmp.variableList = variableList;
	animations.push_back(tmp);
}

void GraphStage::setValue(std::vector <Animation> &animations, int index, int value) {
	Animation tmp;
	tmp.animationType = SetValue;
	tmp.id1 = index;
	tmp.nextValue = value;
	animations.push_back(tmp);
}

void GraphStage::setState(std::vector <Animation> &animations, int index, int state) {
	Animation tmp;
	tmp.animationType = SetState;
	tmp.id1 = index;
	tmp.nextValue = state;
	animations.push_back(tmp);
}

void GraphStage::setColorType(std::vector <Animation> &animations, int index, int nextColorType) {
	Animation tmp;
	tmp.animationType = SetColorType;
	tmp.id1 = index;
	tmp.nextValue = nextColorType;
	animations.push_back(tmp);
}

void GraphStage::setLeftEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType) {
	Animation tmp;
	tmp.animationType = SetLeftEdgeColorType;
	tmp.id1 = index;
	tmp.nextValue = nextColorType;
	animations.push_back(tmp);
}

void GraphStage::setRightEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType) {
	Animation tmp;
	tmp.animationType = SetRightEdgeColorType;
	tmp.id1 = index;
	tmp.nextValue = nextColorType;
	animations.push_back(tmp);
}

void GraphStage::setRoot(std::vector <Animation> &animations, int nextValue) {
	Animation tmp;
	tmp.animationType = SetRoot;
	tmp.nextValue = nextValue;
	animations.push_back(tmp);
}

void GraphStage::addNode(std::vector <Animation> &animations, int index, int value) {
	Animation tmp;
	tmp.animationType = AddNode;
	tmp.id1 = index;
	tmp.nextValue = value;
	animations.push_back(tmp);
}

void GraphStage::addNode(std::vector <Animation> &animations, int index, std::string value) {
	Animation tmp;
	tmp.animationType = AddNode;
	tmp.id1 = index;
	tmp.nextString = value;
	animations.push_back(tmp);
}

void GraphStage::deleteNode(std::vector <Animation> &animations, int index) {
	Animation tmp;
	tmp.animationType = DeleteNode;
	tmp.id1 = index;
	animations.push_back(tmp);
}

void GraphStage::swapNode(std::vector <Animation> &animations, int index1, int index2) {
	Animation tmp;
	tmp.animationType = SwapNode;
	tmp.id1 = index1;
	tmp.id2 = index2;
	animations.push_back(tmp);
}

void GraphStage::setLeftNode(std::vector <Animation> &animations, int index, int nextValue) {
	Animation tmp;
	tmp.animationType = SetLeftNode;
	tmp.id1 = index;
	tmp.nextValue = nextValue;
	animations.push_back(tmp);
}

void GraphStage::setRightNode(std::vector <Animation> &animations, int index, int nextValue) {
	Animation tmp;
	tmp.animationType = SetRightNode;
	tmp.id1 = index;
	tmp.nextValue = nextValue;
	animations.push_back(tmp);
}

void GraphStage::insertNodeToGroup(std::vector <Animation> &animations, int idGroup, int idNode) {
	Animation tmp;
	tmp.animationType = InsertNodeToGroup;
	tmp.id1 = idGroup;
	tmp.id2 = idNode;
	animations.push_back(tmp);
}

void GraphStage::deleteNodeFromGroup(std::vector <Animation> &animations, int idGroup, int idNode) {
	Animation tmp;
	tmp.animationType = DeleteNodeFromGroup;
	tmp.id1 = idGroup;
	tmp.id2 = idNode;
	animations.push_back(tmp);
}

void GraphStage::addGroup(std::vector <Animation> &animations, int idGroup) {
	Animation tmp;
	tmp.animationType = AddGroup;
	tmp.id1 = idGroup;
	animations.push_back(tmp);
}

void GraphStage::deleteGroup(std::vector <Animation> &animations, int idGroup) {
	Animation tmp;
	tmp.animationType = DeleteGroup;
	tmp.id1 = idGroup;
	animations.push_back(tmp);
}

void GraphStage::setEdge(std::vector <Animation> &animations, int idGroup, int idPos, int value) {
	Animation tmp;
	tmp.animationType = SetEdge;
	tmp.id1 = idGroup;
	tmp.id2 = idPos;
	tmp.nextValue = value;
	animations.push_back(tmp);
}

void GraphStage::insertEdge(std::vector <Animation> &animations, int idU, int idV, std::string weight) {
	Animation tmp;
	tmp.animationType = InsertEdge;
	tmp.id1 = idU;
	tmp.id2 = idV;
	tmp.nextString = weight;
	animations.push_back(tmp);
}

void GraphStage::deleteEdge(std::vector <Animation> &animations, int idU, int idV) {
	Animation tmp;
	tmp.animationType = DeleteEdge;
	tmp.id1 = idU;
	tmp.id2 = idV;
	animations.push_back(tmp);
}
