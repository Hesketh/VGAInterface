#include <math.h>

uint16_t abs(uint16_t value)
{
    return (value < 0) ? -value : value;
}

// Low Precision COS Implementation
// Adapted from Source: http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
// Michael @ Polygonal.de - July 18th 2007
double cos(double value)
{
    if (value < -3.14159265)
	{
		value += 6.28318531;

	}
	else if (value > 3.14159265)
	{
		value -= 6.28318531;
	}
	
	value += 1.57079632;
	if (value > 3.14159265)
	{
		value -= 6.28318531;
	}

	if (value < 0)
	{
		return 1.27323954 * value + 0.405284735 * value * value;
	}
	return 1.27323954 * value - 0.405284735 * value * value;
}

// Low Precision SIN Implementation
// Adapted from Source: http://lab.polygonal.de/2007/07/18/fast-and-accurate-sinecosine-approximation/
// Michael @ Polygonal.de - July 18th 2007
double sin(double value)
{
    if (value < -3.14159265)
	{
		value += 6.28318531;

	}
	else if (value > 3.14159265)
	{
		value -= 6.28318531;
	}

	if (value < 0)
	{
		return 1.27323954 * value + .405284735 * value * value;
	}
	return 1.27323954 * value - 0.405284735 * value * value;
}

//double ceil(double value)
//{
//   return -floor(-value);
//}

//double floor(double value)
//{
//    double intValue = (int)value;
//    return value < intValue ? intValue - 1 : intValue;
//}