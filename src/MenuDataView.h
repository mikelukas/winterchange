#ifndef MENUDATAVIEW_H_
#define MENUDATAVIEW_H_

#include "LibMacros.h"

namespace winterchange
{
	/* Interface for how 1 item of menu data should be displayed, in order to
	 * abstract how data is stored from how it is displayed.
	 */
	template <typename T>
	class MenuDataView
	{
		public:
			MenuDataView() {}
			virtual ~MenuDataView() {}

			virtual void drawData(T data, int position, bool selected) = 0;
	};
}

#endif
