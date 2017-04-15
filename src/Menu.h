#ifndef MENU_H_
#define MENU_H_

#include "LibMacros.h"
#include "MenuDataModel.h"
#include "MenuDataView.h"

namespace winterchange
{
	/* Displays data in menu form.  Delegates data management to MenuDataModel
	 * object, and data display to MenuDataView.
	 *
	 * To use, provide an instance of this class with your data encapsulated
	 * within an instance of MenuDataModel and a way to display a given line
	 * of that data in the form of an instance of MenuDataView that you
	 * implement, then call drawAllData();
	 */
	template <typename T>
	class EXTERNAL_CLASS Menu
	{
		protected:
			MenuDataModel<T>* dataModel;
			MenuDataView<T>* dataView;

		public:
			Menu();
			Menu(MenuDataModel<T>*, MenuDataView<T>*);
			virtual ~Menu() {}

			virtual void setData(MenuDataModel<T>*);
			virtual void setDataView(MenuDataView<T>*);

			virtual void drawAllData();
	};
}

#include "Menu.cpp"

#endif
