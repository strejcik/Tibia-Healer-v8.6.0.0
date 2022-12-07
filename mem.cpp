#include "pch.h"

uintptr_t mem::FindDMAAddy2(uintptr_t ptr, std::vector<uint32_t> offsets)
{
	uintptr_t addr = ptr;
	uintptr_t* pointer;
	for (uint32_t i = 0; i < offsets.size(); ++i)
	{
		pointer = (uintptr_t*)addr;
		if (!IsBadReadPtr(pointer, sizeof(pointer)))
		{
			addr = *(uintptr_t*)addr;
			addr += offsets[i];
		}
	}
	if (IsBadReadPtr((uintptr_t*)addr, sizeof(addr))) return 0;
	return addr;
}