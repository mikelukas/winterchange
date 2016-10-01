#include <algorithm>
#include "ExpandableBuffer2D.h"

template <typename T>
ExpandableBuffer2D<T>::ExpandableBuffer2D(int rows, int cols, T defaultVal)
	: rows(rows),
	  cols(cols),
	  defaultVal(defaultVal)
{
	buffer = new T[rows * cols];
	clear();
}

template <typename T>
ExpandableBuffer2D<T>::~ExpandableBuffer2D()
{
	delete[] buffer;
}

template <typename T>
int ExpandableBuffer2D<T>::getWidth() const
{
	return cols;
}

template <typename T>
int ExpandableBuffer2D<T>::getHeight() const
{
	return rows;
}

/* Writes the given content into the buffer at the given row,col coords. If the
 * coords fall outside of the buffer, the buffer size will be expanded first
 * to accommodate. See expand() for how expansion is performed.
 */
template <typename T>
void ExpandableBuffer2D<T>::writeAt(T content, int row, int col)
{
	if(row > rows-1 || col > cols-1)
	{
		expand();
	}

	buffer[row*cols + col] = content;
}

/* Fills the entire buffer w/ defaultVal at every position */
template <typename T>
void ExpandableBuffer2D<T>::clear()
{
	std::fill(buffer, buffer + (rows * cols), defaultVal);
}

/* Fills the buffer w/ defaultVal at each position, starting at given row,col.
 * Everything before these coordinates will be untouched.*/
template <typename T>
void ExpandableBuffer2D<T>::clearFrom(int startRow, int startCol)
{
	int firstCol = startCol;
	for(int row = startRow; row < getHeight(); row++)
	{
		for(int col = firstCol; col < getWidth(); col++)
		{
			buffer[row*cols + col] = defaultVal;
		}

		firstCol = 0;
	}
}

/* Doubles the buffer's dimensions. Creates the new buffer, copies everything
 * from the old one into the new one, then deallocates the old one.
 */
template <typename T>
void ExpandableBuffer2D<T>::expand()
{
	//Make new buffer
	int newRows = rows << 1, newCols = cols << 1; // << 1 == multiply by 2
	T* newBuffer = new T[newRows * newCols];
	std::fill(newBuffer, newBuffer + (newRows * newCols), defaultVal);

	//Copy old buffer into new one, at exact same positions
	int oldRowOffset, newRowOffset;
	for(int row = 0 ; row < rows; row++)
	{
		oldRowOffset = row*cols;
		newRowOffset = row*newCols;
		for(int col = 0; col < cols; col++)
		{
			newBuffer[newRowOffset + col] = buffer[oldRowOffset + col];
		}
	}

	//Update instance with new buffer contents and size
	delete[] buffer;
	buffer = newBuffer;

	rows = newRows;
	cols = newCols;
}
