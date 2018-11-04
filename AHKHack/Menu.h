#pragma once
#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include "Hack.h"

class Menu
{
private:
	bool isHidden = false;

	DWORD nextAllowedNavigation;
	int hackSelectedIndex = 0;
	int optionSelectedIndex = -1;
	bool isInEditMode = false;

	std::vector<Hack*> * hacks;
	int lengthOfHacks = 0;
	int heightOfHacks = 0;
	int distBetweenHacks = 25;

	void drawMenuTitle();
	void drawMainMenu();
	void drawOptionsMenu();

	void controlInput();

public:
	Menu(std::vector<Hack*>* hacks);

	void update();
};

#endif