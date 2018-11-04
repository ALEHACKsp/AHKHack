#pragma once
#ifndef UNLIMITIEDSTUFF_H
#define UNLIMITIEDSTUFF_H

#include "Hack.h"

class UnlimitedStuff :
	public Hack
{
public:
	UnlimitedStuff();

	void update() override;
};

#endif

