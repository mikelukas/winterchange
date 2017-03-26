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

	return inStr;
}
