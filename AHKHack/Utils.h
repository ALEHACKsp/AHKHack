#pragma once
#ifndef UTILS_H
#define UTILS_H

namespace Utils
{
	bool DetourFunction(void* toHook, void* hookHandler, int len);

	bool UnDetourFunction(void* hookedFunc);
}

#endif
