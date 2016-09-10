#ifndef UI_CURSESWINDOW_H_
#define UI_CURSESWINDOW_H_

#include <panel.h>
#include "Window.h"

class CursesWindow : public Window
{
	protected:

		WINDOW* win;
		PANEL* panel;

		void initCursesWin(int w, int h, int row, int col);
		WINDOW* createWin(int w, int h, int row, int col);

	public:
		CursesWindow(int w, int h);
		CursesWindow(int w, int h, int x, int y);
		virtual ~CursesWindow() {}

		virtual int getWidth() const;
		virtual int getHeight() const;
		virtual void resize(int w, int h);
		virtual void resize(int w, int h, int x, int y);
};

#endif
