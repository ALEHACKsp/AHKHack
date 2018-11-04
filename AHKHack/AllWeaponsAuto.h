#pragma once
#ifndef ALLWEAPONSAUTO_H
#define ALLWEAPONSAUTO_H
#include "Hack.h"

class AllWeaponsAuto :
	public Hack
{
private:
	char originalBytes[6];

public:
	AllWeaponsAuto();

	void onOptionChanged(int optionIndex) override;
	void patchMemory();
};

#endif