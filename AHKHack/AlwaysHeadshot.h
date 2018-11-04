#pragma once
#ifndef ALWAYSHEADSHOT_H
#define ALWAYSHEADSHOT_H

#include "Hack.h"
#include <Windows.h>

void __stdcall payload(DWORD* hitzone);
void hook();

class AlwaysHeadshot :
	public Hack
{
public:
	AlwaysHeadshot();

	void onOptionChanged(int optionIndex) override;
};

#endif
