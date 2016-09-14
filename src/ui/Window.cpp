#include "Window.h"

/* returns true if the given x, y values fall within ALL of the given min and
 * max values for their respective dimensions, false otherwise.
 */
bool isWithinBounds(int x, int y, int minX, int minY, int maxX, int maxY)
{
	return x >= minX && y >= minY && x <= maxX && y <= maxY;
}

/* Ensures that value falls between the given min and max values.  If it doesn't
 * returns the nearest of min or max to the given value.
 */
int constrainValue(int value, int min, int max)
{
	if(value < min)
	{
		return min;
	}
	else if(value > max)
	{
		return max;
	}

	return value;
}