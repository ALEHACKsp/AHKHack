#pragma once
#ifndef HACK_H
#define HACK_H

#include <vector>
#include "Classes.h"

enum HackTypes
{
	HACKBOOL = 0, HACKINT, HACKFLOAT
};

class HackOption
{
public:
	const char* name;
	int type;

	bool asBool;
	int asInt;
	float asFloat;

	float incrementBy;
	float minValue;
	float maxValue;

	HackOption(const char* nameVal, bool boolVal)
	{
		name = nameVal;
		type = HACKBOOL;
		asBool = boolVal;
	}

	HackOption(const char* nameVal, int intVal, int increment, int minimumValue, int maximumValue)
	{
		name = nameVal;
		type = HACKINT;
		asInt = intVal;
		incrementBy = increment;
		minValue = minimumValue;
		maxValue = maximumValue;
	}

	HackOption(const char* nameVal, float floatVal, float increment, float minimumValue, float maximumValue)
	{
		name = nameVal;
		type = HACKFLOAT;
		asFloat = floatVal;
		incrementBy = increment;
		minValue = minimumValue;
		maxValue = maximumValue;
	}
};

class Hack
{
public:
	Hack()
	{
		options.push_back(new HackOption("Toggled", true));
	}

	const char* hackName;
	std::vector<HackOption*> options;

	virtual void onOptionChanged(int optionIndex) {}
	virtual void update() {}
};

extern std::vector<Hack*> hacks;

Hack* getHack(const char* name);
void initHacks();
void updateHacks();

#endif
