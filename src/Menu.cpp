#include <stddef.h> //NULL
#include "Menu.h"

using namespace winterchange;

template <typename T>
EXTERNAL_FUNC
Menu<T>::Menu()
	: dataModel(NULL),
	  dataView(NULL)
{

}

template <typename T>
EXTERNAL_FUNC
Menu<T>::Menu(MenuDataModel<T>* dataModel, MenuDataView<T>* dataView)
	: dataModel(dataModel),
	  dataView(dataView)
{

}

/* Sets internal MenuDataModel pointer to point to the provided dataModel.
 *
 * NOTE: older pointer will not be deleted - it is up to the caller to
 * manage lifecycle of provided models.
 */
template <typename T>
EXTERNAL_FUNC
void Menu<T>::setData(MenuDataModel<T>* dataModel)
{
	this->dataModel = dataModel;
}

/* Sets internal MenuDataView pointer to point to the provided dataView.
 *
 * NOTE: older pointer will not be deleted - it is up to the caller to
 * manage lifecycle of provided views.
 */
template <typename T>
EXTERNAL_FUNC
void Menu<T>::setDataView(MenuDataView<T>* dataView)
{
	this->dataView = dataView;
}

/* Loops over all data in dataModel, and asks dataView to draw each datum,
 * providing the current position in the model and the selection state of the
 * datum.
 *
 * Assumes dataModel and dataView have either been set with valid pointers or
 * are NULL. In the latter case, calling this method does nothing.
 */
template <typename T>
EXTERNAL_FUNC
void Menu<T>::drawAllData()
{
	if(dataModel == NULL || dataView == NULL)
	{
		return;
	}

	for(int i = 0; i < dataModel->size(); i++)
	{
		T data = dataModel->getDataAt(i);
		bool selected = dataModel->isSelected(i);

		dataView->drawData(data, i, selected);
	}
}
