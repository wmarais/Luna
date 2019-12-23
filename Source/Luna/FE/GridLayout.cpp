#include "../../../Include/Luna/FE/GridLayout.hpp"

using namespace Luna::FE;

/******************************************************************************/
GridLayout::GridLayout(size_t rows, size_t cols) : fNumRows(0), fNumCols(0)
{
  resize(rows, cols);
}

/******************************************************************************/
GridLayout::~GridLayout()
{

}

/******************************************************************************/
void GridLayout::resize(size_t rows, size_t cols)
{
  /* Check if there is anything to do. */
  if(rows == fNumRows && cols == fNumCols)
    return;

  /* Allocate the new memory. */
  std::unique_ptr<std::shared_ptr<Object>[]> temp =
      std::make_unique<std::shared_ptr<Object>[]>(rows * cols);

  /* Iterate through each row, and copy the columns across. */
  for(size_t r = 0; r < fNumRows; r++)
  {
    memcpy(&(temp[r * cols]), &(fCells[r * fNumCols]), fNumCols);
  }

  /* Store the new size and memory. */
  fNumRows = rows;
  fNumCols = cols;
  fCells = std::move(temp);
}

/******************************************************************************/
void GridLayout::setCell(size_t row, size_t col, std::shared_ptr<Object> obj)
{
  /* Check to see if the memory must be grown to fit the cell. */
  if(row > fNumRows || col > fNumCols)
    resize(row, col);

  /* Set the contents of the cell. */
  fCells[row * fNumRows + col] = obj;
}

/******************************************************************************/
void GridLayout::render()
{

}
