#include "UnlimitedStuff.h"
#include "Game.h"

UnlimitedStuff::UnlimitedStuff()
{
	this->hackName = "Unlimited Stuff";
}

void UnlimitedStuff::update()
{
	*localPlayer->pWeapon->pLoadedAmmo = 999;
	*localPlayer->pWeapon->pReserveAmmo = 999;
	localPlayer->Health = 999;
}