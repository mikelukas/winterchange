#include "CursesWindow.h"

CursesWindow::CursesWindow(int w, int h)
	: win(NULL),
	  panel(NULL),
	  title("")
{
	initCursesWin(h, w, 0, 0);
}

CursesWindow::CursesWindow(int w, int h, int x, int y)
	: win(NULL),
	  panel(NULL),
	  title("")
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
	setTitle(title);

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
	return getmaxx(win);
}

/* Calculates the height of the window by getting the max and beginning x, and
 * returning the difference.
 */
int CursesWindow::getHeight() const
{
	return getmaxy(win);
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

/* Gets the current title set in the top of the window */
const string& CursesWindow::getTitle() const
{
	return title;
}

/* Prints the given title string centered in the top border of the window, and
 * updates title field tracked by this instance.
 *
 * Title is truncated if it is longer than the window width.
 * */
void CursesWindow::setTitle(const string& newTitle)
{
	int widthNoCorners = getWidth()-2; //-2 to avoid writing on corners

	//clear old title if there was one
	if(!title.empty())
	{
		mvwhline(win, 0, 1, 0, widthNoCorners); //0 for border char uses default char, x=1 to start after corner char
	}

	title = newTitle;

	//Done drawing if new title is empty string;
	if(newTitle.length() == 0)
	{
		update_panels(); //so that if we're clearing the title, the border gets updated
		return;
	}

	//Truncate title if it is larger than window width
	const string& displayedTitle = (newTitle.length() <= widthNoCorners) ? newTitle : newTitle.substr(0, widthNoCorners);

	//overwrite border w/ new title
	int textStartCol = (widthNoCorners - displayedTitle.length()) / 2; //truncate is OK; breaks ties by preferring the left-alignment
	mvwprintw(win, 0, textStartCol+1, displayedTitle.c_str());

	//Add horizontal lines one each side of title if there's room
	if(displayedTitle.length() <= widthNoCorners-2)  //need at least 2 extra chars between corners, so widthNoCorners -2
	{
		mvwaddch(win, 0, textStartCol, ACS_RTEE);
		mvwaddch(win, 0, textStartCol + displayedTitle.length() + 1, ACS_LTEE);
	}

	update_panels();
}
