#include "Window.h"

using namespace winterchange;

Window::Window()
	: parent(NULL),
	  wordWrap(false)
{
	children = new set<Window*>();
}

Window::Window(Window* parent)
	: parent(parent),
	  wordWrap(false)
{
	children = new set<Window*>();
}

Window::~Window()
{
	//This allows child Windows to be deleted by themselves - ensures parent won't be holding pointer to them
	if (parent != NULL)
	{
		parent->detachChild(this);
	}

	//Destroy this Window's children (which will in turn call their destructors to remove themselves from the children set)
	set<Window*> childrenToDelete(*children); //so we aren't modifying the set we're iterating over
	for (std::set<Window*>::iterator it = childrenToDelete.begin(); it != childrenToDelete.end(); it++)
	{
		delete (*it);
	}

	delete children;
}

/* If the given Window is in this Window's set of children, it is removed.
* It is NOT deleted though.
*/
EXTERNAL_FUNC
void Window::detachChild(Window* child)
{
	if (child == NULL)
	{
		return;
	}

	set<Window*>::iterator it = children->find(child);
	if (it == children->end())
	{
		return;
	}

	children->erase(it);
}

/** Sets Top padding to be sized to be a given percentage of the Window content
 * area height (i.e. border size will be subtracted from height).
 *  -Truncates to nearest integer.
 *  -Accepts values between 0 and 1.0 (being 100%).
 *  -Padding will not be changed if passed values out of range.
 */
EXTERNAL_FUNC
void Window::setPaddingTopPercent(double percent)
{
	setPaddingTop(percent * (getHeight()-2));
}

/** Sets Bottom padding to be sized to be a given percentage of the Window content
 * area height (i.e. border size will be subtracted from height)..
 *  -Truncates to nearest integer.
 *  -Accepts values between 0 and 1.0 (being 100%).
 *  -Padding will not be changed if passed values out of range.
 */
EXTERNAL_FUNC
void Window::setPaddingBottomPercent(double percent)
{
	setPaddingBottom(percent * (getHeight()-2));
}

/** Sets Left padding to be sized to be a given percentage of the Window content
 * area width (i.e. border size will be subtracted from width).
 *  -Truncates to nearest integer.
 *  -Accepts values between 0 and 1.0 (being 100%).
 *  -Padding will not be changed if passed values out of range.
 */
EXTERNAL_FUNC
void Window::setPaddingLeftPercent(double percent)
{
	setPaddingLeft(percent * (getWidth()-2));
}

/** Sets Right padding to be sized to be a given percentage of the Window content
 * area width (i.e. border size will be subtracted from width)..
 *  -Truncates to nearest integer.
 *  -Accepts values between 0 and 1.0 (being 100%).
 *  -Padding will not be changed if passed values out of range.
 */
EXTERNAL_FUNC
void Window::setPaddingRightPercent(double percent)
{
	setPaddingRight(percent * (getWidth()-2));
}

/* Sets the padding on this window so that when the provided content is set,
 * it will be centered in the window.  Padding will be 0 in dimensions that
 * overflow the window.
 */
EXTERNAL_FUNC
void Window::setPaddingToCenterContent(const char* text)
{
	int winW = getWidth(), winH = getHeight();
	int boxW = 0, boxH = 0;
	int horizBorderTotal = 2, vertBorderTotal = 2; //TODO when borders are toggleable these will vary

	getBoxSizeForText(text, boxW, boxH, 0, 0);

	if(boxW < winW)
	{
		int padHoriz = (winW-horizBorderTotal - boxW);
		setPaddingLeft(padHoriz >> 1);
		setPaddingRight(padHoriz % 2 == 0 ? padHoriz >> 1 : (padHoriz >> 1)+1);
	}
	else
	{
		setPaddingLeft(0);
		setPaddingRight(0);
	}

	if(boxH < winH)
	{
		int padVert = (winH-vertBorderTotal - boxH);
		setPaddingTop(padVert >> 1);
		setPaddingBottom(padVert % 2 == 0 ? padVert >> 1 : (padVert >> 1)+1);
	}
	else
	{
		setPaddingTop(0);
		setPaddingBottom(0);
	}
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
void winterchange::getBoxSizeForText(const char* text, int& textW, int& textH, int hBorderSizeSum, int vBorderSizeSum)
{
	int currLineLen = 0;
	textW = 0;
	textH = 1;

	for(int i = 0; text[i] != 0; i++)
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
