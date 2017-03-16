#ifndef WINDOW_H_
#define WINDOW_H_

#include <set>
#include "LibMacros.h"

using std::set;

namespace winterchange
{
	/* Abstract base class for manipulating windows; intended to keep details of the
	 * underlying low-level window implementation abstract */
	class EXTERNAL_CLASS Window
	{
		protected:

			Window* parent;
			set<Window*>* children;

			bool wordWrap;

		public:
			Window();
			Window(Window* parent);
			virtual ~Window();

			virtual int getWidth() const = 0;
			virtual int getHeight() const = 0;

			virtual void resize(int w, int h) = 0;
			virtual void resize(int w, int h, int x, int y) = 0;

			virtual void maximize() = 0;
			virtual void unmaximize() = 0;

			virtual int getX() const = 0;
			virtual int getY() const = 0;
			virtual void move(int x, int y) = 0;

			virtual const char* getTitle() const = 0;
			virtual void setTitle(const char*) = 0;

			virtual void setPadding(int top, int bottom, int left, int right) = 0;
			virtual void setPaddingTop(int) = 0;
			virtual void setPaddingTopPercent(double);
			virtual void setPaddingBottom(int) = 0;
			virtual void setPaddingBottomPercent(double);
			virtual void setPaddingLeft(int) = 0;
			virtual void setPaddingLeftPercent(double);
			virtual void setPaddingRight(int) = 0;
			virtual void setPaddingRightPercent(double);
			virtual void setPaddingToCenterContent(const char*);

			virtual void clearContent() = 0;
			virtual void refreshContent() = 0;
			virtual void redraw() = 0;

			virtual void show() = 0;
			virtual void hide() = 0;

			virtual void scrollUp() = 0;
			virtual void scrollDown() = 0;

			virtual void scrollToTop() = 0;
			virtual void scrollToBottom() = 0;

			virtual void scrollLeft() = 0;
			virtual void scrollRight() = 0;

			virtual void scrollToLeftEnd() = 0;
			virtual void scrollToRightEnd() = 0;

			virtual void writeCharAt(char, int x, int y) = 0;
			virtual void writeStrAt(const char*, int x, int y) = 0;

			virtual void setWordWrap(bool wordWrap) { this->wordWrap = wordWrap; }
			virtual void replaceText(const char*) = 0;
			virtual void appendText(const char*) = 0;

			virtual Window* makeChild(int w, int h) = 0; //adds child at 0,0 relative to this window's top-left corner
			virtual Window* makeChildCentered(int w, int h) = 0;
			virtual Window* makeChild(int w, int h, int x, int y) = 0;

			virtual Window* makeChildWithContent(const char*) = 0;
			virtual Window* makeChildWithContentCentered(const char*) = 0;
			virtual Window* makeChildWithContent(const char*, int x, int y) = 0;

			virtual Window* getParent() { return parent; }
			virtual void detachChild(Window*);
	};

	bool isWithinBounds(int x, int y, int minX, int minY, int maxX, int maxY);
	void getBoxSizeForText(const char* text, int& textW, int& textH, int hBorderSizeSum, int vBorderSizeSum);
	int constrainValue(int value, int min, int max);
	bool isWhitespaceChar(char);
}

#endif
