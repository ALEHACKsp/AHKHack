#include "RapidFire.h"
#include "Utils.h"
#include "Game.h"

// The reason I did all of this hooking instead of just patching those 2 bytes
// is because in bot mode the rapid fire affected the bots as well
// and that bothered me
// so :P

using namespace AssaultCube;

DWORD jmpBackAddr;

RapidFire::RapidFire()
{
	this->hackName = "Rapid Fire";
	options.push_back(new HackOption("Divide by", 2.0f, 0.5f, 1.0f, 10.0f));

	int hookLength = 5;
	jmpBackAddr = OFFSET_RAPIDFIRE + hookLength;

	Utils::DetourFunction((void*)OFFSET_RAPIDFIRE, rapidFireHook, hookLength);
}

void RapidFire::onOptionChanged(int optionIndex)
{
}

void __stdcall rapidFireHookHandler(Weapon* weapon)
{
	Hack* hack = getHack("Rapid Fire"); //iz ghetto ik
	HackOption* option = hack->options[1];

	if (weapon->player == localPlayer && hack->options[0]->asBool)
	{
		*weapon->pNextFire = weapon->info->attackDelay / option->asFloat;
	}
	else
	{
		*weapon->pNextFire = weapon->info->attackDelay;
	}
}

void __declspec(naked) rapidFireHook()
{
	__asm
	{
		push esi
		call rapidFireHookHandler

		mov esi, [esi+0x14]

		jmp [jmpBackAddr]
	}
}