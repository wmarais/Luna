#pragma once

#include <vector>

#include "Object.hpp"

namespace Luna::FE
{
  class GridLayout : public Object
  {
    size_t fNumRows;
    size_t fNumCols;

    std::unique_ptr<std::shared_ptr<Object>[]> fCells;


    void resize(size_t rows, size_t cols);

  public:
    GridLayout(size_t rows = 0, size_t cols = 0);

    virtual ~GridLayout() override;

    void render() override;

    void setCell(size_t row, size_t col, std::shared_ptr<Object> obj);



  };
}
