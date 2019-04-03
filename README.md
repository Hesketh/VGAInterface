# VGAInterface
![Preview](https://github.com/Hesketh/VGAInterface/blob/master/images/combined.png?raw=true)

As a part of the operating system being developed throughout my Systems Programming module during 3rd year at the University of Derby, I implemented a library of low level VGA drawing primitive functions.

## Protection Levels
To draw to the VGA buffer it is a requirement that this code runs in ring 0, however it was a requirement that ring 3 code could utilise this functionality. To achieve this a custom interupt table has been used that ring 3 functions call to execute the ring 0 functions.

## Features
* Setting the colour of an individual pixel (SetPixel)
* Drawing a line from one point on screen to another point on screen (DrawLine)
* Drawing the line in different styles (Dotted, Dashed, Solid)
* Drawing rectangles that are both unfilled and filled (DrawRectangle)
* Clearing the screen to a colour (ClearScreen)
* Drawing unfilled and filled circles (DrawCircle)
* Drawing unfilled and filled polygons of arbitrary size, vertex and line count. Regardless of whether they were concave or convex.
