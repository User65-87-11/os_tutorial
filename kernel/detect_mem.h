#pragma once

#include "../cpu/types.h"

/**
NEEDS A FIX
RETURNS 0

*/

typedef struct SMAP_entry SMAP_entry_t;

int __attribute__((noinline)) __attribute__((regparm(3))) detectMemory(SMAP_entry_t* buffer, int maxentries);

typedef struct SMAP_entry {

	u32 BaseL; // base address uint64_t
	u32 BaseH;
	// union{
	// 	u64 LengthLH;
	// 	struct{
	// 		u32 LengthL; // length uint64_t
	// 		u32 LengthH;
	// 	};
	// };

	u32 LengthL; // length uint64_t
	u32 LengthH;
	u32 Type; // entry Type
	u32 ACPI; // extended

}__attribute__((packed)) SMAP_entry_t;



#ifdef DETECT_MEMORY_IMPLEMENTATION





// load memory map to buffer - note: regparm(3) avoids stack issues with gcc in real mode
int __attribute__((noinline)) __attribute__((regparm(3))) detectMemory(SMAP_entry_t* buffer, int maxentries)
{
	u32 contID = 0;
	int entries = 0, signature, bytes;
	do 
	{
		__asm__ __volatile__ ("int  $0x15" 
				: "=a"(signature), "=c"(bytes), "=b"(contID)
				: "a"(0xE820), "b"(contID), "c"(24), "d"(0x534D4150), "D"(buffer));
		if (signature != 0x534D4150) 
			return -1; // error
		if (bytes > 20 && (buffer->ACPI & 0x0001) == 0)
		{
			// ignore this entry
		}
		else {
			buffer++;
			entries++;
		}
	} 
	while (contID != 0 && entries < maxentries);
	return entries;
}

#endif