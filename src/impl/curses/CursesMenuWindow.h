#ifndef IMPL_CURSES_CURSESMENUWINDOW_H_
#define IMPL_CURSES_CURSESMENUWINDOW_H_

#include "../../Menu.h"
#include "CursesWindow.h"

namespace winterchange
{
	/* CursesWindow that can display the given data as a menu. */
	template <typename T>
	class EXTERNAL_CLASS CursesMenuWindow: public CursesWindow, public MenuDataView<T>
	{
		protected:
			Menu<T> menu;
			char* (*dataToString)(T); //data needs to be converted to a string first before it can written into the window - the instantiator of this menu controls how that string is built via what is passed for this function. Returned string should be able to be de-allocated.

		public:
			CursesMenuWindow(int w, int h, MenuDataModel<T>* data, char* (*dataToString)(T));
			virtual ~CursesMenuWindow() {}

			virtual void redraw();
			virtual void drawData(T data, int position, bool selected);
	};
}

#include "CursesMenuWindow.cpp"

#endif
