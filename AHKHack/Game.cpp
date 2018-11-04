#include "Game.h"

using namespace AssaultCube;

int* maxPlayers = (int*)OFFSET_MAXPLAYERS;
int* gameMode = (int*)OFFSET_GAMEMODE;
int* lastMillis = (int*)OFFSET_MILLIS;

ACPlayer* localPlayer = *(ACPlayer**)OFFSET_LOCALPLAYER;
ViewMatrix* viewMatrix = (ViewMatrix*)OFFSET_VIEWMATRIX;

std::vector<ACPlayer*> validPlayers;

Entity* entities = (Entity*)*(DWORD*)OFFSET_ENTITYLIST;
int* entityCount = (int*)OFFSET_ENTITYCOUNT;

void updateLists()
{
	validPlayers.clear();
	
	ACPlayer** allPlayers = (ACPlayer**)(*(DWORD*)0x50F4F8);
	int* maxPlayers = reinterpret_cast<int*>(OFFSET_MAXPLAYERS);

	entities = (Entity*)*(DWORD*)OFFSET_ENTITYLIST;

	if (allPlayers)
	{
		for (int i = 0; i < *maxPlayers; i++)
		{
			if (IsValidEntity(allPlayers[i]) && allPlayers[i]->bDead != true && allPlayers[i]->Health > 0)
			{
				validPlayers.push_back(allPlayers[i]);
			}
		}
	}
}

bool IsValidEntity(ACPlayer* player)
{
	if (player)
	{
		if (player->vTable == 0x4E4A98 || player->vTable == 0x4E4AC0)
		{
			return true;
		}
	}
	return false;
}