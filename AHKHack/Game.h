#pragma once
#ifndef GAME_H
#define GAME_H

#include "Classes.h"
#include <vector>

extern int* maxPlayers;
extern int* gameMode;
extern int* lastMillis;

extern AssaultCube::ACPlayer* localPlayer;
extern ViewMatrix* viewMatrix;

extern AssaultCube::Entity* entities;
extern int* entityCount;

extern std::vector<AssaultCube::ACPlayer*> validPlayers;

void updateLists();
bool IsValidEntity(AssaultCube::ACPlayer* player);

#endif