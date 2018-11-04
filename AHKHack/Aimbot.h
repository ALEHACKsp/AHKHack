#pragma once
#ifndef AIMBOT_H
#define AIMBOT_H

#include "Hack.h"
#include "GameFunctions.h"
#include <algorithm>

using namespace AssaultCube;

void calcAngles(ACPlayer* localPlayer, ACPlayer* otherPlayer, float* pitch, float* yaw);
ACPlayer* findTarget();

bool distanceSorter(ACPlayer* player1, ACPlayer* player2);
bool crosshairSorter(ACPlayer* player1, ACPlayer* player2);

class Aimbot :
	public Hack
{
private:
	ACPlayer* target;

public:
	Aimbot();

	void update() override;
	void updateAimbotTarget();
};

#endif

