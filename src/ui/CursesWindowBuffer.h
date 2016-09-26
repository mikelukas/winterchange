#ifndef UI_CURSESWINDOWBUFFER_H_
#define UI_CURSESWINDOWBUFFER_H_

#include "ExpandableBuffer2D.h"
#include "../curseslib.h"

/* Holds a buffer of curses chtype, with the ability to flush to a window
 */
class CursesWindowBuffer: public ExpandableBuffer2D<chtype>
{
	public:
		CursesWindowBuffer(int rows, int cols);
		virtual ~CursesWindowBuffer() {}

		virtual void flushTo(WINDOW*, int winStartRow, int winStartCol, int bufStartRow, int bufStartCol, int w, int h);
};

#endif
