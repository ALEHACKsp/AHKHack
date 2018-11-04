#pragma once
#ifndef CLASSES_H
#define CLASSES_H

#include "Offsets.h"
#include <math.h>
#include <Windows.h>

// network messages codes, c2s, c2c, s2c
enum
{
	SV_SERVINFO = 0, SV_WELCOME, SV_INITCLIENT, SV_POS, SV_POSC, SV_POSN, SV_TEXT, SV_TEAMTEXT, SV_TEXTME, SV_TEAMTEXTME, SV_TEXTPRIVATE,
	SV_SOUND, SV_VOICECOM, SV_VOICECOMTEAM, SV_CDIS,
	SV_SHOOT, SV_EXPLODE, SV_SUICIDE, SV_AKIMBO, SV_RELOAD, SV_AUTHT, SV_AUTHREQ, SV_AUTHTRY, SV_AUTHANS, SV_AUTHCHAL,
	SV_GIBDIED, SV_DIED, SV_GIBDAMAGE, SV_DAMAGE, SV_HITPUSH, SV_SHOTFX, SV_THROWNADE,
	SV_TRYSPAWN, SV_SPAWNSTATE, SV_SPAWN, SV_SPAWNDENY, SV_FORCEDEATH, SV_RESUME,
	SV_DISCSCORES, SV_TIMEUP, SV_EDITENT, SV_ITEMACC,
	SV_MAPCHANGE, SV_ITEMSPAWN, SV_ITEMPICKUP,
	SV_PING, SV_PONG, SV_CLIENTPING, SV_GAMEMODE,
	SV_EDITMODE, SV_EDITXY, SV_EDITARCH, SV_EDITBLOCK, SV_EDITD, SV_EDITE, SV_NEWMAP,
	SV_SENDMAP, SV_RECVMAP, SV_REMOVEMAP,
	SV_SERVMSG, SV_ITEMLIST, SV_WEAPCHANGE, SV_PRIMARYWEAP,
	SV_FLAGACTION, SV_FLAGINFO, SV_FLAGMSG, SV_FLAGCNT,
	SV_ARENAWIN,
	SV_SETADMIN, SV_SERVOPINFO,
	SV_CALLVOTE, SV_CALLVOTESUC, SV_CALLVOTEERR, SV_VOTE, SV_VOTERESULT,
	SV_SETTEAM, SV_TEAMDENY, SV_SERVERMODE,
	SV_IPLIST,
	SV_LISTDEMOS, SV_SENDDEMOLIST, SV_GETDEMO, SV_SENDDEMO, SV_DEMOPLAYBACK,
	SV_CONNECT,
	SV_SWITCHNAME, SV_SWITCHSKIN, SV_SWITCHTEAM,
	SV_CLIENT,
	SV_EXTENSION,
	SV_MAPIDENT, SV_HUDEXTRAS, SV_POINTS,
	SV_NUM
};

enum                            // static entity types
{
	NOTUSED = 0,                // entity slot not in use in map
	LIGHT,                      // lightsource, attr1 = radius, attr2 = intensity
	PLAYERSTART,                // attr1 = angle, attr2 = team
	I_CLIPS, I_AMMO, I_GRENADE,
	I_HEALTH, I_HELMET, I_ARMOUR, I_AKIMBO,
	MAPMODEL,                   // attr1 = angle, attr2 = idx
	CARROT,                     // attr1 = tag, attr2 = type
	LADDER,
	CTF_FLAG,                   // attr1 = angle, attr2 = red/blue
	SOUND,
	CLIP,
	PLCLIP,
	MAXENTTYPES
};

enum { INBASE = 0, STOLEN = 1, DROPPED = 2, IDLE = 3 };

class ViewMatrix
{
public:
	float matrix[16]; //0x00
};

class Vector3D
{
public:
	float x;
	float y;
	float z;

	Vector3D()
	{
		this->x = 0;
		this->y = 0;
		this->z = 0;
	};

	Vector3D(float px, float py, float pz)
	{
		this->x = px;
		this->y = py;
		this->z = pz;
	};

	Vector3D sub(Vector3D v)
	{
		Vector3D p = Vector3D(this->x, this->y, this->z);
		p.x -= v.x;
		p.y -= v.y;
		p.z -= v.z;
		return p;
	};

	float dot(Vector3D vec)
	{
		float temp = 0;
		temp = this->x*vec.x + this->y*vec.y + this->z*vec.z;
		return temp;
	};

	float dist(Vector3D vec)
	{
		return sqrt((this->x - vec.x) * (this->x - vec.x) + (this->y - vec.y) * (this->y - vec.y) + (this->z - vec.z) * (this->z - vec.z));
	}

	Vector3D mul(Vector3D v)
	{
		return Vector3D(x * v.x, y * v.y, z * v.z);
	};

	Vector3D mul(float f)
	{
		return Vector3D(x * f, y * f, z * f);
	};

	Vector3D div(Vector3D v)
	{
		return Vector3D(x / v.x, y / v.y, z / v.z);
	};

	float Length()
	{
		float flLengthX, flLengthY, flLengthZ, flLength;

		flLengthX = x * x;
		flLengthY = y * y;
		flLengthZ = z * z;

		flLength = sqrt(flLengthX + flLengthY + flLengthZ);

		return fabs(flLength);
	}

