#include <string.h>
#include <console.h>
#include <hal.h>
#include <exception.h>
#include <keyboard.h>
#include <sysapi.h>
#include <user.h>
#include <vgamodes.h>
#include <vgaapi.h>
#include <uservga.h>
#include <colourpallete.h>
#include "physicalmemorymanager.h"
#include "virtualmemorymanager.h"
#include "bootinfo.h"

// This is a dummy __main.  For some reason, gcc puts in a call to 
// __main from main, so we just include a dummy.
 
BootInfo *	_bootInfo;
 
void __main() {}

void InitialisePhysicalMemory()
{
	// Initialise the physical memory manager
	// We place the memory bit map in the next available block of memory after the kernel.
	// Note that all memory addresses here are physical memory addresses since we are dealing
	// with management of physical memory

	uint32_t memoryMapAddress = 0x100000 + _bootInfo->KernelSize;
	if (memoryMapAddress % PMM_GetBlockSize() != 0)
	{
		// Make sure that the memory map is going to be aligned on a block boundary
		memoryMapAddress = (memoryMapAddress / PMM_GetBlockSize() + 1) * PMM_GetBlockSize();
	}
	uint32_t sizeOfMemoryMap = PMM_Initialise(_bootInfo, memoryMapAddress);

	// We now need to mark various regions as unavailable
	
	// Mark first page as unavailable (so that a null pointer is invalid)
	PMM_MarkRegionAsUnavailable(0x00, PMM_GetBlockSize());

	// Mark memory used by kernel as unavailable
	PMM_MarkRegionAsUnavailable(0x100000, _bootInfo->KernelSize);

	// Mark memory used by memory map as unavailable
	PMM_MarkRegionAsUnavailable(PMM_GetMemoryMap(), sizeOfMemoryMap);

	// Reserve two blocks for the stack and make unavailable (the stack is set at 0x90000 in boot loader)
	uint32_t stackSize = PMM_GetBlockSize() * 2;
	PMM_MarkRegionAsUnavailable(_bootInfo->StackTop - stackSize, stackSize);
	
	// Reserve two block for the kernel stack and make unavailable
	PMM_MarkRegionAsUnavailable(0x80000 - stackSize, stackSize);
}
void Initialise()
{
	ConsoleClearScreen(0x1F);
	ConsoleWriteString("UODOS 32-bit Kernel. Kernel size is ");
	ConsoleWriteInt(_bootInfo->KernelSize, 10);
	ConsoleWriteString(" bytes\n");
	HAL_Initialise();
	InitialisePhysicalMemory();
	VMM_Initialise();
	KeyboardInstall(33); 
	InitialiseSysCalls();
	InitialiseVGACalls();
}

