#pragma once
#ifndef KILLALL_H
#define KILLALL_H
#include "Hack.h"

class KillAll :
	public Hack
{
private:
	DWORD nextFire = 0;

public:
	KillAll();

	void update() override;
};

#endif