	void Normalize()
	{
		float length, ilength;

		length = this->Length();

		if (length)
		{
			ilength = 1 / length;

			this->x *= ilength;
			this->y *= ilength;
			this->z *= ilength;
		}
	}

	Vector3D &div(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
};

namespace AssaultCube
{
	class HitMessage
	{
	public:
		int target;
		int lifeSequence;
		int info;
		Vector3D dir;
	};

	class WeaponInfo
	{
	public:
		char name[260]; //0x3D5888 
		__int16 sound; //0x0104 
		__int16 reload; //0x0106 
		__int16 reloadTime; //0x0108 
		__int16 attackDelay; //0x010A 
		__int16 damage; //0x010C 
		__int16 piercing; //0x010E 
		__int16 projSpeed; //0x0110 
		__int16 part; //0x0112 
		__int16 spread; //0x0114 
		__int16 recoil; //0x0116 
		__int16 magSize; //0x0118 
		__int16 mdlKickRot; //0x011A 
		__int16 mdlKickBack; //0x011C 
		__int16 recoilIncrease; //0x011E 
		__int16 recoilBase; //0x120
		__int16 maxRecoil; //0x0122 
		__int16 recoilBackFade; //0x0124 
		__int16 pushFactor; //0x0126
		bool isAuto; //0x0128

	}; //Size=0x0127

	class ACPlayer;

	class Weapon
	{
	public:
		virtual void Function0(); //
		virtual void Function1(); //
		virtual void Function2(); //
		virtual bool Attack(Vector3D &target); //
		virtual void AttackFx(const Vector3D &from, const Vector3D &to, int millis);
		virtual void AttackPhysics(Vector3D &from, Vector3D &to);
		virtual void AttackSound();
		virtual bool Reload(bool autoReloaded); //

		int type; //0x0004 
		ACPlayer* player; //0x0008
		WeaponInfo * info; //0x000C 
		int* pReserveAmmo; //0x0010 
		int* pLoadedAmmo; //0x0014 
		int* pNextFire; //0x0018 
	};

	class ACPlayer
	{
	public:
		DWORD vTable; //0x0000
		Vector3D PosHead; //0x0004 
		Vector3D Velocity; //0x0010 
		char pad_0x001C[0x18]; //0x001C
		Vector3D PosFeet; //0x0034 
		float yaw; //0x0040 
		float pitch; //0x0044 
		char pad_0x0048[0xB0]; //0x0048
		int Health; //0x00F8 
		char pad_0x00FC[0x18]; //0x00FC
		int PistolReserveAmmo; //0x0114 
		char pad_0x0118[0x10]; //0x0118
		int ARReserveAmmo; //0x0128 
		char pad_0x012C[0x10]; //0x012C
		int PistolLoadedAmmo; //0x013C 
		char pad_0x0140[0x10]; //0x0140
		int ARLoadedAmmo; //0x0150 
		char pad_0x0154[0x4]; //0x0154
		int Grenades; //0x0158 
		char pad_0x015C[0x88]; //0x015C
		int clientNum; //0x01E4 
		int lastUpdate; //0x01E8 
		int plag; //0x01EC 
		int ping; //0x01F0 
		int address; //0x01F4 
		int lifeSequence; //0x01F8 
		int frags; //0x01FC 
		int flagScore; //0x0200 
		int deaths; //0x0204 
		int points; //0x0208 
		int tks; //0x020C 
		int lastAction; //0x0210 
		int lastMove; //0x0214 
		int lastPain; //0x0218 
		int lastVoiceCom; //0x021C 
		int clientRole; //0x0220 
		BYTE shoot; //0x0224 
		char name[16]; //0x3D5888 
		char pad_0x0235[0xF7]; //0x0235
		__int8 team; //0x032C 
		char pad_0x032D[0xB]; //0x032D
		BYTE bDead; //0x0338 
		char pad_0x0339[0x3B]; //0x0339
		Weapon* pWeapon; //0x0374 
		char pad_0x0378[0xC0]; //0x0378

		float GetDistanceFromCrosshair();

	}; //Size=0x0420

	class Entity
	{
	public:
		__int16 x; //0x0000 
		__int16 y; //0x0002 
		__int16 z; //0x0004 
		__int16 attr1; //0x0006 
		bool type; //0x0008 
		bool attr2; //0x0009 
		bool attr3; //0x000A 
		bool attr4; //0x000B 
		bool spawned; //0x000C 
		char pad_0x000D[0x3]; //0x000D
		int lastmillis; //0x0010 
	}; //Size=0x0040

	class FlagEnt
	{
	public:
		int team; //0x0000
		Entity* mapent; //0x0004
		int number; //0x0008
		ACPlayer* player; //0x000C
		Vector3D pos; //0x0010
		int state; //0x001C
		int unknown2; //0x0020
	};//Size=0x0024

	struct sqr
	{
		unsigned char type;
		char floor, ceil;
		unsigned char wtex, ftex, ctex;
		unsigned char r, g, b;
		unsigned char vdelta;
		char defer;
		char occluded;
		unsigned char utex;
		unsigned char tag;
		unsigned char reserved[2];
	};
}

#endif