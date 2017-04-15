#ifndef MENUDATAMODEL_H_
#define MENUDATAMODEL_H_

#include "LibMacros.h"

namespace winterchange
{
	/* Interface used by Menu for abstracting how data is stored and manipulated
	 * from how it is accessed for display. For example, use this to encapsulate
	 * STL containers for use across the DLL boundary.
	 *
	 * NOTE: changing the model's data will not cause the display to change.
	 * Code backed by the model must be told by some other means to update its
	 * display.
	 */
	template <typename T>
	class EXTERNAL_CLASS MenuDataModel
	{
		public:
			MenuDataModel() {}
			virtual ~MenuDataModel() {}

			virtual int size() const = 0;
			virtual bool isEmpty() const = 0;

			virtual T getDataAt(int) = 0;

			virtual bool isSelected(int) const = 0;
			virtual void toggleSelected(int) = 0;
	};
}

#endif
