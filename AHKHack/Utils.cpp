#include "Utils.h"
#include <Windows.h>
#include <vector>

namespace Utils
{
	struct Detour
	{
		byte originalBytes[64];
		int hookLength;
		void* hookFunction;
		void* hookHandler;
	};

	std::vector<Detour> activeDetours;

	bool DetourFunction(void* toHook, void* hookHandler, int len)
	{
		if (len < 5) 
		{
			return false;
		}

		DWORD currentProt;
		VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &currentProt);

		Detour detour = Detour();
		detour.hookLength = len;
		detour.hookFunction = toHook;
		detour.hookHandler = hookHandler;

		memcpy(detour.originalBytes, toHook, len);

		activeDetours.push_back(detour);

		memset(toHook, 0x90, len);

		DWORD relativeAddress = ((DWORD)hookHandler - (DWORD)toHook) - 5;
		*(BYTE*)toHook = 0xE9;
		*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

		DWORD temp;
		VirtualProtect(toHook, len, currentProt, &temp);
		return true;
	}

	bool UnDetourFunction(void* hookedFunc)
	{
		for (auto it = activeDetours.begin(); it < activeDetours.end(); ++it)
		{
			Detour detour = *it;

			if ((DWORD)detour.hookFunction == (DWORD)hookedFunc)
			{
				DWORD oldProt;
				VirtualProtect(hookedFunc, detour.hookLength, PAGE_EXECUTE_READWRITE, &oldProt);

				// Copy old bytes back
				memcpy(hookedFunc, detour.originalBytes, detour.hookLength);

				VirtualProtect(hookedFunc, detour.hookLength, oldProt, &oldProt);

				activeDetours.erase(it);

				return true;
			}
		}

		return false;
	}
}
