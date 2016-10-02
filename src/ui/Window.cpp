#include "Window.h"

Window::Window()
	: parent(NULL),
	  wordWrap(false)
{

}

Window::Window(Window* parent)
	: parent(parent),
	  wordWrap(false)
{

}

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

/* returns true if c is: ' ', \t, \n, \f, \r, and false otherwise */
bool isWhitespaceChar(char c)
{
	switch(c)
	{
	case ' ':
	case '\t':
	case '\n':
	case '\r':
	case '\f':
		return true;
	default:
		return false;
	}
}
