#include "Window.h"

using namespace winterchange;

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

Window::~Window()
{
	//This allows child Windows to be deleted by themselves - ensures parent won't be holding pointer to them
	if (parent != NULL)
	{
		parent->detachChild(this);
	}

	//Destroy this Window's children (which will in turn call their destructors to remove themselves from the children set)
	set<Window*> childrenToDelete(children); //so we aren't modifying the set we're iterating over
	for (std::set<Window*>::iterator it = childrenToDelete.begin(); it != childrenToDelete.end(); it++)
	{
		delete (*it);
	}
}

/* If the given Window is in this Window's set of children, it is removed.
* It is NOT deleted though.
*/
void Window::detachChild(Window* child)
{
	if (child == NULL)
	{
		return;
	}

	set<Window*>::iterator it = children.find(child);
	if (it == children.end())
	{
		return;
	}

	children.erase(it);
}

/* returns true if the given x, y values fall within ALL of the given min and
 * max values for their respective dimensions, false otherwise.
 */
bool winterchange::isWithinBounds(int x, int y, int minX, int minY, int maxX, int maxY)
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
void winterchange::getBoxSizeForText(const string& text, int& textW, int& textH, int hBorderSizeSum, int vBorderSizeSum)
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
int winterchange::constrainValue(int value, int min, int max)
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
bool winterchange::isWhitespaceChar(char c)
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
