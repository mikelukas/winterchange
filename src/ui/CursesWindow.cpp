#include "CursesWindow.h"

CursesWindow::CursesWindow(int w, int h)
{
	init("", w, h, 0, 0);
}

CursesWindow::CursesWindow(const string& content)
{
	int w, h;
	getBoxSizeForText(content, w, h, 2, 2); //2 for borders on each side
	init(content, w, h, 0, 0);
}

CursesWindow::CursesWindow(int w, int h, int x, int y)
{
	init("", w, h, x, y);
}

CursesWindow::CursesWindow(const string& content, int x, int y)
{
	int w,h;
	getBoxSizeForText(content, w, h, 2, 2); //2 for borders on each side
	init(content, w, h, x, y);
}

CursesWindow::CursesWindow(Window* parent, int w, int h, int x, int y)
	: Window(parent)
{
	init("", w, h, x, y);
}

CursesWindow::CursesWindow(Window* parent, const string& content, int x, int y)
	: Window(parent)
{
	int w, h;
	getBoxSizeForText(content, w, h, 2, 2);  //2 for borders on each side
	init(content, w, h, x, y);
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
	delete buffer;
	del_panel(panel);
	delwin(win);

	//Update panel system so that it knows the panel has been deleted
	update_panels();
}

void CursesWindow::init(const string& content, int w, int h, int x, int y)
{
	win = NULL;
	panel = NULL;
	paddingT = 0;
	paddingB = 0;
	paddingL = 0;
	paddingR = 0;
	nextWriteRow = 0;
	nextWriteCol = 0;
	contentMaxRow = 0;
	contentMaxCol = 0;
	scrollRowOffset = 0;
	scrollColOffset = 0;
	title = "";
	this->content = "";
	buffer = NULL;

	buildWindow(h, w, y, x);

	if(content.length() > 0) {
		replaceText(content);
	}
}

/* NOTE: rows = height, cols = width, reverse of constructor
 * Create a curses window and panel w/ the given dimensions and absolute coords.
 * If the given coordinates are off-screen, they will each be adjusted to fall
 * on-screen.
 * Creates default border around the window.
 *
 * Does not update physical screen, just draws to the panel virtual screen.
 */
