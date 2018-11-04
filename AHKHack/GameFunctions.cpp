#include "GameFunctions.h"
#include "Game.h"
#include <gl\GL.h>

using namespace AssaultCube;

namespace GameFunctions
{
	typedef void(__cdecl * _contoutf)(const char* string, ...);
	_contoutf contoutf = (_contoutf)0x46b060;

	typedef AssaultCube::ACPlayer*(__cdecl* _getCrosshairEntity)();
	_getCrosshairEntity getCrosshairEntity = (_getCrosshairEntity)0x4607C0;

	typedef bool(__cdecl * _sendMessage)(int type, const char *fmt, ...);
	_sendMessage sendMessage = (_sendMessage)OFFSET_ADDMSG;

	void OutputToConsole(const char* string)
	{
		contoutf(string);
	}

	AssaultCube::ACPlayer* GetEntityPlayerAimingAt() 
	{
		return getCrosshairEntity();
	}

	bool IsTeamGame()
	{
		int gameMode = *reinterpret_cast<int*>(OFFSET_GAMEMODE);
		return (gameMode == 0 || gameMode == 4 || gameMode == 5 || gameMode == 7 || gameMode == 11 || gameMode == 13 || gameMode == 14 || gameMode == 16 || gameMode == 17 || gameMode == 20 || gameMode == 21);
	}

	bool IsVisible(Vector3D from, Vector3D to)
	{
		__asm
		{
			push to.z;
			push to.y;
			push to.x;

			push from.z;
			push from.y;
			push from.x;

			xor cl, cl;   //Tracer
			xor eax, eax;
			mov ebx, OFFSET_ISVISIBLE;
			call ebx;
			add esp, 0x18;
		}
	}

	void SendShoot(ACPlayer* target, int millis, bool gib)
	{
		// Convert to to a velocity vector;
		Vector3D vel(target->PosHead);
		vel.z += 0.2f;
		vel.sub(localPlayer->PosHead);
		vel.Normalize();

		// Make a HitMessage instance
		HitMessage hitMsg = { 0 };
		hitMsg.lifeSequence = target->lifeSequence;
		hitMsg.target = target->clientNum;
		hitMsg.info = gib ? 1 : 0;
		hitMsg.dir = Vector3D(int(vel.x * 16.0f), int(vel.y * 16.0f), int(vel.z * 16.0f));

		// Send the message
		sendMessage(SV_SHOOT, "ri2i3iv", millis, localPlayer->pWeapon->type,
			(int)(target->PosHead.x * 16.0f),
			(int)(target->PosHead.y * 16.0f),
			(int)((target->PosHead.z + 0.2f) * 16.0f),
			1, // one hit message
			1 * sizeof(HitMessage) / sizeof(int),
			&hitMsg);
	}

	bool WorldToScreen(Vector3D pos, Vector3D &screen)
	{
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		float xCoord = pos.x*viewMatrix->matrix[0] + pos.y*viewMatrix->matrix[4] + pos.z*viewMatrix->matrix[8] + viewMatrix->matrix[12];
		float yCoord = pos.x*viewMatrix->matrix[1] + pos.y*viewMatrix->matrix[5] + pos.z*viewMatrix->matrix[9] + viewMatrix->matrix[13];
		float wCoord = pos.x*viewMatrix->matrix[3] + pos.y*viewMatrix->matrix[7] + pos.z*viewMatrix->matrix[11] + viewMatrix->matrix[15];

		if (wCoord < 0.1f)
			return false;

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		Vector3D NDC;
		NDC.x = xCoord / wCoord;
		NDC.y = yCoord / wCoord;

		// Get Window Width/Height from OpenGL
		GLfloat viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);

		//Transform to window coordinates
		screen.x = (viewport[2] / 2 * NDC.x) + (NDC.x + viewport[2] / 2);
		screen.y = -(viewport[3] / 2 * NDC.y) + (NDC.y + viewport[3] / 2);
		return true;
	}

	Vector3D GetCrosshair() 
	{
		// Get Window Width/Height from OpenGL
		GLfloat viewport[4];
		glGetFloatv(GL_VIEWPORT, viewport);

		int windowWidth = viewport[2];
		int windowHeight = viewport[3];

		Vector3D crosshair = Vector3D(windowWidth / 2, windowHeight / 2, 0);

		return crosshair;
	}
}