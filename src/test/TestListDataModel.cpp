#include "TestListDataModel.h"

using namespace winterchange;

TestListDataModel::TestListDataModel(vector<string*>* testData)
	: testData(testData),
	  selectedIndex(NO_SELECTION)
{

}

int TestListDataModel::size() const
{
	return testData->size();
}

bool TestListDataModel::isEmpty() const
{
	return testData->empty();
}

/* Returns data contents at given index */
string* TestListDataModel::getDataAt(int index)
{
	return (*testData)[index];
}

/* Returns true if given index is selected, false, otherwise) */
bool TestListDataModel::isSelected(int index) const
{
	return (index == selectedIndex);
}

/* If given index is not selected, the previous selection (if any) will be cleared
 * and the given index will be set as the selected row.
 * If it is already selected, it will be unselected.
 */
void TestListDataModel::toggleSelected(int index)
{
	if(selectedIndex != index) {
		selectedIndex = index;
	}
	else
	{
		selectedIndex = NO_SELECTION;
	}
}

/* Adds an x at the end of eveyr string that's been transformed */
char* testTransformFunc(string* data)
{
	const char* origCStr = data->c_str();
	char* newCStr = new char[data->length() + 2]; //+1 for extra char at end, +1 for \0
	strcpy(newCStr, origCStr);
	newCStr[data->length()] = 'x';
	newCStr[data->length() + 1] = '\0';

	return newCStr;
}
