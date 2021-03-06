// AHKHack.cpp : Defines the exported functions for the DLL application.
//

#include "AHKHack.h"
#include "Drawing.h"
#include "Utils.h"
#include "Hack.h"
#include "Menu.h"
#include "Game.h"
#include "tinyxml2.h"
#include "Config.h"
#include <iostream>
#include <vector>

Menu* menu;

typedef void(__cdecl * _drawFrame)(int w, int h, int curfps, int nquads, int curvert, bool underwater);
_drawFrame drawFrame = (_drawFrame)0x40AAF0;

void onFrameRender()
{
	glPushMatrix();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, 0, 1);

	// Update the valid player list
	updateLists();

	// Update hacks
	updateHacks();

	// Update menu
	menu->update();

	glEnable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE);

	glPopMatrix();
}

DWORD jmpBack;

void __declspec(naked) drawFrameHook()
{
	__asm
	{
		call onFrameRender

		// Original code
		mov esi, glDisable

		jmp [jmpBack]
	}
}

DWORD __stdcall MainThread(void* pPparam)
{
	initHacks();

	loadConfig();

	menu = new Menu(&hacks);

	// Hook drawframe function
	int hookLength = 6;
	DWORD hookAddress = 0x0040C375;
	jmpBack = hookAddress + hookLength;

	Utils::DetourFunction((void*)hookAddress, drawFrameHook, hookLength);

	FreeLibraryAndExitThread((HMODULE)pPparam, 0);

	return 0;
}