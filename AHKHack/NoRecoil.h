#pragma once
#ifndef NORECOIL_H
#define NORECOIL_H
#include "Hack.h"

class NoRecoil :
	public Hack
{
private:
	byte originalBytes[10];

public:
	NoRecoil();

	void onOptionChanged(int optionIndex) override;
	void patchRecoil(bool patch);
};

#endif
