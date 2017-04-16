#include "CursesInputDelegate.h"

using namespace winterchange;

CursesInputDelegate::CursesInputDelegate(WINDOW* win)
	: win(win)
{

}
/** Returns int keycode for pressed key, using curses wgetch() w/ this window's
 * context.
 */
int CursesInputDelegate::awaitKey()
{
	return wgetch(win);
}

/* Allocates a string of size maxLen+1 (+1 to hold null terminator), waits for
 * wgetnstr() to collect that many characters or a newline/carriage return, and
 * returns the string.
 *
 * Caller must deallocate returned string.
 */
char* CursesInputDelegate::awaitCString(const int maxLen)
{
	char* inStr = new char[maxLen+1]; //+1 for null terminator
	echo();
	wgetnstr(win, inStr, maxLen);
	noecho();

#ifdef WIN32
	//PDCurses will always echo the newline character typed after entering input, erasing the right border character on the row input was entered, so need to go back and write over the blank
	int curRow, curCol;
	getyx(win, curRow, curCol);
	mvwaddch(win, curRow - 1, win->_maxx-1, ACS_VLINE);
	wmove(win, curRow, curCol);
#endif

	return inStr;
}
