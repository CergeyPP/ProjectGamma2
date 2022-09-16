#include "InputMap.h"

#include <iostream>
#include <fstream>

InputMap::~InputMap()
{
	for (auto& elem : actionMap) {
		delete elem.second;
	}
	actionMap.clear();
}

void InputMap::load(const std::string& filePath)
{
	std::fstream file(filePath);
	if (!file.is_open()) {
		std::cout << "Cant load  InputMap from " << filePath << std::endl;
		return;
	}

	while (!file.eof()) {
		std::string actionName;
		file >> actionName;
		std::string actionType;
		file >> actionType;

		if (actionType == "KEY") {
			char button;
			file >> button;

			actionMap[actionName] = new KeyInputAction(button);
		}
		if (actionType == "KEYAXIS") {
			std::string buttons[4];
			for (int i = 0; i < 4; i++) {
				file >> buttons[i];
			}

			actionMap[actionName] = new AxisKeyInputAction(buttons[0][0], buttons[1][0], buttons[2][0], buttons[3][0]);
		}
		if (actionType == "MOUSEBUTTON") {
			int button;
			file >> button;
			actionMap[actionName] = new MouseButtonAction(button);
		}
		if (actionType == "MOUSEAXIS") {
			actionMap[actionName] = new AxisMouseMovedInputAction();
		}
		if (actionType == "SCROLLAXIS") {
			actionMap[actionName] = new AxisScrollInputAction();
		}
		if (actionType == "CLOSE") {
			
		}
	}
}

void InputMap::broadcastKeyEvent(int key, int action, int mods)
{
	for (auto& elem : actionMap) {
		elem.second->keyAction(key, action, mods);
	}
}

void InputMap::broadcastMouseButtonEvent(int button, int action, int mods)
{
	for (auto& elem : actionMap) {
		elem.second->mouseButtonAction(button, action, mods);
	}
}

void InputMap::broadcastMouseMovedEvent(double xoffset, double yoffset)
{
	for (auto& elem : actionMap) {
		elem.second->mouseMovedAction(xoffset, yoffset);
	}
}

void InputMap::broadcastScrollEvent(double xoffset, double yoffset)
{
	for (auto& elem : actionMap) {
		elem.second->scrollAction(xoffset, yoffset);
	}
}

void InputMap::broadcastCloseEvent()
{
	for (auto& elem : actionMap) {
		elem.second->closeAction();
	}
}
