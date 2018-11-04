#pragma once
#ifndef RAPIDFIRE_H
#define RAPIDFIRE_H

#include "Hack.h"

void __stdcall rapidFireHookHandler(AssaultCube::Weapon* weapon);
void rapidFireHook();

class RapidFire :
	public Hack
{
private:

public:
	RapidFire();

	void onOptionChanged(int optionIndex) override;
};

#endif