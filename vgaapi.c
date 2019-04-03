#include <vgaapi.h>
#include <hal.h>
#include <vga.h>

#define MAX_VGACALL 8

typedef struct _VGACallInfo
{
	void * VGACall;
	int    ParamCount;
} VGACallInfo;

VGACallInfo _VGACalls[MAX_VGACALL]; 

void InitialiseVGACall(int index, void * vgaCall, int paramCount)
{
	if (index >= 0 && index < MAX_VGACALL)
	{
		_VGACalls[index].VGACall = vgaCall;
		_VGACalls[index].ParamCount = paramCount;
	}
}

void VGACallDispatcher() 
{
	static int index = 0;
	// Get index into _VGACalls table from eax
	asm volatile("movl %%eax, %0" : "=r" (index));

	if (index < MAX_VGACALL)
	{
		// Temporarily save the registers that are used to pass in the parameters
		asm volatile ("push %edx\n\t"
					  "push %ecx\n\t"
					  "push %ebx\n\t");
		void * function = _VGACalls[index].VGACall;
		int paramCount = _VGACalls[index].ParamCount;
		// Now generate the code for the user call.  There is different
		// code depending on how many parameters are passed to the function.
		// After the call to the kernel routine, we remove the parameters from the
		// stack by adjusting the stack pointer.  This is the standard C calling convention.
		switch (paramCount)
		{
			case 3:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "push %%edx\n\t"
							  "push %%ecx\n\t"
							  "push %%ebx\n\t"
							  "call *%0\n\t"
							  "addl $12, %%esp"
							  : : "r"(function)
							  );
							  break;
				
			case 2:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "push %%ecx\n\t"
							  "push %%ebx\n\t"
							  "call *%0\n\t"
							  "addl $8, %%esp"
							  : : "r"(function)
							  );
							  break;
							  
			case 1:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "push %%ebx\n\t"
							  "call *%0\n\t"
							  "addl $4, %%esp"
							  : : "r"(function)
							  );
							  break;
							  
			case 0:
				asm volatile ("pop %%ebx\n\t"
							  "pop %%ecx\n\t"
							  "pop %%edx\n\t"
							  "call *%0\n\t"
							  : : "r"(function)
							  );
							  break;
		}
	}
	asm("leave");
	asm("iret");
}

#ifndef I86_IDT_DESC_RING3
#define I86_IDT_DESC_RING3		0x60
#endif

void InitialiseVGACalls()
{
	InitialiseVGACall(0, VGA_SetPixel, 1);
    InitialiseVGACall(1, VGA_DrawLine, 3);
    InitialiseVGACall(2, VGA_DrawRectangle, 3);
    InitialiseVGACall(3, VGA_ClearScreen, 0);
    InitialiseVGACall(4, VGA_DrawCircle, 3);
    InitialiseVGACall(5, VGA_DrawPolygon, 3);
	InitialiseVGACall(6, VGA_SetColour, 1);
	InitialiseVGACall(7, VGA_SetLineThickness, 1);
    
	HAL_SetInterruptVector(0x81, VGACallDispatcher, I86_IDT_DESC_RING3);
}