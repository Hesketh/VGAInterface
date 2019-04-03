#ifndef _VGAMODES_H
#define _VGAMODES_H
#include <stdint.h>

extern uint16_t screenWidth;
extern uint16_t screenHeight;

int VGA_SetGraphicsMode(uint16_t width, uint16_t height, uint8_t chain4);

#endif
