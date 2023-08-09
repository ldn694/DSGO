#include <cassert>
#include <iostream>
#include "Stage.h"

Stage::Stage(sf::RenderWindow& _window, std::vector <std::string> _operationName, std::vector <std::vector <std::string> > _modeName,
	std::vector <std::vector <std::vector <std::string> > > _valueName,
	std::vector <std::vector <std::vector <TypingBoxMode> > > _typingMode,
	std::vector <std::vector <std::vector <std::pair <int*, int*> > > > _valueBound,
	std::vector <std::vector <std::vector <std::string> > > codes,
	int* maxSizeData, int* maxValueData, InputType type,
	ColorTheme _theme) :
	window(_window), operationName(_operationName), modeName(_modeName), valueName(_valueName), typingMode(_typingMode), valueBound(_valueBound), theme(_theme), codes(codes), highlightLine(-1),
	backButton(WIDTH_RES - widthBox / 2, 0, widthBox / 4, widthBox / 4),
	readFromFile(widthBox * 0.625, HEIGHT_RES - heightBox * 11.75, widthBox * 0.75, heightBox / 2,
				0, HEIGHT_RES - heightBox * 15, widthBox * 2, heightBox, font(fontType::Prototype), *maxSizeData, 0, *maxValueData, type),
	lightBulb("Images/full_bulb.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 8 / 46 * 30, widthBox / 8, bulbColor),
	darkBulb("Images/empty_bulb.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 8 / 46 * 30, widthBox / 8, bulbColor),
	themeBox("Images/curved_square.png", WIDTH_RES - widthBox / 8, widthBox / 8, widthBox / 4, widthBox / 4, backButtonNormalFillColor),
	ingameSettings(0, HEIGHT_RES - heightBox * 11, widthBox * 2, heightBox * 3, _theme, &animatingDirection),
	codeVisualizer(0, HEIGHT_RES - heightBox * 6, widthBox * 2, heightBox * 6, 0, HEIGHT_RES - heightBox * 7.5, widthBox * 2, heightBox * 1.5),
	toolBox(sf::Vector2f(2 * widthBox, HEIGHT_RES))
{
	toolBox.setPosition(0, 0);
	toolBox.setOutlineThickness(3);
	numOperation = operationName.size();
	operationBox.resize(numOperation);
	curOperation = 0;
	operating = false;
	operationSelecting = false;
	for (int i = 0; i < numOperation; i++) {
		operationBox[i] = Box(0, HEIGHT_RES - heightBox * 14, widthBox, heightBox, { CommandBoxNormal, CommandBoxSelected },
						operationName[i], font(fontType::Prototype), 30, WITH_BORDER, outlineBox);
		if (i != 0) {
			operationBox[i].setDrawable(false);
		}
	}

	outerGoBox = Box(widthBox, HEIGHT_RES - heightBox * 14, widthBox, heightBox, { CommandBoxNormal });
	goBox = Box(widthBox * 1.25f, HEIGHT_RES - heightBox * 13.75f, widthBox / 2.0f, heightBox / 2.0f, { GoBoxNormal, GoBoxSelected },
				"GO", font(fontType::Prototype), 20);

	assert(modeName.size() == numOperation);
	numMode.resize(numOperation);
	modeBox.resize(numOperation);
	for (int i = 0; i < numOperation; i++) {
		numMode[i] = modeName[i].size();
		modeBox[i].resize(numMode[i]);
		for (int j = 0; j < numMode[i]; j++) {
			modeBox[i][j] = Box(0, HEIGHT_RES - heightBox * 13, 2 * widthBox, heightBox, { CommandBoxNormal },
							modeName[i][j], font(fontType::Prototype), 20);
			modeBox[i][j].setDrawable(false);
		}
	}
	if (numOperation) {
		modeBox[0][0].setDrawable(true);
	}
	curMode = 0;

	prevModeButton = TriangleButton(widthBox / 5.0f, HEIGHT_RES - heightBox * 12.5f, 12, 3, -90.f);
	nextModeButton = TriangleButton(2 * widthBox - widthBox / 5.0f, HEIGHT_RES - heightBox * 12.5f, 12, 3, 90.f);
	rightwardTriangle.setPointCount(3);
	rightwardTriangle.setRadius(7);
	rightwardTriangle.setOutlineThickness(0);
	rightwardTriangle.setOrigin(rightwardTriangle.getLocalBounds().left + rightwardTriangle.getLocalBounds().width / 2, rightwardTriangle.getLocalBounds().top + rightwardTriangle.getLocalBounds().height / 2);
	rightwardTriangle.setPosition(widthBox - widthBox * 0.06f, HEIGHT_RES - heightBox * 13.5f);
	rightwardTriangle.setRotation(90.f);
	rightwarding = true;

	numValue.resize(numOperation);
	assert(valueName.size() == numOperation);
	assert(valueBound.size() == numOperation);
	assert(typingMode.size() == numOperation);
	assert(codes.size() == numOperation);
	for (int i = 0; i < numOperation; i++) {
		assert(valueName[i].size() == numMode[i]);
		assert(valueBound[i].size() == numMode[i]);
		assert(typingMode[i].size() == numMode[i]);
		assert(codes[i].size() == numMode[i]);
		numValue[i].resize(numMode[i]);
		for (int j = 0; j < numValue[i].size(); j++) {
			assert(valueName[i][j].size() == valueBound[i][j].size());
			assert(typingMode[i][j].size() == valueBound[i][j].size());
			numValue[i][j] = valueName[i][j].size();
		}
	}
	if (numOperation && numMode[0]) {
		updateModeBox(0);
	}
}

void Stage::setDSName(std::string name) {
	dsName.setString(name);
	dsName.setFont(*font(fontType::Prototype));
	dsName.setCharacterSize(50);
	dsName.setOrigin(dsName.getLocalBounds().left + dsName.getLocalBounds().width / 2, dsName.getLocalBounds().top + dsName.getLocalBounds().height / 2);
	dsName.setPosition((WIDTH_RES - 2 * widthBox) / 2 + 2 * widthBox, 50);
}

void Stage::updateModeBox(int newMode) {
	for (int i = 0; i < numMode[curOperation]; i++) {
		modeBox[curOperation][i].setDrawable(false);
	}
	modeBox[curOperation][newMode].setDrawable(true);
	curMode = newMode;
	codeVisualizer.setLines(codes[curOperation][curMode]);
	if (modeName[curOperation][curMode] == "Upload File") {
		valueTypingBox.clear();
		readFromFile.reset();
		readFromFile.setDisplaying(true);
	}
	else {
		readFromFile.reset();
		valueTypingBox.resize(numValue[curOperation][curMode]);
		for (int i = 0; i < numValue[curOperation][curMode]; i++) {
			valueTypingBox[i] = BigTypingBox(2 * widthBox / numValue[curOperation][curMode] * i, HEIGHT_RES - 12 * heightBox, 2 * widthBox / numValue[curOperation][curMode], heightBox, widthBox / 3, outlineBox, valueName[curOperation][curMode][i],
				0, HEIGHT_RES - heightBox * 15, widthBox * 2, heightBox,
				typingMode[curOperation][curMode][i], font(fontType::Prototype), typingModeMaxCharacter[typingMode[curOperation][curMode][i]], *(valueBound[curOperation][curMode][i].first), *(valueBound[curOperation][curMode][i].second));
		}
	}
}

bool Stage::handleMousePressed(float x, float y) {
	handleMouseMove(x, y);
	ingameSettings.handleMousePressed(x, y);
	codeVisualizer.handleMousePressed(x, y);
	if (goBox.isInside(x, y) && !operationSelecting) {
		operating = true;
	}
	if (!operationSelecting) {
		if (operationBox[curOperation].isInside(x, y)) {
			rightwardTriangle.setRotation(270.f);
			operationSelecting = true;
			//operationBox[curOperation].setColorMode(CommandBoxSelected);
			float x1 = 0, y1 = HEIGHT_RES - heightBox * 14;
			for (int i = 0; i < numOperation; i++) {
				if (i == curOperation) {
					continue;
				}
				x1 += widthBox;
				operationBox[i].setPosition(x1, y1);
				operationBox[i].setColorMode(CommandBoxNormal);
				operationBox[i].setDrawable(true);
			}
		}
	}
	else {
		rightwardTriangle.setRotation(90.f);
		bool flag = false;
		for (int i = 0; i < numOperation; i++) {
			if (operationBox[i].isInside(x, y)) {
				operationSelecting = false;
				if (curOperation != i) {
					for (int j = 0; j < numMode[curOperation]; j++) {
						modeBox[curOperation][j].setDrawable(false);
					}
					curOperation = i;
					updateModeBox(0);
				}
				for (int j = 0; j < numOperation; j++) {
					operationBox[j].setPosition(0, HEIGHT_RES - heightBox * 14);
					operationBox[j].setColorMode(CommandBoxNormal);
					operationBox[j].setDrawable(false);
				}
				operationBox[curOperation].setDrawable(true);
				flag = true;
				break;
			}
		}
		if (!flag) {
			operationSelecting = false;
			for (int i = 0; i < numOperation; i++) {
				operationBox[i].setPosition(0, HEIGHT_RES - heightBox * 14);
				operationBox[i].setColorMode(CommandBoxNormal);
				operationBox[i].setDrawable(false);
			}
			operationBox[curOperation].setDrawable(true);
		}
	}
	if (prevModeButton.isInside(x, y)) {
		updateModeBox((curMode + numMode[curOperation] - 1) % numMode[curOperation]);
	}
	if (nextModeButton.isInside(x, y)) {
		updateModeBox((curMode + numMode[curOperation] + 1) % numMode[curOperation]);
	}
	for (int i = 0; i < numValue[curOperation][curMode]; i++) {
		valueTypingBox[i].clickOn(x, y);
	}
	readFromFile.handleMousePressed(x, y);
	if (themeBox.isMousePressed(x, y)) {
		setTheme(ColorTheme((theme + 1) % numColorTheme));
	}
	return backButton.handleMousePressed(x, y);
}

void Stage::handleKeyPressed(int key) {
	for (int i = 0; i < numValue[curOperation][curMode]; i++) {
		if (valueTypingBox[i].isReading())
		{
			valueTypingBox[i].readKey(key);
		}
	}
	if (key == int(sf::Keyboard::Enter)) {
		operating = true;
	}
	if (key == int(sf::Keyboard::Space)) {
		//toggleIsAnimating();	
	}
	ingameSettings.handleKeyPressed(key);
}

void Stage::handleMouseMove(float x, float y) {
	for (int i = 0; i < numValue[curOperation][curMode]; i++) {
		if (valueTypingBox[i].handleMouseMove(x, y, window)) {
			return;
		}
	}
	window.setMouseCursor(arrowCursor);
	for (int i = 0; i < numOperation; i++) {
		if (i == curOperation) {
			if (operationBox[i].handleMouseMove(x, y, window)) {
				rightwarding = false;
			}
			else {
				rightwarding = true;
			}
		}
		else {
			operationBox[i].handleMouseMove(x, y, window);
		}
	}
	goBox.handleMouseMove(x, y, window);
	prevModeButton.handleMouseMove(x, y, window);
	nextModeButton.handleMouseMove(x, y, window);
	themeBox.handleMouseMove(x, y, window);
	backButton.handleMouseMove(x, y, window);
	readFromFile.handleMouseMove(x, y, window);
	ingameSettings.handleMouseMove(x, y, window);
	codeVisualizer.handleMouseMoved(x, y, window);
}

void Stage::handleMouseReleased(float x, float y) {
	ingameSettings.handleMouseReleased(x, y);
	codeVisualizer.handleMouseReleased(x, y);
}

void Stage::draw() {
	toolBox.setFillColor(toolBoxColor[theme]);
	toolBox.setOutlineColor(toolBoxColor[theme]);
	window.draw(toolBox);
	ingameSettings.draw(window, theme);
	codeVisualizer.draw(window, theme, ingameSettings.getIsDescription());
	themeBox.draw(window, theme);
	if (theme == LightTheme) {
		lightBulb.draw(window, theme);
		dsName.setFillColor(BlackColor);
	}
	else {
		darkBulb.draw(window, theme);
		dsName.setFillColor(WhiteColor);
	}
	window.draw(dsName);
	outerGoBox.draw(window, theme);
	goBox.draw(window, theme);
	for (int i = 0; i < numOperation; i++) {
		for (int j = 0; j < numMode[i]; j++) {
			modeBox[i][j].draw(window, theme);
		}
	}
	prevModeButton.draw(window, theme);
	nextModeButton.draw(window, theme);
	if (theme == LightTheme) {
		if (rightwarding) {
			rightwardTriangle.setFillColor(BlackColor);
		}
		else {
			rightwardTriangle.setFillColor(WhiteColor);
		}
	}
	else {
		if (rightwarding) {
			rightwardTriangle.setFillColor(WhiteColor);
		}
		else {
			rightwardTriangle.setFillColor(BlackColor);
		}
	}
	if (numValue[curOperation][curMode]) {
		for (int i = 0; i < numValue[curOperation][curMode]; i++) {
			valueTypingBox[i].drawAll(window, theme);
		}
	}
	else {
		if (modeName[curOperation][curMode] == "Upload File") {
			Box emptyBox(0, HEIGHT_RES - heightBox * 12, widthBox * 2, heightBox, { CommandBoxNormal });
			emptyBox.draw(window, theme);
		}
	}
	readFromFile.draw(window, theme);
	for (int i = 0; i < numOperation; i++) {
		operationBox[i].draw(window, theme);
	}
	backButton.draw(window, theme);
	window.draw(rightwardTriangle);
}

sf::Time Stage::getPrefixTime(int step) {
	if (step >= animationList.size()) {
		return getTotalTime();
	}
	sf::Time prefixTime = sf::Time::Zero;
	for (int i = 0; i < step; i++) {
		prefixTime += animationList[i].time;
	}
	return prefixTime;
}

sf::Time Stage::getTotalTime() {
	sf::Time totalTime = sf::Time::Zero;
	for (int i = 0; i < animationList.size(); i++) {
		totalTime += animationList[i].time;
	}
	return totalTime;
}

int Stage::getCurStep() {
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

void Stage::updateCurTime(sf::Time deltaT) {
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

void Stage::setCurTime(sf::Time newTime) {
	curTime = newTime;
	ingameSettings.setPercent(curTime.asSeconds() / getTotalTime().asSeconds());
}

void Stage::stageUpdate(sf::Time deltaT) {
	for (int i = 0; i < numValue[curOperation][curMode]; i++) {
		valueTypingBox[i].update(deltaT);
	}
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

void Stage::setAnimatingDirection(AnimatingDirection dir) {
	animatingDirection = dir;
}

void Stage::setTheme(ColorTheme newTheme) {
	theme = newTheme;
}

void Stage::insertVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList) {
	Animation tmp;
	tmp.animationType = InsertVariable;
	tmp.id1 = index;
	tmp.variableList = variableList;
	animations.push_back(tmp);
}

void Stage::deleteVariable(std::vector <Animation> &animations, int index, std::vector <std::string> variableList) {
	Animation tmp;
	tmp.animationType = DeleteVariable;
	tmp.id1 = index;
	tmp.variableList = variableList;
	animations.push_back(tmp);
}

void Stage::setValue(std::vector <Animation> &animations, int index, int value) {
	Animation tmp;
	tmp.animationType = SetValue;
	tmp.id1 = index;
	tmp.nextValue = value;
	animations.push_back(tmp);
}

void Stage::setState(std::vector <Animation> &animations, int index, int state) {
	Animation tmp;
	tmp.animationType = SetState;
	tmp.id1 = index;
	tmp.nextValue = state;
	animations.push_back(tmp);
}

void Stage::setColorType(std::vector <Animation> &animations, int index, int nextColorType) {
	Animation tmp;
	tmp.animationType = SetColorType;
	tmp.id1 = index;
	tmp.nextValue = nextColorType;
	animations.push_back(tmp);
}

void Stage::setLeftEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType) {
	Animation tmp;
	tmp.animationType = SetLeftEdgeColorType;
	tmp.id1 = index;
	tmp.nextValue = nextColorType;
	animations.push_back(tmp);
}

void Stage::setRightEdgeColorType(std::vector <Animation> &animations, int index, int nextColorType) {
	Animation tmp;
	tmp.animationType = SetRightEdgeColorType;
	tmp.id1 = index;
	tmp.nextValue = nextColorType;
	animations.push_back(tmp);
}

void Stage::setRoot(std::vector <Animation> &animations, int nextValue) {
	Animation tmp;
	tmp.animationType = SetRoot;
	tmp.nextValue = nextValue;
	animations.push_back(tmp);
}

void Stage::addNode(std::vector <Animation> &animations, int index, int value) {
	Animation tmp;
	tmp.animationType = AddNode;
	tmp.id1 = index;
	tmp.nextValue = value;
	animations.push_back(tmp);
}

void Stage::addNode(std::vector <Animation> &animations, int index, std::string value) {
	Animation tmp;
	tmp.animationType = AddNode;
	tmp.id1 = index;
	tmp.nextString = value;
	animations.push_back(tmp);
}

void Stage::deleteNode(std::vector <Animation> &animations, int index) {
	Animation tmp;
	tmp.animationType = DeleteNode;
	tmp.id1 = index;
	animations.push_back(tmp);
}

void Stage::swapNode(std::vector <Animation> &animations, int index1, int index2) {
	Animation tmp;
	tmp.animationType = SwapNode;
	tmp.id1 = index1;
	tmp.id2 = index2;
	animations.push_back(tmp);
}

void Stage::setLeftNode(std::vector <Animation> &animations, int index, int nextValue) {
	Animation tmp;
	tmp.animationType = SetLeftNode;
	tmp.id1 = index;
	tmp.nextValue = nextValue;
	animations.push_back(tmp);
}

void Stage::setRightNode(std::vector <Animation> &animations, int index, int nextValue) {
	Animation tmp;
	tmp.animationType = SetRightNode;
	tmp.id1 = index;
	tmp.nextValue = nextValue;
	animations.push_back(tmp);
}

void Stage::insertNodeToGroup(std::vector <Animation> &animations, int idGroup, int idNode) {
	Animation tmp;
	tmp.animationType = InsertNodeToGroup;
	tmp.id1 = idGroup;
	tmp.id2 = idNode;
	animations.push_back(tmp);
}

void Stage::deleteNodeFromGroup(std::vector <Animation> &animations, int idGroup, int idNode) {
	Animation tmp;
	tmp.animationType = DeleteNodeFromGroup;
	tmp.id1 = idGroup;
	tmp.id2 = idNode;
	animations.push_back(tmp);
}

void Stage::addGroup(std::vector <Animation> &animations, int idGroup) {
	Animation tmp;
	tmp.animationType = AddGroup;
	tmp.id1 = idGroup;
	animations.push_back(tmp);
}

void Stage::deleteGroup(std::vector <Animation> &animations, int idGroup) {
	Animation tmp;
	tmp.animationType = DeleteGroup;
	tmp.id1 = idGroup;
	animations.push_back(tmp);
}

void Stage::setEdge(std::vector <Animation> &animations, int idGroup, int idPos, int value) {
	Animation tmp;
	tmp.animationType = SetEdge;
	tmp.id1 = idGroup;
	tmp.id2 = idPos;
	tmp.nextValue = value;
	animations.push_back(tmp);
}

void Stage::insertEdge(std::vector <Animation> &animations, int idU, int idV, std::string weight) {
	Animation tmp;
	tmp.animationType = InsertEdge;
	tmp.id1 = idU;
	tmp.id2 = idV;
	tmp.nextString = weight;
	animations.push_back(tmp);
}

void Stage::deleteEdge(std::vector <Animation> &animations, int idU, int idV) {
	Animation tmp;
	tmp.animationType = DeleteEdge;
	tmp.id1 = idU;
	tmp.id2 = idV;
	animations.push_back(tmp);
}
