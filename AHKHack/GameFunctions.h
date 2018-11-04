#pragma once
#ifndef GAMEFUNCTIONS_H
#define GAMEFUNCTIONS_H

#include "Classes.h"

namespace GameFunctions
{
	void OutputToConsole(const char* string);

	AssaultCube::ACPlayer* GetEntityPlayerAimingAt();

	bool IsTeamGame();

	bool IsVisible(Vector3D from, Vector3D to);

	bool WorldToScreen(Vector3D pos, Vector3D &screen);

	void SendShoot(AssaultCube::ACPlayer* target, int millis, bool gib);

	Vector3D GetCrosshair();
}

#endif