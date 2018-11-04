#include "Classes.h"
#include "GameFunctions.h"

float AssaultCube::ACPlayer::GetDistanceFromCrosshair()
{
	Vector3D screenCoords;
	if (GameFunctions::WorldToScreen(PosHead, screenCoords))
	{
		// This player is on the screen
		// Find distance from crosshair
		Vector3D crosshair = GameFunctions::GetCrosshair();
		float crosshairDist = crosshair.dist(screenCoords);
		return crosshairDist;
	}

	return 100000000;
}