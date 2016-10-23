#ifndef IMPL_CURSES_CURSESWINDOWBUFFER_H_
#define IMPL_CURSES_CURSESWINDOWBUFFER_H_

#include "../../util/ExpandableBuffer2D.h"
#include "curseslib.h"

namespace winterchange
{
	/* Holds a buffer of curses chtype, with the ability to flush to a window
	 */
	class CursesWindowBuffer: public ExpandableBuffer2D<chtype>
	{
		public:
			CursesWindowBuffer(int rows, int cols);
			virtual ~CursesWindowBuffer() {}

			virtual void flushTo(WINDOW*, int winStartRow, int winStartCol, int bufStartRow, int bufStartCol, int w, int h);
	};
}

#endif
