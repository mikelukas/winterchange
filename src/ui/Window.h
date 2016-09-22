#ifndef UI_WINDOW_H_
#define UI_WINDOW_H_

#include <string>
#include <vector>

using std::string;
using std::vector;

/* Abstract base class for manipulating windows; intended to keep details of the
 * underlying low-level window implementation abstract */
class Window
{
	protected:

		Window* parent;
		vector<Window*> children;

	public:
		Window() : parent(NULL) {}
		Window(Window* parent) : parent(parent) {}
		virtual ~Window() {}

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
		virtual void replaceText(const string&) = 0;
		virtual void appendText(const string&) = 0;
		virtual void appendText(const string&, bool newline) = 0;

		virtual Window* makeChild(int w, int h) = 0; //adds child at 0,0 relative to this window's top-left corner
		virtual Window* makeChildCentered(int w, int h) = 0;
		virtual Window* makeChild(int w, int h, int x, int y) = 0;

		virtual Window* getParent() { return parent; }
};

bool isWithinBounds(int x, int y, int minX, int minY, int maxX, int maxY);
int constrainValue(int value, int min, int max);
bool isWhitespaceChar(char);

#endif
