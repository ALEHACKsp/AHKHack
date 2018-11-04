#include "NoRecoil.h"
#include <iostream>

NoRecoil::NoRecoil()
{
	this->hackName = "No Recoil";
	patchRecoil(true);
}

void NoRecoil::patchRecoil(bool patch)
{		
	// Open protection
	DWORD oldProt;
	VirtualProtect((void*)OFFSET_NORECOIL, 10, PAGE_EXECUTE_READWRITE, &oldProt);

	if (patch == true)
	{
		// Copy the bytes
		memcpy(originalBytes, (void*)OFFSET_NORECOIL, 10);

		// NOP it out
		memset((void*)OFFSET_NORECOIL, 0x90, 10);
	}
	else
	{
		// Open protection
		DWORD oldProt;
		VirtualProtect((void*)OFFSET_NORECOIL, 10, PAGE_EXECUTE_READWRITE, &oldProt);

		// Copy the bytes
		memcpy((void*)OFFSET_NORECOIL, originalBytes, 10);
	}

	// Set the protection back
	DWORD temp;
	VirtualProtect((void*)OFFSET_NORECOIL, 10, oldProt, &temp);
}

void NoRecoil::onOptionChanged(int optionIndex)
{
	if(optionIndex == 0)
		patchRecoil(options[0]->asBool);
}