void CursesWindow::buildWindow(int rows, int cols, int row, int col)
{
	if(buffer != NULL)
	{
		delete buffer;
	}
	buffer = new CursesWindowBuffer(rows, cols);

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
	buildWindow(h, w, y, x); //TODO: use wresize instead, still replace panel, and update panels. do move in a separate move_panel step after resize, or don't allow new x,y at all
	replaceText(content);
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

/** Allocates a new CursesWindow child window (see non-withContent() methods),
 * sized to fit the given text, with top-left corner in the top-left corner
 * inside this Window's borders.
 */
Window* CursesWindow::makeChildWithContent(const string& content)
{
	return makeChildWithContent(content, 0,0);
}

/** Allocates a new CursesWindow child window (see non-withContent() methods),
 * sized to fit the given text, centered within this window.  Window dimensions
 * will be shrunken to fit on screen if they spill out.
 */
Window* CursesWindow::makeChildWithContentCentered(const string& content)
{
	int w,h;
	getBoxSizeForText(content, w, h, 2, 2); //+2 for borders

	Window* child = makeChildCentered(w,h);
	child->replaceText(content);

	return child;
}

/** Allocates a new CursesWindow child window (see non-withContent() methods),
 * sized to fit the given text, with top-left corner positioned at the given
 * x,y coordinates relative to the area within the window's borders.
 */
Window* CursesWindow::makeChildWithContent(const string& content, int x, int y)
{
	Window* child = new CursesWindow(parent, content, getX()+1 + x, getY()+1 + y);   //+1 so child position is relative to parent content area (area within borders)
	children.push_back(child);

	return child;
}

/* Set all padding for conteint, in rows/cols. Padding values given must be >= 0
 * otherwise they will be ignored. refreshContent() must be called afterward
 * unless resizing window or replacing current text afterward.
 */
void CursesWindow::setPadding(int top, int bottom, int left, int right)
{
	setPaddingTop(top);
	setPaddingBottom(bottom);
	setPaddingLeft(left);
	setPaddingRight(right);
}

/* Set top padding for content, in rows. Padding value given must be >= 0,
 * otherwise it will be ignored. refreshContent() must be called afterward
 * unless resizing window or replacing current text afterward.
 */
void CursesWindow::setPaddingTop(int paddingT)
{
	if(paddingT < 0) {
		return;
	}

	this->paddingT = paddingT;
}

/* Set bottom padding for content, in rows. Padding value given must be >= 0,
 * otherwise it will be ignored. refreshContent() must be called afterward
 * unless resizing window or replacing current text afterward.
 */
void CursesWindow::setPaddingBottom(int paddingB)
{
	if(paddingB < 0) {
		return;
	}

	this->paddingB = paddingB;
}

/* Set left padding for content, in rows. Padding value given must be >= 0,
 * otherwise it will be ignored. refreshContent() must be called afterward
 * unless resizing window or replacing current text afterward.
 */
void CursesWindow::setPaddingLeft(int paddingL)
{
	if(paddingL < 0) {
		return;
	}

	this->paddingL = paddingL;
}

/* Set right padding for content, in rows. Padding value given must be >= 0,
 * otherwise it will be ignored. refreshContent() must be called afterward
 * unless resizing window or replacing current text afterward.
 */
void CursesWindow::setPaddingRight(int paddingR)
{
	if(paddingR < 0) {
		return;
	}

	this->paddingR = paddingR;
}

/** Overwrites entire content area of window (area within borders) with spaces.
 *  updates panels.
 * */
void CursesWindow::clearContent()
{
	for(int row = 1; row < getHeight()-1; row++)
	{
		mvwhline(win, row, 1, ' ', getWidth()-2); //line width is win width-2 b/c of 1 col for left and right borders
	}

	update_panels();
}

/* Re-writes text content into window. Unless replacing text, use after changing
 * padding to make content lay itself out again within window observing new
 * padding sizes.
 */
void CursesWindow::refreshContent()
{
	buffer->clear();
	clearContent();
	replaceText(content);
}

/* Clears content drawn to the window, then flushes the buffer to it again,
 * then calls doupdate() on ncurses.
 */
void CursesWindow::redraw()
{
	clearContent();
	flushBuffer();
	doupdate();
}

/* Decrements scroll row offset, so that when content is flushed to window from
 * buffer, flushing starts at a row position 1 less row from previous pos.
 * Cannot decrement beyond 0.
 */
void CursesWindow::scrollUp()
{
	if(scrollRowOffset > 0) //this implicitly considers if content is shorter than content area, since offset would be 0 if it is shorter, and thus no scrolling would happen
	{
		scrollRowOffset--;
	}
}

/* Increments scroll row offset, so that when content is flushed to window from
 * buffer, flushing starts at a row position 1 more row ahead from previously.
 * Won't scroll past last row in buffer.
 */
void CursesWindow::scrollDown()
{
	if(contentMaxRow >= getHeight()-2 - paddingT - paddingB //can only scroll down if content is taller than content area
	&& scrollRowOffset < buffer->getHeight())
	{
		scrollRowOffset++;
	}
}

/* Sets scroll row offset to the given row, so that when content is flushed to
 * the window from the buffer, flushing starts at that row. If row is greater
 * than the last row of the buffer, it will be set to the last row, and if it is
 * less than 0, it will be set to 0.
 */
void CursesWindow::vScrollTo(int row)
{
	if(row >= buffer->getHeight())
	{
		scrollRowOffset = buffer->getHeight();
	}
	else if(row < 0)
	{
		scrollRowOffset = 0;
	}
	else
	{
		scrollRowOffset = row;
	}
}

/** Sets scroll row offset so that when content is flushed to the window from
 * the buffer, flushing starts at a row that will result in top row of the
 * content being the first row of the buffer.
 */
void CursesWindow::scrollToTop()
{
	scrollRowOffset = 0;
}

/* Sets scroll row offset so that when content is flushed to the window from
 * the buffer, flushing starts from a row that will result in the bottom row
 * of the buffer being visible on-screen; if buffered content is bigger than
 * screen, then bottom row of buffer will be bottom row of content area, and top
 * row of buffer will be the bottom row-window height.  If content is smaller
 * than the content area, it will all be displayed, with the top row of the buffer
 */
void CursesWindow::scrollToBottom()
{
	int contentAreaHeight = getHeight()-2 - paddingT - paddingB;
	if(contentMaxRow < contentAreaHeight)
	{
		scrollRowOffset = 0;
	}
	else
	{
		scrollRowOffset = contentMaxRow+1 - contentAreaHeight;
	}
}

/* Decrements scroll col offset, so that when content is flushed to window from
 * buffer, flushing starts at a col position 1 less col from previous pos.
 * Cannot decrement beyond 0.
 */
void CursesWindow::scrollLeft()
{
	if(scrollColOffset > 0) //this implicitly considers if content is shorter than content area, since offset would be 0 if it is shorter, and thus no scrolling would happen
	{
		scrollColOffset--;
	}
}

/* Increments scroll col offset, so that when content is flushed to window from
 * buffer, flushing starts at a col position 1 more col ahead from previously.
 * Won't scroll past last col in buffer.
 * Cannot scroll right if wordWrap is enabled, b/c content is purposely reshaped
 * to fit the width of the content area in this case and can't be wider.
 */
void CursesWindow::scrollRight()
{
	if(!wordWrap
	&& contentMaxCol >= getWidth()-2 - paddingL - paddingR //can only scroll right if content is wider than content area
	&& scrollColOffset < buffer->getWidth())
	{
		scrollColOffset++;
	}
}

/** Sets scroll col offset so that when content is flushed to the window from
 * the buffer, flushing starts at a col that will result in leftmost col of the
 * content being the first col of the buffer.
 */
void CursesWindow::scrollToLeftEnd()
{
	scrollColOffset = 0;
}

/* Sets scroll col offset so that when content is flushed to the window from
 * the buffer, flushing starts from a col that will result in the rightmost col
 * of the buffer being visible on-screen; if buffered content is bigger than
 * screen, then rightmost col of buffer will be rightmost col of content area, and
 * leftmost col of buffer will be the rightmost col minus window width.  If
 * content is smaller than the content area, it will all be displayed, with the
 * leftmost col of the buffer.
 * Cannot scroll to the right end if word-wrap is enabled.
 */
void CursesWindow::scrollToRightEnd()
{
	if(wordWrap)
	{
		return;
	}

	int contentAreaWidth = getWidth()-2 - paddingL - paddingR;
	if(contentMaxCol < contentAreaWidth)
	{
		scrollColOffset = 0;
	}
	else
	{
		scrollColOffset = contentMaxCol+1 - contentAreaWidth;
	}
}

/* Saves the given coordinates as the next buffer position at which fillWithText
 * will write when appending text.*/
void CursesWindow::saveNextWriteCoords(int row, int col)
{
	nextWriteRow = row;
	nextWriteCol = col;
}

/* Fills the window's buffer with the given text accounting for padding,
 * wrapping text at words if it is too large for the content area.
 *
 * If the padding is invalid (negative, or too large such that there is no area
 * for text), clears content area and returns, and still saves given text.
 */
void CursesWindow::fillWithText(const string& text, int offsetRow, int offsetCol)
{
	if(paddingT + paddingB > getHeight()-2 || paddingL + paddingR > getWidth()-2)
	{
		saveNextWriteCoords(0,0);
		buffer->clear();
		clearContent();
		return;
	}

	int adjustedW = getWidth()-2 - paddingL - paddingR;

	//Extents of content area after applying padding.
	int firstCol = 0;

	int maxCol = firstCol + adjustedW;
	if(wordWrap)
	{
		contentMaxCol = maxCol;
	}

	//Position to actually start writing at
	int startRow = offsetRow;
	int startCol = offsetCol;

	int wordStart = 0;
	int curRow = startRow, curCol = startCol;
	for(int i = 0; i < text.length(); i++)
	{
		if(!isWhitespaceChar(text[i]) && i+1 != text.length()) { //latter test will cause the last word to be flushed if we don't hit another whitespace char as the last char in the text
 			continue; //Only take action when we hit white space
		}

		//Write-out word
		int wordSize = i-wordStart;
		if(wordWrap && curCol + wordSize > maxCol) { //Word won't fit on line
			if(wordSize < adjustedW) {
				//Advance cursor to next line before continuing to draw the word
				curRow++;
				curCol = firstCol;
			}

			int remainingLineW;
			while(wordSize > adjustedW) { //Word is larger than the full line width; break it up
				remainingLineW = maxCol - curCol;
				for(int j = wordStart; j < (wordStart + remainingLineW); j++) {
					buffer->writeAt(text[j], curRow, curCol++);
				}

				//Recalculate word start, size since we've now written part of it
				wordStart += remainingLineW;
				wordSize = i-wordStart;

				//Advance cursor to next line before continuing to draw the word
				curRow++;
				curCol = firstCol;
			}

			//Write remaining
			for(int j = wordStart; j < i; j++) {
				buffer->writeAt(text[j], curRow, curCol++);
			}
		} else { //Word fits on line
			for(int j = wordStart; j < i; j++) {
				buffer->writeAt(text[j], curRow, curCol++);
			}
		}

		//reset word start position
		wordStart = i + 1;

		//Check if we need to move to a new line instead of drawing whitespace
		if(text[i] == '\n' //don't want to draw \n because it will clear border
		||(wordWrap
		   && (curCol >= maxCol-1  //-1 since a space at the end of the line also should advance to the next one
			  || (text[i] == '\t' && maxCol - curCol < TABSIZE)))) //don't want to draw tab that is big enough to overwrite border
		{
			if(curCol > contentMaxCol) //Only will matter when not word wrapping; in that case, location of a newline could be beyond content area max column
			{
				contentMaxCol = curCol;
			}

			curRow++;
			curCol = firstCol;
		}
		else if(text[i] == '\t') //Expand tabs that don't move to new lines, since the buffer needs to have the literal characters that will be flushed to the screen
		{
			int numSpaces = TABSIZE - (curCol % TABSIZE);
			for(int j = 0; j < numSpaces; j++)
			{
				buffer->writeAt(' ', curRow, curCol++);
			}
		}
		else
		{
			//draw whitespace char if it doesn't put us out of bounds
			buffer->writeAt(text[i], curRow, curCol++);
		}
	}

	//Before clearing any remaining existing content, save current coords, since clearing will move cursor position
	saveNextWriteCoords(curRow, curCol);
	contentMaxRow = curRow;

	//Fill rest of buffer with spaces to clear out any previous content
	buffer->clearFrom(curRow, curCol);

	flushBuffer();
}

/* Writes character directly into the window, relative to the top-left corner of
 * the content area (i.e. offset by borders + top-left padding).
 * If coordinates are negative, nothing will be written.  If coordinates are out
 * of bounds to the right or bottom, characters will be written into the buffer
 * but not displayed on screen.
 */
void CursesWindow::writeCharAt(char c, int x, int y)
{
	if(x < 0 || y < 0)
	{
		return;
	}

	buffer->writeAt((chtype) c, y + scrollRowOffset, x + scrollColOffset);
	if(x + scrollColOffset > contentMaxCol)
	{
		contentMaxCol = x + scrollColOffset;
	}

	if(y + scrollRowOffset > contentMaxRow)
	{
		contentMaxRow = y + scrollRowOffset;
	}
}

/* Writes string directly into the window, relative to the top-left corner of
 * the content area (i.e. offset by borders + top-left padding).
 * If coordinates are negative, nothing will be written.  If coordinates are out
 * of bounds to the right or bottom, characters will be written into the buffer
 * but not displayed on screen.
 */
void CursesWindow::writeStrAt(const string& str, int x, int y)
{
	if(x < 0 || y < 0)
	{
		return;
	}

	for(int i = 0; i < str.length(); i++)
	{
		buffer->writeAt((chtype) (str[i]), y + scrollRowOffset, x + scrollColOffset + i);
	}

	int maxX = x + scrollColOffset + str.length() - 1;
	int maxY = y + scrollRowOffset;
	if(maxX > contentMaxCol)
	{
		contentMaxCol = maxX;
	}

	if(maxY > contentMaxRow)
	{
		contentMaxRow = maxY;
	}
}

/* Sets word wrap on text added to this window so that it will go to the next
 * line when it reaches the right edge of the content area.
 * If word wrap is being enabled, the scroll column offset will be reset to the
 * left edge, b/c horizontal scroll cannot happen when words wrap in this way.
 */
void CursesWindow::setWordWrap(bool wordWrap)
{
	Window::setWordWrap(wordWrap);
	scrollColOffset = 0;
}

/* Overwrites all text in the content area with the given text.*/
void CursesWindow::replaceText(const string& text)
{
	content = text;
	fillWithText(text, 0, 0);
}

/* Adds text to the content area at the next available position for writing
 * after the existing content.*/
void CursesWindow::appendText(const string& text)
{
	appendText(text, false);
}

/* Adds text to the content area at the next available position for writing
 * after the existing content, optionally starting on a new line after that
 * content if newline is true.*/
void CursesWindow::appendText(const string& text, bool newline)
{
	string textToWrite = (newline ? "\n" + text : text); //need to make a new string since text is a const
	content += textToWrite;
	fillWithText(textToWrite, nextWriteRow, nextWriteCol);
}

/* Tells the buffer to write its entire content to the content area of the
 * window.
 * Tells panels to update.
 */
void CursesWindow::flushBuffer()
{
	int adjustedW = getWidth()-2 - paddingL - paddingR;
	int adjustedH = getHeight()-2 - paddingT - paddingB;
	buffer->flushTo(win, 1 + paddingT, 1 + paddingL, scrollRowOffset,scrollColOffset, adjustedW, adjustedH);

	update_panels();
}