#include <list.h>
void main(BootInfo * bootInfo) 
{
	_bootInfo = bootInfo;
	Initialise();

	// Enter User Mode
	HAL_EnterUserMode();
	User_ConsoleWriteString("Now running in user mode\n");
	
	User_ConsoleWriteString("\n\nPress Any Key to advance through the following tests...");
	KeyboardGetCharacter();

	// Swap to VGA Graphics Mode
	VGA_SetGraphicsMode(320, 200, 1);

	// Clear Screen Test
	User_VGASetColour(COLOUR_GREEN);
	User_VGAClearScreen();
	User_VGASetColour(COLOUR_RED);

	// SOLID line Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK);
	User_VGAClearScreen();
	User_VGASetColour(COLOUR_RED);
	User_VGADrawLine((Vector2){37,1}, (Vector2){98,197}, SOLID);
	User_VGASetLineThickness(2);
	User_VGADrawLine((Vector2){101,0}, (Vector2){75,197}, SOLID);
	User_VGADrawLine((Vector2){144,4}, (Vector2){114,194}, SOLID);
	User_VGASetColour(COLOUR_GREEN);
	User_VGADrawLine((Vector2){170,194}, (Vector2){151,6}, SOLID);
	User_VGASetLineThickness(3);
	User_VGADrawLine((Vector2){264,1}, (Vector2){177,198}, SOLID);
	User_VGADrawLine((Vector2){3,50}, (Vector2){300,50}, SOLID);
	User_VGASetLineThickness(2);
	User_VGADrawLine((Vector2){25,165}, (Vector2){303,91}, SOLID);
	User_VGASetLineThickness(1);
	User_VGADrawLine((Vector2){273,171}, (Vector2){20,74}, SOLID);

	// Dotted Line Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK); 	
	User_VGAClearScreen(); 	
	User_VGASetColour(COLOUR_RED);;
	User_VGADrawLine((Vector2){37,1}, (Vector2){98,197}, DOTTED);
	User_VGADrawLine((Vector2){101,0}, (Vector2){75,197}, DOTTED);
	User_VGASetLineThickness(2);
	User_VGADrawLine((Vector2){144,4}, (Vector2){114,194}, DOTTED);
	User_VGADrawLine((Vector2){170,194}, (Vector2){151,6}, DOTTED);
	User_VGADrawLine((Vector2){264,1}, (Vector2){177,198}, DOTTED);
	User_VGASetLineThickness(3);
	User_VGADrawLine((Vector2){3,50}, (Vector2){300,50}, DOTTED);
	User_VGADrawLine((Vector2){25,165}, (Vector2){303,91}, DOTTED);
	User_VGASetLineThickness(1);
	User_VGADrawLine((Vector2){273,171}, (Vector2){20,74}, DOTTED);

	// Dashed Line Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK);
	User_VGAClearScreen();
	User_VGASetColour(COLOUR_RED);
	User_VGASetLineThickness(3);
	User_VGADrawLine((Vector2){37,1}, (Vector2){98,197}, DASHED);
	User_VGADrawLine((Vector2){101,0}, (Vector2){75,197}, DASHED);
	User_VGADrawLine((Vector2){144,4}, (Vector2){114,194}, DASHED);
	User_VGASetLineThickness(2);
	User_VGADrawLine((Vector2){170,194}, (Vector2){151,6}, DASHED);
	User_VGADrawLine((Vector2){264,1}, (Vector2){177,198}, DASHED);
	User_VGADrawLine((Vector2){3,50}, (Vector2){300,50}, DASHED);
	User_VGADrawLine((Vector2){25,165}, (Vector2){303,91}, DASHED);
	User_VGASetLineThickness(1);
	User_VGADrawLine((Vector2){273,171}, (Vector2){20,74}, DASHED);

	// Rectangle Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK);
	User_VGAClearScreen();
	User_VGASetColour(COLOUR_RED);;
	User_VGADrawRectangle((Vector2){10,10}, (RectangleDimensions){30, 50}, false);
	User_VGADrawRectangle((Vector2){50,10}, (RectangleDimensions){60, 10}, false);
	User_VGADrawRectangle((Vector2){50,70}, (RectangleDimensions){40, 40}, true);
	User_VGADrawRectangle((Vector2){100,100}, (RectangleDimensions){175, 75}, true);

	// Circle Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK); 
	User_VGAClearScreen(); 
	User_VGASetColour(COLOUR_RED);;
	User_VGADrawCircle((Vector2){20,20}, 20, false);
	User_VGADrawCircle((Vector2){200,100}, 50, true);

	// Polygon Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK);
	User_VGAClearScreen();
	User_VGASetColour(COLOUR_RED);;

	Vector2 pentagon[] = 
	{
		(Vector2){50, 50},
		(Vector2){50, 150},
		(Vector2){150, 150},
		(Vector2){100, 100},
		(Vector2){150, 50}
	};
	User_VGADrawPolygon(pentagon, 5, false);

	Vector2 complex[] = 
	{
		(Vector2){162, 22},
		(Vector2){170, 30},
		(Vector2){162, 40},
		(Vector2){180, 150},
		(Vector2){195, 75},
		(Vector2){230, 150},
		(Vector2){230, 80},
		(Vector2){190, 22}
	};
	User_VGADrawPolygon(complex, 8, false);

	// Filled Polygon Test
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_BLACK); 
	User_VGAClearScreen(); 
	User_VGASetColour(COLOUR_RED);
	User_VGADrawPolygon(pentagon, 5, true);
	User_VGADrawPolygon(complex, 8, true);

	//END
	KeyboardGetCharacter();
	User_VGASetColour(COLOUR_CYAN); 
	User_VGAClearScreen(); 
	User_VGASetColour(COLOUR_RED);

	while(1);
}
