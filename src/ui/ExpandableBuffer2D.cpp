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
 * to accommodate. Doubles buffer size in each dimension that needs expanded.
 */
template <typename T>
void ExpandableBuffer2D<T>::writeAt(T content, int row, int col)
{
	//check for resizing scenarios; only expand the dimension we need more room in
	int newRows = rows, newCols = cols;
	if(row > rows-1)
	{
		newRows = rows << 1; // << 1 == multiply by 2
	}
	if(col > cols-1)
	{
		newCols = cols << 1;
	}

	//Perform a resize if either of the dimensions needs expanded
	if(newRows != rows || newCols != cols)
	{
		expand(newRows, newCols);
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

/* Creates a new buffer of the given size, and copies everything from the old
 * one into the new one, then deallocates the old one.
 */
template <typename T>
void ExpandableBuffer2D<T>::expand(int newRows, int newCols)
{
	//Make new buffer
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
