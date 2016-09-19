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

CursesWindow::CursesWindow(Window* parent, int w, int h, int x, int y)
	: Window(parent),
	  win(NULL),
	  panel(NULL),
	  title("")
{
	initCursesWin(h, w, y, x);
}

CursesWindow::~CursesWindow()
{
	//Destroy this Window's children (which will in turn call their destructors)
	for(int i = 0; i < children.size(); i++)
	{
		delete children[i];
	}
	children.clear();

	//Deallocate
	del_panel(panel);
	delwin(win);
}

/* NOTE: rows = height, cols = width, reverse of constructor
 * Create a curses window and panel w/ the given dimensions and absolute coords.
 * If the given coordinates are off-screen, they will each be adjusted to fall
 * on-screen.
 * Creates default border around the window.
 *
 * Does not update physical screen, just draws to the panel virtual screen.
 */
void CursesWindow::initCursesWin(int rows, int cols, int row, int col)
{
	//Ensure top-left corner is on-screen (newwin will fail otherwise).
	int adjustedRow = constrainValue(row, 0, LINES-1);
	int adjustedCol = constrainValue(col, 0, COLS-1);

	//Ensure window fits within screen
	int adjustedRows = constrainValue(rows, 0, LINES); //0 is allowed because curses interprets it as full-screen.
	int adjustedCols = constrainValue(cols, 0, COLS);
	if(adjustedRow + adjustedRows > LINES)
	{
		adjustedRows = LINES - adjustedRow;
	}

	if (adjustedCol + adjustedCols > COLS)
	{
		adjustedCols = COLS - adjustedCol;
	}

	win = newwin(adjustedRows, adjustedCols, adjustedRow, adjustedCol);
	box(win, 0, 0); //0,0 is default border characters for horizontal and vertical lines
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
	wclear(win);
	initCursesWin(h, w, y, x); //TODO: use wresize instead, still replace panel, and update panels. do move in a separate move_panel step after resize, or don't allow new x,y at all
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

/* Moves top-left corner of window to be at given (x,y) aka (col, row) coords.
 * If new coords are out of bounds, adjusts them to the nearest point in bounds.
 */
void CursesWindow::move(int x, int y)
{
	//Ensure window is moved to a point within bounds
	int adjustedX = constrainValue(x, 0, COLS-1);
	int adjustedY = constrainValue(y, 0, LINES-1);

	move_panel(panel, adjustedY, adjustedX); //moving panel also moves its window
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

/* Allocates a new CursesWindow with the given width and height, and its top
 * left corner in the top left corner of the parent, within its borders. If this
 * causes the child to appear off-screen, it will be moved to be on-screen.*/
Window* CursesWindow::makeChild(int w, int h)
{
	return makeChild(w, h, 0, 0); //0,0 relative to parent content area
}

/* Allocates a new CursesWindow with the given width and height, and its top
 * left corner set to center it relative to parent, within its borders.
 * If centering would move the child's top-left corner off-screen, it will then
 * be moved to be on screen.*/
Window* CursesWindow::makeChildCentered(int w, int h)
{
	//ensure requested child size fits within parent borders
	int parentW = getWidth()-2, parentH = getHeight()-2;

	//Get centered child X,Y relative to parent content area
	int childX = (parentW-w) / 2;
	int childY = (parentH-h) / 2;

	return makeChild(w, h, childX, childY);
}

/* Allocates a new CursesWindow with the given width and height, and its top
 * left corner at position (x,y) relative to parent content area (i.e. within
 * its borders).  If absolute (x,y) of child ends up off-screen, child is moved
 * to appear on-screen.*/
Window* CursesWindow::makeChild(int w, int h, int x, int y)
{
	Window* child = new CursesWindow(parent, w, h, getX()+1 + x, getY()+1 + y);   //+1 so child position is relative to parent content area (area within borders)
	children.push_back(child);

	return child;
}

/* Fills the window's content area, + the given margins, with the given text,
 * wrapping text at words if it is too large for the content area.
 *
 * If the given margins are invalid (negative, or too large such that there is
 * no area for text), returns without doing anything.
 */
void CursesWindow::fillWithText(const string& text, int marginT, int marginB, int marginL, int marginR)
{
	if(marginT < 0 || marginB < 0 || marginL < 0 || marginR < 0)
	{
		return;
	}
	if(marginT + marginB > getHeight() || marginL + marginR > getWidth())
	{
		return;
	}

	int adjustedW = getWidth()-2 - marginL - marginR;
	int adjustedH = getHeight()-2 - marginT - marginB;
	int startRow = 1 + marginT;
	int startCol = 1 + marginL;
	int maxRow = startRow + adjustedH;
	int maxCol = startCol + adjustedW;

	int wordStart = 0;

	wmove(win, startRow, startCol); //+1 to start w/i border
	for(int i = 0; i < text.length(); i++)
	{
		if(!isWhitespaceChar(text[i]) && i+1 != text.length()) { //latter test will cause the last word to be flushed if we don't hit another whitespace char as the last char in the text
 			continue; //Only take action when we hit white space
		}

		int curRow, curCol;
		getyx(win, curRow, curCol);

		//Write-out word
		int wordSize = i-wordStart;
		if(curCol + wordSize > maxCol) {
			if(wordSize < adjustedW) {
				//Advance cursor to next line before continuing to draw the word
				curRow++;
				curCol = startCol;
				if(curRow >= maxRow) {
					return; //abort if we're at the bottom of content area
				}
				wmove(win, curRow, curCol);
			}

			int remainingLineW;
			while(wordSize > adjustedW) {
				remainingLineW = maxCol - curCol;
				for(int j = wordStart; j < (wordStart + remainingLineW); j++) {
					waddch(win, text[j]);
				}

				//Recalculate word start, size since we've now written part of it
				wordStart += remainingLineW;
				wordSize = i-wordStart;

				//Advance cursor to next line before continuing to draw the word
				curRow++;
				curCol = startCol;
				if(curRow >= maxRow) {
					return; //abort if we're at the bottom of content area
				}

				wmove(win, curRow, curCol);
			}

			//Write remaining
			for(int j = wordStart; j < i; j++) {
				waddch(win, text[j]);
			}
		} else {
			for(int j = wordStart; j < i; j++) {
				waddch(win, text[j]);
			}
		}

		//reset word start position
		wordStart = i + 1;

		getyx(win, curRow, curCol);

		//Check if we need to move to a new line instead of drawing whitespace
		if(curCol >= maxCol-1 //-1 since a space at the end of the line also should advance to the next one
		|| text[i] == '\n' //don't want to draw \n because it will clear border
		|| (text[i] == '\t' && maxCol - curCol < TABSIZE)) //don't want to draw tab that is big enough to overwrite border
		{
			curRow +=1;
			curCol = startCol;
			wmove(win, curRow, curCol);
		}
		else
		{
			//draw whitespace char if it doesn't put us out of bounds
			waddch(win, text[i]);
		}
		if(curRow >= maxRow) {
			return;
		}
	}

	update_panels();
}
