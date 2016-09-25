#ifndef UI_EXPANDABLEBUFFER2D_H_
#define UI_EXPANDABLEBUFFER2D_H_

/* Contains a 2D matrix of content (determined by T), which can be written to
 * at any coordinates, and will automatically expand if an attempt to write to
 * out-of-bounds coordinates is made.
 */
template <typename T>
class ExpandableBuffer2D
{
	protected:

		int rows;
		int cols;

		T* buffer; //1D array of 2D content; will be of size rows x cols
		T defaultVal;

		virtual void expand();

	public:
		ExpandableBuffer2D(int rows, int cols, T defaultVal);
		virtual ~ExpandableBuffer2D();

		virtual void writeAt(T, int row, int col);
};

#include "ExpandableBuffer2D.cpp"

#endif