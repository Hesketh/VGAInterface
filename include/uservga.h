#ifndef _USERVGA_H
#define _USERVGA_H

#include <vector2.h>
#include <vga.h>

void User_VGASetPixel(const Vector2 position);
void User_VGASetColour(const uint8_t colour);
void User_VGASetLineThickness(const uint8_t thickness);
void User_VGADrawLine(const Vector2 start, const Vector2 end, const LineType type);
void User_VGADrawRectangle(const Vector2 topLeft, const RectangleDimensions dimensions, const bool fill);
void User_VGAClearScreen();
void User_VGADrawCircle(const Vector2 centre, const uint16_t radius, const bool fill);
void User_VGADrawPolygon(const Vector2* vertices, const size_t vertexCount, const bool fill);

#endif