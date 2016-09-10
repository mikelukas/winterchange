#ifndef UI_CURSESWINDOW_H_
#define UI_CURSESWINDOW_H_

#include <panel.h>
#include "Window.h"

/* Implementation of Window interface using ncurses/PDcurses to draw the window
 * and manage its panel
 */
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

		virtual int getX() const;
		virtual int getY() const;
		virtual void move(int x, int y);
};

#endif
