#include <algorithm>
#include "CursesWindowBuffer.h"

using std::min;
using namespace winterchange;

/* Creates a buffer of size rows x cols, defaulted with spaces */
CursesWindowBuffer::CursesWindowBuffer(int rows, int cols)
	: ExpandableBuffer2D(rows, cols, ' ')
{

}

/* Copies the entire contents of the buffer to the given window, starting at the
 * given offset within the window, copying from the buffer at given offset.
 * Amount of rows and cols copied is equal to h and w respectively.
 *
 * Assumes given window offset and h and w fit within the window's content area.
 */
void CursesWindowBuffer::flushTo(WINDOW* win, int winStartRow, int winStartCol, int bufStartRow, int bufStartCol, int w, int h)
{
	int row = 0, col = 0, rowOffset = 0;
	int numRows = min(h, rows-bufStartRow), numCols = min(w, cols-bufStartCol);

	for(row = bufStartRow; row < bufStartRow+numRows; row++)
	{
		wmove(win, winStartRow++, winStartCol); //move cursor to start of row to be written to

		rowOffset = row*cols;
		for(col = bufStartCol; col < bufStartCol+numCols; col++)
		{
			waddch(win, buffer[rowOffset + col]);
		}

		//If desired write w is more columns than we have left in the row, write default chars since we can't look up characters in the buffer anymore
		for(; col < bufStartCol+w; col++)
		{
			waddch(win, defaultVal);
		}
	}

	//If desired write h is more rows than we have left in the column, write default chars since we can't look up characters in the buffer anymore
	for(; row < bufStartRow+h; row++)
	{
		wmove(win, winStartRow++, winStartCol);
		whline(win, defaultVal, w);
	}
}
