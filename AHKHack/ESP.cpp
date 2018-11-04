#include "ESP.h"
#include "Game.h"
#include <iostream>

ESP::ESP()
{
	this->hackName = "ESP";
	options.push_back(new HackOption("Player ESP", true));
	options.push_back(new HackOption("Item ESP", true));
	options.push_back(new HackOption("Flag ESP", true));
}

void ESP::update()
{
	// Window scale
	GLfloat viewport[4];
	glGetFloatv(GL_VIEWPORT, viewport);
	
	float windowScale = viewport[2] / 800.0f;

	// Player esp
	if (options[1]->asBool)
	{
		for (int i = 0; i < validPlayers.size(); i++)
		{
			ACPlayer* player = validPlayers[i];

			Vector3D headCoords = Vector3D(player->PosHead);
			Vector3D footCoords = Vector3D(player->PosFeet);

			headCoords.z += 0.8f;

			// Get screen coords
			Vector3D headScreenCoords, footScreenCoords;
			if (GameFunctions::WorldToScreen(headCoords, headScreenCoords) && GameFunctions::WorldToScreen(footCoords, footScreenCoords))
			{
				float height = footScreenCoords.y - headScreenCoords.y;
				float width = height / 2;
				float distToPlayer = player->PosHead.dist(localPlayer->PosHead);

				Vector3D espTopLeft = Vector3D(headScreenCoords.x - width / 2, headScreenCoords.y, 0);
				Vector3D espBottomRight = Vector3D(headScreenCoords.x + width / 2, headScreenCoords.y + height, 0);

				float r, g, b;

				r = 1.0f;
				g = 0.0f;
				b = 0.0f;

				// Different colors if this is a team game
				if (GameFunctions::IsTeamGame())
				{
					if (player->team == localPlayer->team)
					{
						// Full blue for same team
						r = 0.0f;
						g = 0.0f;
						b = 1.0f;
					}
				}

				// Draw the outline
				Drawing::DrawBoxOutline(espTopLeft.x, espTopLeft.y, espBottomRight.x, espBottomRight.y, 2.0f, r, g, b, 0.6f);

				// Draw the main box
				Drawing::DrawBox(espTopLeft.x, espTopLeft.y, espBottomRight.x, espBottomRight.y, r, g, b, 0.3f);

				// Draw the name
				std::string str = player->name;
				int textWidth = str.length() * 8;
				int difference = width - textWidth;

				Drawing::DrawString(espTopLeft.x + difference / 2, espBottomRight.y + 12, 0.5f, 1.0f, 0.0f, 1.0f, "%s", player->name);

				// Healthbar stuff
				float y = (200 / distToPlayer) - 4;
				if (y > 18)
					y = 18;
				Vector3D healthBarLeft = Vector3D(espTopLeft.x, espTopLeft.y - y, 0);
				Vector3D healthBarRight = Vector3D(espBottomRight.x, healthBarLeft.y, 0);

				float totalHealthBar = healthBarRight.x - healthBarLeft.x;
				float screenHealth = player->Health * totalHealthBar / 100;
				Vector3D healthBarAmount = Vector3D(healthBarLeft.x + screenHealth, healthBarLeft.y, 0);

				// Draw the background
				Drawing::DrawLine(healthBarLeft.x, healthBarLeft.y, healthBarRight.x, healthBarLeft.y, 4, 0.0f, 0.0f, 0.0f, 1.0f);

				// Actual healthbar
				Drawing::DrawLine(healthBarLeft.x, healthBarLeft.y, healthBarAmount.x, healthBarLeft.y, 4, r, g, b, 1.0f);
			}
		}
	}

	// Item esp
	if (options[2]->asBool)
	{
		for (int i = 0; i < *entityCount; i++)
		{
			Entity* ent = &entities[i];

			switch (ent->type)
			{
			case I_HEALTH:
			case I_AMMO:
			case I_CLIPS:
			case I_GRENADE:
			case I_HELMET:
			case I_AKIMBO:
				drawEntityEsp(ent, windowScale);
				break;
			}
		}
	}

	// Flag esp
	if (options[3]->asBool)
	{
		// Rake
		FlagEnt* flags[2] = 
		{ 
			(FlagEnt*)0x50F4A8, 
			(FlagEnt*)0x50F4CC 
		};

		for (int i = 0; i < 2; i++)
		{
			FlagEnt* flag = flags[i];

			if (!flag || flag == nullptr)
				continue;

			Vector3D centerScreenCoord;
			if (GameFunctions::WorldToScreen(flag->pos, centerScreenCoord))
			{
				float dist = flag->pos.dist(localPlayer->PosHead);
				float width = 450 / dist * windowScale;
				float height = width * 2;

				Vector3D topLeft = Vector3D(centerScreenCoord.x - width, centerScreenCoord.y - height, 0);
				Vector3D bottomRight = Vector3D(centerScreenCoord.x + width, centerScreenCoord.y + height + width, 0);

				float r, g, b;

				if (flag->team == localPlayer->team)
				{
					r = 0.0f;
					g = 1.0f;
					b = 0.0f;
				}
				else
				{
					r = 1.0f;
					g = 0.0f;
					b = 0.0f;
				}

				Drawing::DrawBoxOutline(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 2, r, g, b, 0.7f);
				Drawing::DrawBox(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, r, g, b, 0.3f);

				std::string str = "Flag";
				int textWidth = str.length() * 8;
				int difference = (width * 2) - textWidth;

				Drawing::DrawString(topLeft.x + difference / 2, bottomRight.y + 12, 0.5f, 1.0f, 0.0f, 1.0f, "%s", str.c_str());
			}
		}
	}
}

