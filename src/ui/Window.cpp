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

/* Scans the entire given string for lines of text delimited by \n and fills the
 * given integers with the width and height (in # characters) of a box that
 * would fit around the text.
 * Height will always be at least 1, even if given an empty string.
 * hBorderSizeSum and vBorderSizeSum contain the total size of the left+right
 * and top+bottom borders, to be added into the final dimensions set into textW
 * and textH. E.g. for 1-char borders, pass in a sizeSum of 2 (1 for each side).
 */
void getBoxSizeForText(const string& text, int& textW, int& textH, int hBorderSizeSum, int vBorderSizeSum)
{
	int currLineLen = 0;
	textW = 0;
	textH = 1;

	for(int i = 0; i < text.length(); i++)
	{
		if(text[i] != '\n')  //not end of line, keep counting
		{
			currLineLen++;
		}
		else //End of line, check if our line is longest we've seen, reset line length counter
		{
			if(currLineLen > textW)
			{
				textW = currLineLen;
			}

			currLineLen = 0;
			textH++;
		}
	}

	textW += hBorderSizeSum;
	textH += vBorderSizeSum;
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
