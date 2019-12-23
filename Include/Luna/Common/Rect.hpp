#pragma once

#include <algorithm>
#include <optional>
#include <ostream>

namespace Luna
{
  template <typename T> class Rect
  {
    /* The coordinates of the rectangle. */
    T fX0, fY0, fX1, fY1;

    /***********************************************************************//**
     * Make sure that (X0, Y0) specify the small coordinate and (X1, Y1)
     * specify the large coordinate.
     **************************************************************************/
    void validate()
    {
      if(fX0 > fX1)
        std::swap(fX0, fX1);

      if(fY0 > fY1)
        std::swap(fY0, fY1);
    }

  public:

    /***********************************************************************//**
     * Create a new rectangle object. The coordinates of the rectangle corners
     * are always calculated so that the (fX0, fX0) is the smallest coordinate
     * and (fX1, fY1) is the largest coordinate, regardless of the sign of the
     * width and height properties supplied. This guarentee reduces the
     * complexity of other algorithms that use the rect object.
     *
     * @param x       The X coordinate of the rectangle.
     * @param y       The Y coordinate of the rectangle.
     * @param width   The Width of the rectangle.
     * @param height  The height of the rectangle.
     **************************************************************************/
    Rect(T x = 0, T y = 0, T width = 0, T height = 0) : fX0(x), fY0(y),
      fX1(x + width), fY1(y + height)
    {
      validate();
    }

    T x0() const
    {
      return fX0;
    }

    T y0() const
    {
      return fY0;
    }

    T x1() const
    {
      return fX1;
    }

    T y1() const
    {
      return fY1;
    }

    /***********************************************************************//**
     * Calculate and return the intersection of two rectangles if it exist.
     *
     * @param rect  The other rectangle to check against.
     * @return      False if no intersection, else the rectangle that formed
     *              from the intersection of the rectangles.
     **************************************************************************/
    std::optional<Rect<T>> intersect(const Rect<T> & rect)
    {
      /* Check for an intersection using exclusion. */
      if(fX0 > rect.fX1 || fX1 < rect.fX0 ||
         fY0 > rect.fY1 || fY1 < rect.fY0)
      {
        /* No intersection is possible. */
        return {};
      }

      /* Calculate the intersection of the two rectangles. */
      Rect temp;
      temp.fX0 = fX0 < rect.fX0 ? rect.fX0 : fX0;
      temp.fX1 = fX1 > rect.fX1 ? rect.fX1 : fX1;
      temp.fY0 = fY0 < rect.fY0 ? rect.fY0 : fY0;
      temp.fY1 = fY1 > rect.fY1 ? rect.fY1 : fY1;

      /* Return the intersection between the rectangles. */
      return temp;
    }
  };

  /*************************************************************************//**
   * The overloaded stream insertion operator to print the coordinates of the
   * rectangle to an output stream.
   *
   * @param os    The output stream to print too.
   * @param rect  The rectangle to print to the output stream.
   * @return      The output stream that was supplied.
   ****************************************************************************/
  template <typename T>
  std::ostream & operator << (std::ostream & os, const Rect<T> & rect)
  {
    os << "[" << rect.x0() << ", " << rect.y0() << "] - [" << rect.x1() <<
          ", " << rect.y1() << "]";
    return os;
  }
}
