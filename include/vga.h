#ifndef _VGA_H
#define _VGA_H

#include <stdint.h>
#include <size_t.h>
#include <vector2.h>

typedef enum
{
    SOLID = 0,
    DOTTED = 1,
    DASHED = 2
} LineType;

typedef struct
{
    uint16_t width;
    uint16_t height;
} RectangleDimensions;

// Sets the pixel at the specified x, y co-ordinate to the specified colour
void VGA_SetPixel(Vector2 position);

// Sets the drawing colour of the below method
void VGA_SetColour(uint8_t colour);

// Sets the type of line to be drawn
void VGA_SetLineThickness(uint8_t thickness);

// Draws a line between two co-ordinates.
void VGA_DrawLine(Vector2 start, Vector2 end, LineType type);

// Draws a rectangle of a specified width and height with its top left-hand corner at co-ordinate x, y using the specified colour. Optional fill of the rectangle.
void VGA_DrawRectangle(Vector2 topLeft, RectangleDimensions dimensions, bool fill);

// Clear the screen to the specified colour
void VGA_ClearScreen();

// Draws a circle with a specified radius at the specified centre point. Optional fill of the circle.
void VGA_DrawCircle(Vector2 centre, uint16_t radius, bool fill);

// Fills a polygon using Scanline fill
void VGA_FillPolygonScanline(Vector2* vertices, size_t vertexCount);

// Draws a polygon from provided vertices. Optional fill of the polygon (Default: false)
void VGA_DrawPolygon(Vector2* vertices, size_t vertexCount, bool fill);

#endif