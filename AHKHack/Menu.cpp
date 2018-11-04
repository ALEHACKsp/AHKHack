#include "Menu.h"
#include "Drawing.h"
#include "Config.h"
#include <iostream>
#include <sstream>

Menu::Menu(std::vector<Hack*>* hacks)
{
	this->hacks = hacks;
	for (int i = 0; i < hacks->size(); i++)
	{
		std::string str = hacks->at(i)->hackName;
		int length = str.length() * 8 + 10;
		if (length > this->lengthOfHacks)
			this->lengthOfHacks = length;

		this->heightOfHacks += distBetweenHacks;
	}
}

void Menu::update()
{
	controlInput();

	if (isHidden)
		return;

	drawMenuTitle();
	drawMainMenu();
	drawOptionsMenu();
}

void Menu::controlInput()
{
	if (GetTickCount() < nextAllowedNavigation)
	{
		return;
	}

	nextAllowedNavigation = GetTickCount() + 100;

	if (GetAsyncKeyState(VK_INSERT))
	{
		isHidden = !isHidden;
	}

	if (isHidden)
		return;

	if (GetAsyncKeyState(VK_UP))
	{
		if (optionSelectedIndex == -1)
		{
			int temp = hackSelectedIndex - 1;

			if (temp < 0)
			{
				temp = hacks->size() - 1;
			}

			hackSelectedIndex = temp;
		}
		else if(!isInEditMode)
		{
			int temp = optionSelectedIndex - 1;

			if (temp < 0)
			{
				temp = hacks->at(hackSelectedIndex)->options.size() - 1;
			}

			optionSelectedIndex = temp;
		}
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		if (optionSelectedIndex == -1)
		{
			int temp = hackSelectedIndex + 1;

			if (temp > hacks->size() - 1)
			{
				temp = 0;
			}

			hackSelectedIndex = temp;
		}
		else if(!isInEditMode)
		{
			int temp = optionSelectedIndex + 1;
			
			if (temp > hacks->at(hackSelectedIndex)->options.size() - 1)
			{
				temp = 0;
			}

			optionSelectedIndex = temp;
		}
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		if (optionSelectedIndex == -1)
		{
			optionSelectedIndex = 0;
		}
		else
		{
			Hack* hack = hacks->at(hackSelectedIndex);
			HackOption* option = hack->options[optionSelectedIndex];

			if (option->type == HACKBOOL)
			{
				option->asBool = !option->asBool;
			}
			else
			{
				isInEditMode = true;

				if (option->type == HACKINT)
				{
					option->asInt += option->incrementBy;

					if (option->asInt > option->maxValue)
						option->asInt = option->minValue;
				}
				else
				{
					option->asFloat += option->incrementBy;

					if (option->asFloat > option->maxValue)
						option->asFloat = option->minValue;
				}
			}

			saveConfig();

			hack->onOptionChanged(optionSelectedIndex);
		}
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		if (optionSelectedIndex != -1)
		{
			if (isInEditMode)
			{
				Hack* hack = hacks->at(hackSelectedIndex);
				HackOption* option = hack->options[optionSelectedIndex];

				if (option->type == HACKINT)
				{
					option->asInt -= option->incrementBy;

					if (option->asInt < option->minValue)
						option->asInt = option->maxValue;
				}
				else if(option->type == HACKFLOAT)
				{
					option->asFloat -= option->incrementBy;

					if (option->asFloat < option->minValue)
						option->asFloat = option->maxValue;
				}

				saveConfig();

				hack->onOptionChanged(optionSelectedIndex);
			}
			else
			{
				optionSelectedIndex = -1;
			}
		}
	}
	else if (GetAsyncKeyState(VK_ESCAPE) & 0x80000)
	{
		isInEditMode = false;
	}

}

void Menu::drawMenuTitle()
{
	// Draw title
	Vector3D titlePos = Vector3D(10, 200, 0);
	Drawing::DrawBox(titlePos.x, titlePos.y, titlePos.x + this->lengthOfHacks, titlePos.y + 30, 0.0f, 0.0f, 0.0f, 1.0f);
	Drawing::DrawBoxOutline(titlePos.x, titlePos.y, titlePos.x + this->lengthOfHacks, titlePos.y + 30, 2, 0.1f, 0.4f, 0.7f, 1.0f);
	Drawing::DrawString(titlePos.x + 5, titlePos.y + 20, 1.0f, 1.0f, 1.0f, 1.0f, "%s", "AHK Menu v1.0");
}

