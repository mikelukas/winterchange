#ifndef IMPL_CURSES_CURSESINPUTDELEGATE_H_
#define IMPL_CURSES_CURSESINPUTDELEGATE_H_

#include "curseslib.h"
#include "../../InputDelegate.h"

namespace winterchange
{
	/** Implementation of InputDelegate using the curses library, for use with
	 * curses WINDOW* objects so that input can be retrieved from them.
	 */
	class CursesInputDelegate: public InputDelegate
	{
		protected:
			WINDOW* win;

		public:
			CursesInputDelegate(WINDOW*);
			virtual ~CursesInputDelegate() {}

			virtual int awaitKey();
			virtual char* awaitCString(const int maxLen);
	};

}

#endif
