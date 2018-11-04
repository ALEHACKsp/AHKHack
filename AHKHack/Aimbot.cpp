#include "Aimbot.h"
#include "Game.h"

Aimbot::Aimbot()
{
	this->hackName = "Aimbot";
	this->options.push_back(new HackOption("Smooth", 0.0f, 0.5f, 0.0f, 5.0f));
}

void Aimbot::update()
{
	if (GetAsyncKeyState(VK_LCONTROL) & 0x80000)
	{
		updateAimbotTarget();

		if (target && target != nullptr && target != 0)
		{
			float yaw, pitch;
			calcAngles(localPlayer, target, &pitch, &yaw);

			if (this->options[1]->asFloat != 0.0f)
			{
				// Stolen from 
				// https://bitbucket.org/Krampus1025/rektcube-2.0/src/24927b840b39c97b12e5fe51e110222105da63a9/Hacks/Aimbot.cpp?at=master&fileviewer=file-view-default#Aimbot.cpp-86

				// Get the difference between our wanted angles and our current angles
				float yawBetween = yaw - localPlayer->yaw;
				float pitchBetween = pitch - localPlayer->pitch;

				// Prevent 360 spins
				if (yawBetween > 180)
					yawBetween -= 360;

				if (yawBetween < -180)
					yawBetween += 360;

				// Smewth
				localPlayer->yaw += yawBetween / this->options[1]->asFloat;
				localPlayer->pitch += pitchBetween / this->options[1]->asFloat;
			}
			else
			{
				localPlayer->yaw = yaw;
				localPlayer->pitch = pitch;
			}
		}
	}
	else
	{
		target = nullptr;
	}
}

void calcAngles(ACPlayer* localPlayer, ACPlayer* otherPlayer, float* pitch, float* yaw)
{
	Vector3D posHead = Vector3D(otherPlayer->PosHead);
	posHead.z += 0.2f;

	Vector3D relativeVec = posHead.sub(localPlayer->PosHead);

	*yaw = -atan2(relativeVec.x, relativeVec.y) / 3.141592653589793238463 * 180 + 180;
	*pitch = atan2(relativeVec.z, relativeVec.Length()) * 180.0f / 3.141592f;
}

void Aimbot::updateAimbotTarget()
{
	if (target)
	{
		// Check if target is still a good target
		Vector3D temp;
		bool isOnScreen = GameFunctions::WorldToScreen(target->PosHead, temp);

		// Check if target is dead
		bool isDead = target->bDead || target->Health < 0;

		if (!isOnScreen || isDead)
		{
			// Find new target
			target = findTarget();
		}
	}
	else
	{
		// Not locked on, so just find a target normally
		target = findTarget();
	}
}

ACPlayer* findTarget()
{
	std::sort(validPlayers.begin(), validPlayers.end(), crosshairSorter);

	ACPlayer* chosen = nullptr;

	for (int i = 0; i < validPlayers.size(); i++)
	{
		ACPlayer* player = validPlayers[i];

		if (player->bDead || !GameFunctions::IsVisible(localPlayer->PosHead, player->PosHead) || (GameFunctions::IsTeamGame() && player->team == localPlayer->team))
			continue;

		if (chosen == nullptr)
			chosen = player;
	}

	return chosen;
}

bool distanceSorter(ACPlayer* player1, ACPlayer* player2)
{
	return player1->PosHead.dist(localPlayer->PosHead) < player2->PosHead.dist(localPlayer->PosHead);
}

bool crosshairSorter(ACPlayer* player1, ACPlayer* player2)
{
	return player1->GetDistanceFromCrosshair() < player2->GetDistanceFromCrosshair();
}
