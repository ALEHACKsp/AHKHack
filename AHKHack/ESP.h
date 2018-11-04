#pragma once
#ifndef ESP_H
#define ESP_H

#include "Hack.h"
#include "GameFunctions.h"
#include "Drawing.h"

using namespace AssaultCube;

void drawEntityEsp(Entity* ent, float windowScale);

class ESP :
	public Hack
{
public:
	ESP();

	void update() override;
};

#endif

