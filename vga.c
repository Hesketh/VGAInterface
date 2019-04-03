#include <vga.h>
#include <vgamodes.h>
#include <colourpallete.h>
#include <_null.h>
#include <math.h>
#include <list.h>

static uint8_t _colour = COLOUR_BLACK;
static uint8_t _thickness = 1;

void VGA_SetPixel(Vector2 position)
{
    if(position.x >= 0 && position.x < screenWidth
    && position.y >= 0 && position.y < screenHeight)
    {
        *(((uint8_t*)0x0A0000) + screenWidth * position.y + position.x) = _colour;
    }
}

void VGA_SetColour(uint8_t colour)
{
    _colour = colour;
}

void VGA_SetLineThickness(uint8_t thickness)
{
    _thickness = thickness;
}

void VGA_DrawLine(Vector2 start, Vector2 end, LineType type)
{
    if(_thickness <= 0)
    {
        // Nothing can be done
        return;
    }

    // We count the amount of pixels between the pixels on a line we draw so that we can do dotted lines
    uint8_t pixelsSinceLast = -1;

    // We count the amount pixels in a row so we can do dashed lines
    uint8_t pixelsConsecutive = 0;

    // We will use bresenham alogorithm to draw the line from the initial to the end point
    Vector2 pointA = start;
    Vector2 pointB = end;

    // For bresenham to work the first points x must be smaller than the second
    if(pointA.x > pointB.x)
    {
        pointA = end;
        pointB = start;
    }

    // Calculate the change in x and y
    uint16_t dx = pointB.x - pointA.x;
    uint16_t dy = pointB.y - pointA.y;

    int epsilon = 0;

    // The position we are currently at
    Vector2 position = pointA;

    // The desired x position
    uint16_t targetX = pointB.x;

    bool swapXY = abs(dx) < abs(dy);
    bool negativeSlope = pointB.y - pointA.y < 0;

    // We have determined we are in octant 3 or 7 therefore we must swap the vertices (for the second time if the octant is 7)
    if (swapXY && negativeSlope)
    {
        //Swap the vertices
        Vector2 temp = pointA;
        pointA = pointB;
        pointB = temp;

        //Update position, target position and gradient
        position = pointA;

        targetX = pointB.x;

        dx = pointB.x - pointA.x;
        dy = pointB.y - pointA.y;
    }

    // To transform the octant we are in to octant 1 we need to swap the x and y values (another time if this was 3 or 7)
    if(swapXY)
    {
        position.x = pointA.y;
        position.y = pointA.x;

        targetX = pointB.y;

        dx = pointB.y - pointA.y;
        dy = pointB.x - pointA.x;
    }

    // We need to reflect in the y axis to transform to octant 1
    if(negativeSlope)
    {
        position.y = -position.y;
        dy = -dy;
    }

    while(position.x <= targetX)
    {
        // This is the positon that will be written too
        Vector2 writePosition = position;

        // We need to re-reflect in the x axis on the writePosition if we reflected in position
        if(negativeSlope)
        {
            writePosition.y = -writePosition.y;
        }

        // If we swapped the x and y in the position earlier, we must reverse that for the write position
        if(swapXY)
        {
            uint16_t tempX = writePosition.x;
            writePosition.x = writePosition.y;
            writePosition.y = tempX;
        }

        if (type == SOLID 
        || (type == DOTTED && pixelsSinceLast == 1)
        || (type == DASHED && (pixelsConsecutive <= 3)))
        {
            VGA_SetPixel(writePosition);
            pixelsSinceLast = 0;
            ++pixelsConsecutive;

            if(_thickness > 1)
            {
                uint8_t thickness = _thickness;
                uint16_t amountUp = 0;      //ceil((thickness - 1) / 2);
                uint16_t amountDown = 0;    //floor((thickness - 1) / 2);

                uint16_t pixelThickness = thickness;
                // Solution while ceil and floor are causing problems
                while(pixelThickness > 1)
                {
                    // Add one to the amount we are adding above the line
                    amountUp += 1;
                    pixelThickness -= 1;

                    // If we still can add one to the amount we are adding below the line
                    if(thickness > 1)
                    {
                        amountDown += 1;
                        pixelThickness -= 1;
                    }
                }

                LineType thicknessType = SOLID;
                if(type == DOTTED)
                {
                    thicknessType = DOTTED;
                }

                uint8_t temp = _thickness;
                VGA_SetLineThickness(1);
                if(swapXY)
                {
                    // Add horizontally if we swapped the x and y
                    VGA_DrawLine((Vector2){writePosition.x, writePosition.y + amountUp}, (Vector2){writePosition.x - amountDown, writePosition.y}, thicknessType);
                }
                else
                {
                    // Add vertically if we didnt
                    VGA_DrawLine((Vector2){writePosition.x, writePosition.y + amountUp}, (Vector2){writePosition.x, writePosition.y - amountDown}, thicknessType);
                }
                VGA_SetLineThickness(temp);
            }
        }
        else
        {
            ++pixelsSinceLast;
            
            if (type == DASHED && pixelsSinceLast > 3)
            {
                pixelsConsecutive = 0;
            }     
        }

        epsilon += dy;

        // If 2*epsilon is bigger than dx we need to do down to the next y
        if(2*epsilon >= dx)
        {
            position.y += 1;
            epsilon -= dx;
        }

        position.x += 1;
    }
}