void Menu::drawMainMenu()
{
	// Then, using that, we can draw the background
	Vector3D topLeft = Vector3D(10, 230, 0);
	Vector3D bottomRight = Vector3D(topLeft.x + this->lengthOfHacks, topLeft.y + this->heightOfHacks, 0);

	Drawing::DrawBox(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 0.0f, 0.0f, 0.0f, 1.0f);
	Drawing::DrawBoxOutline(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 2, 0.1f, 0.4f, 0.7f, 1.0f);

	// Draw selection box on currently selected index
	int yIndex = topLeft.y + (hackSelectedIndex * distBetweenHacks);
	Vector3D selectTopLeft = Vector3D(topLeft.x, yIndex, 0);
	Vector3D selectBottomRight = Vector3D(bottomRight.x, yIndex + distBetweenHacks, 0);

	// Draw a normal selection box
	Drawing::DrawBox(selectTopLeft.x, selectTopLeft.y, selectBottomRight.x, selectBottomRight.y, 0.29804f, 0.56471f, 1.0f, 1.0f);

	// Draw the hacks
	Vector3D optionTopLeft = Vector3D(topLeft.x + 5, topLeft.y + 18, 0);

	for (int i = 0; i < hacks->size(); i++)
	{
		Hack* hack = hacks->at(i);

		float r = 1, g = 1, b = 1;

		Drawing::DrawString(optionTopLeft.x, optionTopLeft.y + (i * distBetweenHacks), r, g, b, 1.0f, "%s", hack->hackName);
	}
}

void Menu::drawOptionsMenu()
{
	// Get the hack that is currently selected
	Hack* hack = hacks->at(hackSelectedIndex);
	if (!hack)
	{
		// Return out if the hack pointer is invalid, for whatever reason
		return;
	}

	// First, get the height of all the options
	int widestLength = 130; // using hardcoded length
	int height = 0;
	for (int i = 0; i < hack->options.size(); i++)
	{
		height += distBetweenHacks;
	}

	// Calculate the y index of the currently selected hack
	int yIndex = 230 + (hackSelectedIndex * distBetweenHacks);

	// Then, using that, we can draw the background
	Vector3D topLeft = Vector3D(25 + this->lengthOfHacks, yIndex, 0);
	Vector3D bottomRight = Vector3D(topLeft.x + widestLength, topLeft.y + height, 0);

	Drawing::DrawBox(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 0.0f, 0.0f, 0.0f, 1.0f);
	Drawing::DrawBoxOutline(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 2, 0.1f, 0.4f, 0.7f, 1.0f);

	// If we're on the options menu, then we need to draw the selection box as well
	if (optionSelectedIndex != -1)
	{
		int selectionYIndex = topLeft.y + (optionSelectedIndex * distBetweenHacks);
		Vector3D selectTopLeft = Vector3D(topLeft.x, selectionYIndex, 0);
		Vector3D selectBottomRight = Vector3D(bottomRight.x, selectionYIndex + distBetweenHacks, 0);

		Drawing::DrawBox(selectTopLeft.x, selectTopLeft.y, selectBottomRight.x, selectBottomRight.y, 0.29804f, 0.56471f, 1.0f, 1.0f);
	}

	// Draw the hacks
	Vector3D optionTopLeft = Vector3D(topLeft.x + 5, topLeft.y + 18, 0);

	for (int i = 0; i < hack->options.size(); i++)
	{
		HackOption* hackOption = hack->options[i];

		Drawing::DrawString(optionTopLeft.x, optionTopLeft.y + (i * distBetweenHacks), 1.0f, 1.0f, 1.0f, 1.0f, "%s", hackOption->name);
	
		if (hackOption->type == HACKBOOL)
		{
			// Draw rect
			
			Vector3D boxTopLeft = Vector3D(topLeft.x + widestLength - 20, topLeft.y + (i * distBetweenHacks) + 5, 0);
			Vector3D boxBottomRight = Vector3D(topLeft.x + widestLength - 5, boxTopLeft.y + 15, 0);

			if (hackOption->asBool == true)
			{
				Drawing::DrawBox(boxTopLeft.x, boxTopLeft.y, boxBottomRight.x, boxBottomRight.y, 1.0f, 1.0f, 1.0f, 1.0f);
			}
			else
			{
				Drawing::DrawBoxOutline(boxTopLeft.x, boxTopLeft.y, boxBottomRight.x, boxBottomRight.y, 1.8f, 1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
		else
		{
			// Get std::string
			std::stringstream ss;

			if(hackOption->type == HACKINT)
				ss << hackOption->asInt;
			else
				ss << hackOption->asFloat;

			std::string str = ss.str();

			// Calculate length
			int optionLength = str.length() * 8;

			// Draw
			Vector3D optionPoint = Vector3D(optionTopLeft.x + widestLength - optionLength - 10, optionTopLeft.y + (i * distBetweenHacks), 0);
			Drawing::DrawString(optionPoint.x, optionPoint.y, 1.0f, 1.0f, 1.0f, 1.0f, "%s", str.c_str());
		}
	}
}