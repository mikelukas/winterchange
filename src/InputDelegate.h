#ifndef INPUTDELEGATE_H_
#define INPUTDELEGATE_H_

#include "LibMacros.h"

namespace winterchange
{
	/* Abstracts implementation of capturing input from winterchange windows, so
	 * that different window implementations can provide different ways of
	 * getting input from them.
	 *
	 * Unfortunately does not handle unicode yet.
	 */
	class EXTERNAL_CLASS InputDelegate
	{
		public:
			InputDelegate() {};
			virtual ~InputDelegate() {}

			virtual int awaitKey() = 0;
			virtual char* awaitCString(const int maxLen) = 0;
	};
}

#endif
