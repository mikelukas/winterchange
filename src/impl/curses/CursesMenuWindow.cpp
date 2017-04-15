#include "CursesMenuWindow.h"

using namespace winterchange;

template <typename T>
EXTERNAL_FUNC
CursesMenuWindow<T>::CursesMenuWindow(int w, int h, MenuDataModel<T>* data, char* (*dataToString)(T))
	: CursesWindow(w, h),
	  menu(data, this),
	  dataToString(dataToString)
{

}

/* Asks the menu to start drawing all its data (which will trigger calls to our
 * drawData() method for each menu item.
 */
template <typename T>
EXTERNAL_FUNC
void CursesMenuWindow<T>::redraw()
{
	menu.drawAllData();
	CursesWindow::redraw();
}

/* Draws one menu datum into the window, highlighting it if it's selected.*/
template <typename T>
EXTERNAL_FUNC
void CursesMenuWindow<T>::drawData(T data, int position, bool selected)
{
	//If selected, we want to highlight the option by reversing video
	if(selected)
	{
		writeCharAt('>', 0, position);
	}
	else
	{
		writeCharAt(' ', 0, position);
	}

	//Convert the data to a string first, then write it into the window
	char* dataAsStr = (*dataToString)(data);
	writeStrAt(dataAsStr, 2, position);
	delete dataAsStr;
}
