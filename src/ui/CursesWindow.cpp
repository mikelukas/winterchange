#include "CursesWindow.h"

CursesWindow::CursesWindow(int w, int h)
	: win(NULL),
	  panel(NULL)
{
	initCursesWin(w, h, 0, 0);
}

CursesWindow::CursesWindow(int w, int h, int x, int y)
	: win(NULL),
	  panel(NULL)
{
	initCursesWin(w, h, y, x); //curses likes y coord first
}

/* Create a curses window and panel w/ the given dimensions and absolute coords.
 * Creates default border around the window.
 *
 * Does not draw the window, just updates the panel virtual screen.*/
void CursesWindow::initCursesWin(int w, int h, int row, int col)
{
	win = newwin(w, h, row, col);
	box(win, 0, 0); //0,0 is default board characters for horizontal and vertial lines

	if(panel == NULL) {
		panel = new_panel(win);
	} else {
		WINDOW* oldwin = panel_window(panel);
		replace_panel(panel, win);
		delwin(oldwin);
	}

	update_panels(); //write new panel to virtual screen
}

/* Calculates the width of the window by getting the max and beginning y, and
 * returning the difference.
 */
int CursesWindow::getWidth() const
{
	return getmaxy(win) - getbegy(win);
}

/* Calculates the height of the window by getting the max and beginning x, and
 * returning the difference.
 */
int CursesWindow::getHeight() const
{
	return getmaxx(win) - getbegx(win);
}

/* Resizes relative to the current top left corner of window */
void CursesWindow::resize(int w, int h)
{
	int currX, currY;
	getbegyx(win, currY, currX);

	resize(w, h, currX, currY);
}

/** Resizes window and places top left corner at given (x, y) coords */
void CursesWindow::resize(int w, int h, int x, int y)
{
	wclrtobot(win);
	initCursesWin(w, h, y, x);
}

