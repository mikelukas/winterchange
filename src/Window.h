#ifndef WINDOW_H_
#define WINDOW_H_

#include <set>
#include <string>

using std::set;
using std::string;

namespace winterchange
{
	/* Abstract base class for manipulating windows; intended to keep details of the
	 * underlying low-level window implementation abstract */
	class Window
	{
		protected:

			Window* parent;
			set<Window*> children;

			bool wordWrap;

		public:
			Window();
			Window(Window* parent);
			virtual ~Window();

			virtual int getWidth() const = 0;
			virtual int getHeight() const = 0;
			virtual void resize(int w, int h) = 0;
			virtual void resize(int w, int h, int x, int y) = 0;

			virtual int getX() const = 0;
			virtual int getY() const = 0;
			virtual void move(int x, int y) = 0;

			virtual const string& getTitle() const = 0;
			virtual void setTitle(const string&) = 0;

			virtual void setPadding(int top, int bottom, int left, int right) = 0;
			virtual void setPaddingTop(int) = 0;
			virtual void setPaddingBottom(int) = 0;
			virtual void setPaddingLeft(int) = 0;
			virtual void setPaddingRight(int) = 0;

			virtual void clearContent() = 0;
			virtual void refreshContent() = 0;
			virtual void redraw() = 0;

			virtual void scrollUp() = 0;
			virtual void scrollDown() = 0;

			virtual void scrollToTop() = 0;
			virtual void scrollToBottom() = 0;

			virtual void scrollLeft() = 0;
			virtual void scrollRight() = 0;

			virtual void scrollToLeftEnd() = 0;
			virtual void scrollToRightEnd() = 0;

			virtual void writeCharAt(char, int x, int y) = 0;
			virtual void writeStrAt(const string&, int x, int y) = 0;

			virtual void setWordWrap(bool wordWrap) { this->wordWrap = wordWrap; }
			virtual void replaceText(const string&) = 0;
			virtual void appendText(const string&) = 0;
			virtual void appendText(const string&, bool newline) = 0;

			virtual Window* makeChild(int w, int h) = 0; //adds child at 0,0 relative to this window's top-left corner
			virtual Window* makeChildCentered(int w, int h) = 0;
			virtual Window* makeChild(int w, int h, int x, int y) = 0;

			virtual Window* makeChildWithContent(const string&) = 0;
			virtual Window* makeChildWithContentCentered(const string&) = 0;
			virtual Window* makeChildWithContent(const string&, int x, int y) = 0;

			virtual Window* getParent() { return parent; }
			virtual void detachChild(Window*);
	};

	bool isWithinBounds(int x, int y, int minX, int minY, int maxX, int maxY);
	void getBoxSizeForText(const string& text, int& textW, int& textH, int hBorderSizeSum, int vBorderSizeSum);
	int constrainValue(int value, int min, int max);
	bool isWhitespaceChar(char);
}

#endif
