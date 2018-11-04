#include "AllWeaponsAuto.h"

AllWeaponsAuto::AllWeaponsAuto()
{
	this->hackName = "All Weapons Auto";
	onOptionChanged(0);
}

void AllWeaponsAuto::onOptionChanged(int optionIndex)
{
	if (optionIndex == 0)
		patchMemory();
}

void AllWeaponsAuto::patchMemory()
{
	// Enable protection to allow read/write
	DWORD oldProt;
	VirtualProtect((void*)OFFSET_WEAPONAUTO, 6, PAGE_EXECUTE_READWRITE, &oldProt);

	if (options[0]->asBool)
	{
		// Copy old bytes
		memcpy(originalBytes, (void*)OFFSET_WEAPONAUTO, 6);

		// NOP out bytes
		memset((void*)OFFSET_WEAPONAUTO, 0x90, 6);
	}
	else
	{
		// Copy old bytes back
		memcpy((void*)OFFSET_WEAPONAUTO, originalBytes, 6);
	}

	VirtualProtect((void*)OFFSET_WEAPONAUTO, 6, oldProt, &oldProt);
}