void drawEntityEsp(Entity* ent, float windowScale)
{
	if (!ent->spawned) return;

	// Stolen from A200K
	float flNewZ = (float)(1 + sinf(*lastMillis / 100.0f + ent->x + ent->y) / 20);
	flNewZ += (&(*reinterpret_cast< sqr** >(OFFSET_WORLD))[((ent->y) << (*reinterpret_cast< DWORD* >(OFFSET_SFACTOR))) + (ent->x)])->floor + ent->attr1;

	flNewZ += 0.5f;

	Vector3D worldCoords = Vector3D(ent->x, ent->y, flNewZ);

	Vector3D centerScreenCoord;
	if (GameFunctions::WorldToScreen(worldCoords, centerScreenCoord))
	{
		float dist = worldCoords.dist(localPlayer->PosHead);
		float width = 400 / dist * windowScale;

		Vector3D topLeft = Vector3D(centerScreenCoord.x - width, centerScreenCoord.y - width, 0);
		Vector3D bottomRight = Vector3D(centerScreenCoord.x + width, centerScreenCoord.y + width, 0);

		float r, g, b;

		r = 0.0f;
		g = 1.0f;
		b = 0.0f;

		Drawing::DrawBoxOutline(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, 2, r, g, b, 0.7f);
		Drawing::DrawBox(topLeft.x, topLeft.y, bottomRight.x, bottomRight.y, r, g, b, 0.3f);

		std::string str;

		switch (ent->type)
		{
		case I_AMMO:
			str = "Ammo";
			break;

		case I_CLIPS:
			str = "Clips";
			break;

		case I_HEALTH:
			str = "Health";
			break;

		case I_ARMOUR:
		case I_HELMET:
			str = "Armor";
			break;

		case I_AKIMBO:
			str = "Akimbo";
			break;

		case I_GRENADE:
			str = "Grenade";
			break;
		}

		int textWidth = str.length() * 8;
		int difference = (width * 2) - textWidth;

		Drawing::DrawString(topLeft.x + difference / 2, bottomRight.y + 12, 0.5f, 1.0f, 0.0f, 1.0f, "%s", str.c_str());
	}
}