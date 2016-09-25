#include "CursesWindowBuffer.h"

/* Creates a buffer of size rows x cols, defaulted with spaces */
CursesWindowBuffer::CursesWindowBuffer(int rows, int cols)
	: ExpandableBuffer2D(rows, cols, ' ')
{

}

/* Copies the entire contents of the buffer to the given window, starting at the
 * given offset within the window.
 * Assumes window is large enough to accommodate.
 */
void CursesWindowBuffer::flushTo(WINDOW* win, int offsetT, int offsetL)
{
	int rowOffset = 0;
	for(int row = 0; row < rows; row++)
	{
		wmove(win, offsetT+row, offsetL); //move cursor to start of row to be written to

		rowOffset = row*cols;
		for(int col = 0; col < cols; col++)
		{
			waddch(win, buffer[rowOffset + col]);
		}
	}
}
