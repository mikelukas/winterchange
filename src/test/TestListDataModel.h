#ifndef TEST_TESTLISTDATAMODEL_H_
#define TEST_TESTLISTDATAMODEL_H_

#include <string>
#include <vector>
#include "../MenuDataModel.h"

using std::string;
using std::vector;

const int NO_SELECTION = -1;

namespace winterchange
{
	/* Simple data model that just takes a vector of strings, and lets you select
	 * one at a time.
	 */
	class TestListDataModel : public MenuDataModel<string*>
	{
		protected:
			vector<string*>* testData;
			int selectedIndex;

		public:
			TestListDataModel(vector<string*>*);
			virtual ~TestListDataModel() {}

			virtual int size() const;
			virtual bool isEmpty() const;

			virtual string* getDataAt(int);

			virtual bool isSelected(int) const;
			virtual void toggleSelected(int);
	};
}

char* testTransformFunc(string*);

#endif
