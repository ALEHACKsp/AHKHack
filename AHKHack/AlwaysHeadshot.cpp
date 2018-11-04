#include "AlwaysHeadshot.h"
#include "Utils.h"
#include <iostream>

DWORD jmpBackAt;

AlwaysHeadshot::AlwaysHeadshot()
{
	this->hackName = "Always Headshot";
	options.push_back(new HackOption("Chance", 50, 1, 0, 100));

	int hookLength = 5;
	jmpBackAt = OFFSET_HEADSHOT + hookLength;

	Utils::DetourFunction((void*)OFFSET_HEADSHOT, hook, hookLength);
}

void AlwaysHeadshot::onOptionChanged(int optionIndex)
{
}

void __stdcall payload(DWORD* hitzone)
{
	Hack* hack = getHack("Always Headshot"); // maybe this is ghetto idk any other way to do this

	if (hack->options[0]->asBool)
	{
		if ((rand() % 100) < hack->options[1]->asInt)
		{
			*hitzone = 2;
		}
	}
}

void __declspec(naked) hook()
{
	__asm
	{
		push [esp+0x30]
		call payload

		//Original code
		mov eax, [esp+0x2C]
		pop esi

		jmp [jmpBackAt]
	}
}