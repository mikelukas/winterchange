#include "CursesWindow.h"

CursesWindow::CursesWindow(int w, int h)
	: win(NULL),
	  panel(NULL)
{
	initCursesWin(h, w, 0, 0);
}

CursesWindow::CursesWindow(int w, int h, int x, int y)
	: win(NULL),
	  panel(NULL)
{
	initCursesWin(h, w, y, x);
}

/* NOTE: rows = height, cols = width, reverse of constructor
 * Create a curses window and panel w/ the given dimensions and absolute coords.
 * Creates default border around the window.
 *
 * Does not draw the window, just updates the panel virtual screen.
 */
void CursesWindow::initCursesWin(int rows, int cols, int row, int col)
{
	win = newwin(rows, cols, row, col);
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

/* Resizes window and places top left corner at given (x, y) coords */
void CursesWindow::resize(int w, int h, int x, int y)
{
	wclrtobot(win);
	initCursesWin(h, w, y, x);
}

/* Get X position (column number) of top-left corner of window. */
int CursesWindow::getX() const
{
	return getbegx(win);
}

/* Get Y position (row number) of top-left corner of window. */
int CursesWindow::getY() const
{
	return getbegy(win);
}

/* Moves top-left corner of window to be at given (x,y) aka (col, row) coords.*/
void CursesWindow::move(int x, int y)
{
	move_panel(panel, y, x); //moving panel also moves its window
	update_panels();
}
