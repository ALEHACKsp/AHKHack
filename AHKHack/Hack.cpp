#include "Hack.h"
#include "UnlimitedStuff.h"
#include "Aimbot.h"
#include "ESP.h"
#include "NoRecoil.h"
#include "RapidFire.h"
#include "KillAll.h"
#include "AlwaysHeadshot.h"
#include "AllWeaponsAuto.h"
#include "Game.h"

std::vector<Hack*> hacks;

void initHacks()
{
	hacks.push_back(new UnlimitedStuff);
	hacks.push_back(new Aimbot);
	hacks.push_back(new ESP);
	hacks.push_back(new NoRecoil);
	hacks.push_back(new RapidFire);
	hacks.push_back(new KillAll);
	hacks.push_back(new AlwaysHeadshot);
	hacks.push_back(new AllWeaponsAuto);
}

void updateHacks()
{
	for (int i = 0; i < hacks.size(); i++)
	{
		// Check if the hack isnt toggled
		if (hacks[i]->options[0]->asBool == false)
		{
			continue;
		}

		// Update it
		hacks[i]->update();
	}
}

Hack* getHack(const char* name)
{
	for (int i = 0; i < hacks.size(); i++)
	{
		Hack* hack = hacks[i];

		if (hack->hackName == name)
			return hack;
	}

	return nullptr;
}