void VGA_DrawRectangle(Vector2 topLeft, RectangleDimensions dimensions, bool fill)
{
    if(!fill)
    {
        // Draw an unfilled rectangle
        VGA_DrawLine(topLeft, (Vector2){topLeft.x + dimensions.width, topLeft.y}, SOLID);
        VGA_DrawLine(topLeft, (Vector2){topLeft.x, topLeft.y + dimensions.height}, SOLID);
        VGA_DrawLine((Vector2){topLeft.x + dimensions.width, topLeft.y}, (Vector2){topLeft.x + dimensions.width, topLeft.y + dimensions.height}, SOLID);
        VGA_DrawLine((Vector2){topLeft.x, topLeft.y + dimensions.height}, (Vector2){topLeft.x + dimensions.width, topLeft.y + dimensions.height}, SOLID);
        
    }
    else
    {
        // Draw a filled rectangle
        uint8_t temp = _thickness;
        VGA_SetLineThickness(1);
        for(int y = 0; y < dimensions.height; y++)
        {
            VGA_DrawLine((Vector2){topLeft.x, topLeft.y + y}, (Vector2){topLeft.x + dimensions.width, topLeft.y + y}, SOLID);
        }
        VGA_SetLineThickness(temp);
    }
}

void VGA_ClearScreen()
{
    for(uint16_t y = 0; y < screenHeight; ++y)
    {
        for(uint16_t x = 0; x < screenWidth; ++x)
        {
            VGA_SetPixel((Vector2){x, y});
        }
    }
}

void VGA_DrawCircle(Vector2 centre, uint16_t radius, bool fill)
{
    if(!fill)
    {
        // Midpoint Circle Algorithm
        uint16_t x = radius - 1;
        uint16_t y = 0;
        uint16_t dx = 1;
        uint16_t dy = 1;
        int err = dx - (radius << 1);

        while(x >= y)
        {
            VGA_SetPixel((Vector2){centre.x + x, centre.y + y});
            VGA_SetPixel((Vector2){centre.x + y, centre.y + x});
            VGA_SetPixel((Vector2){centre.x - y, centre.y + x});
            VGA_SetPixel((Vector2){centre.x - x, centre.y + y});
            VGA_SetPixel((Vector2){centre.x - x, centre.y - y});
            VGA_SetPixel((Vector2){centre.x - y, centre.y - x});
            VGA_SetPixel((Vector2){centre.x + y, centre.y - x});
            VGA_SetPixel((Vector2){centre.x + x, centre.y - y});

            if(err <= 0)
            {
                y++;
                err += dy;
                dy += 2;
            }

            if(err > 0)
            {
                x--;
                dx += 2;
                err += (-radius << 1) + dx;
            }
        }
    }
    else
    {
        for(int y = -radius; y <= radius; y++)
        {
            for(int x = -radius; x <= radius; x++)
            {
                if(x*x + y*y <= radius*radius)
                {
                    VGA_SetPixel((Vector2){centre.x + x, centre.y + y});
                }
            }
        }
    }
}

