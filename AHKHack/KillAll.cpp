#include "KillAll.h"
#include "GameFunctions.h"
#include "Game.h"
#include <iostream>

using namespace AssaultCube;

KillAll::KillAll()
{
	this->hackName = "Kill All";
}

void KillAll::update()
{
	// Get closest player
	ACPlayer* closest = nullptr;
	float bestDist = 0;
	for (int i = 0; i < validPlayers.size(); i++)
	{
		ACPlayer* player = validPlayers[i];

		if (!player || player->Health < 0 || (GameFunctions::IsTeamGame() && player->team == localPlayer->team))
			continue;

		float dist = player->PosHead.dist(localPlayer->PosHead);

		if (closest == nullptr || dist < bestDist)
		{
			closest = player;
			bestDist = dist;
		}
	}

	// Check if we got a player
	if (closest)
	{
		// Don't do none of this if player is a spectator
		if (localPlayer->bDead) return;

		// If we can fire
		if (GetTickCount() > nextFire)
		{
			// Check if player has enough ammo for this
			if (*localPlayer->pWeapon->pLoadedAmmo > 0)
			{
				// Play the sound + other stuff on our own client
				localPlayer->pWeapon->AttackFx(localPlayer->PosHead, closest->PosHead, *lastMillis);

				// Send the actual shot to the server
				GameFunctions::SendShoot(closest, *lastMillis, true);

				// Decrease his ammo
				--*localPlayer->pWeapon->pLoadedAmmo;

				int ammo = 0;
				memcpy(&ammo, localPlayer->pWeapon->pLoadedAmmo, sizeof(int));

				if (ammo <= 0)
				{
					// Set next fire time
					nextFire = GetTickCount() + localPlayer->pWeapon->info->reloadTime + 1000;

					localPlayer->pWeapon->Reload(true);
				}
				else
				{
					// Set next fire time
					nextFire = GetTickCount() + localPlayer->pWeapon->info->attackDelay;
				}
			}
			else if(localPlayer->pWeapon->type == 0) // knife
			{
				// Play the sound + other stuff on our own client
				localPlayer->pWeapon->AttackFx(localPlayer->PosHead, closest->PosHead, *lastMillis);

				// Send the actual shot to the server
				GameFunctions::SendShoot(closest, *lastMillis, false);

				// Set next fire time
				nextFire = GetTickCount() + localPlayer->pWeapon->info->attackDelay;
			}
		}
	}
}