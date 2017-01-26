#ifndef IMPL_CURSES_CURSESWINDOW_H_
#define IMPL_CURSES_CURSESWINDOW_H_

#include <panel.h>
#include "../../Window.h"
#include "CursesWindowBuffer.h"

namespace winterchange
{
	/* Implementation of Window interface using ncurses/PDcurses to draw the window
	 * and manage its panel
	 */
	class EXTERNAL_CLASS CursesWindow : public Window
	{
		protected:

			WINDOW* win;
			PANEL* panel;

			int paddingT;
			int paddingB;
			int paddingL;
			int paddingR;

			int nextWriteRow;
			int nextWriteCol;

			int contentMaxRow;
			int contentMaxCol;

			int scrollRowOffset;
			int scrollColOffset;

			string title;
			string content;

			CursesWindowBuffer* buffer;

			virtual void init(const string& content, int w, int h, int x, int y);
			virtual void buildWindow(int rows, int cols, int row, int col); //note rows = height, cols = width, reverse of constructor

			virtual void vScrollTo(int row);

			virtual void fillWithText(const string&, int, int);
			virtual void flushBuffer();

			virtual void saveNextWriteCoords(int row, int col);

		public:
			CursesWindow(int w, int h);
			CursesWindow(const string&);
			CursesWindow(int w, int h, int x, int y);
			CursesWindow(const string&, int x, int y);
			CursesWindow(Window* parent, int w, int h, int x, int y);
			CursesWindow(Window* parent, const string&, int x, int y);
			virtual ~CursesWindow();

			virtual int getWidth() const;
			virtual int getHeight() const;

			virtual void resize(int w, int h);
			virtual void resize(int w, int h, int x, int y);

			virtual int getX() const;
			virtual int getY() const;
			virtual void move(int x, int y);

			virtual const string& getTitle() const;
			virtual void setTitle(const string&);

			virtual void setPadding(int top, int bottom, int left, int right);
			virtual void setPaddingTop(int);
			virtual void setPaddingBottom(int);
			virtual void setPaddingLeft(int);
			virtual void setPaddingRight(int);

			virtual void clearContent();
			virtual void refreshContent();
			virtual void redraw();

			virtual void scrollUp();
			virtual void scrollDown();
			virtual void scrollToTop();
			virtual void scrollToBottom();

			virtual void scrollLeft();
			virtual void scrollRight();
			virtual void scrollToLeftEnd();
			virtual void scrollToRightEnd();

			virtual void writeCharAt(char, int x, int y);
			virtual void writeStrAt(const string&, int x, int y);

			virtual void setWordWrap(bool);
			virtual void replaceText(const string&);
			virtual void appendText(const string&);
			virtual void appendText(const string&, bool newLine);

			virtual Window* makeChild(int w, int h); //adds child at 0,0 relative to this window's top-left corner
			virtual Window* makeChildCentered(int w, int h);
			virtual Window* makeChild(int w, int h, int x, int y);

			virtual Window* makeChildWithContent(const string&);
			virtual Window* makeChildWithContentCentered(const string&);
			virtual Window* makeChildWithContent(const string&, int x, int y);
	};
}

#endif