// Fills a polygon using Scanline fill
void VGA_FillPolygonScanline(Vector2* vertices, size_t vertexCount)
{
    // ScanLine fill algorithm
    // We need to determine the highest and lowest y points on the shape
    uint16_t minY = screenHeight - 1;
    uint16_t maxY = 0;

    for(size_t i = 0; i < vertexCount; i++)
    {
        uint16_t y = vertices[i].y;

        minY = y < minY ? y : minY;
        maxY = y > maxY ? y : maxY;
    }

    // Clamp the min and max values to the resolution size
    minY = minY > 0 ? minY : 0;
    maxY = maxY < screenHeight - 1 ? maxY : screenHeight - 1;

    List* lut = NULL;

    // Creating the look up tables for scan line fill
    for(uint16_t y = minY; y <= maxY; y++)
    {
        for(size_t i = 0; i < vertexCount; i++)
        {
            Vector2 start = vertices[i];
            Vector2 end = i + 1 < vertexCount ? vertices[i + 1] : vertices[0];

            // We must determine if the scnaline is between the start and the end points y position
            if(start.y <= y && y <= end.y || start.y >= y && y >= end.y)
            {
                // We calculate the intercept of the edge and the scanline
                // Firstly, ensure we dont have a div by 0 error, as that causes the OS to just stop
                // This shouldnt be an issue unless a vertex is added twice (which would cause a whole bunch of problems anyway)
                uint16_t dy = start.y - end.y;
                if(dy == 0)
                {
                    continue;
                }

                uint16_t x = start.x + (y - start.y) * ((start.x - end.x) / (dy));

                // We check to see if we are on the vertex or within the edge and add it to the look up table
                if(start.y == y)
                {
                    // If it is a vertex we dont want to add it to the look up table.
                    // Doing so would result in a pair to draw a line between then starting at the vertex instead of the next pair
                    // Check if we are bigger than both the previous vertices, or smaller than both
                    if(!((vertices[i - 1].y > y && vertices[i + 1].y > y) || (vertices[i - 1].y < y && vertices[i + 1].y < y)))
                    {
                        // Add to the look up table
                        lut = List_Insert(lut, y, x);
                    }
                }
                else if ((start.y < y && y < end.y) || (start.y > y && y > end.y))
                {
                    // Here we determined that the y intersects with an edge
                    lut = List_Insert(lut, y, x);
                }
            }
        }
    }
    
    lut = List_GetFirst(lut);
    
    while(lut)
    {
        List* next = lut->next;

        if(lut->y == next->y)
        {
            VGA_DrawLine((Vector2){lut->x, lut->y}, (Vector2){next->x, next->y}, SOLID);
            //VGA_SetPixel((Vector2){lut->x, lut->y}, colour);
            //VGA_SetPixel((Vector2){next->x, next->y}, colour);
            lut = lut->next;
        }

        if(lut)
        {
            lut = lut->next;
        }
    }
}

void VGA_DrawPolygon(Vector2* vertices, size_t vertexCount, bool fill)
{
    if(vertexCount < 0 || !vertices)
    {
        // We cant do anything with incomplete data
        return;
    }

    if(!fill)
    {
        // We draw a line from one vertex to the other
        for(size_t i = 0; i < vertexCount; i++)
        {
            Vector2 start = vertices[i];
            Vector2 end = i + 1 == vertexCount ? vertices[0] : vertices[i+1];

            VGA_DrawLine(start, end, SOLID);
        }
    }
    else
    {
        // TODO:
        // Until we figure out dynamic memory allocation we are unable to use scan line fill properly
        // as it requires the linked list to create a LUT
        
        VGA_FillPolygonScanline(vertices, vertexCount);
    }
}