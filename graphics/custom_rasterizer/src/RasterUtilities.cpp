/*!
*****************************************************************************
\file   RasterUtilities.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #4
\date   06/04/2015

\brief
  This is the definition file for the functions used to rasterize a rectangle.
  Operations include:
    - Drawing a rectangle (border/edges)
    - Filling in a rectangle

*******************************************************************************/
#include "RasterUtilities.h"

/* 
  * To prevent needless GotoPoint calls, 
  * we are going to traverse back and forth in the x-direction. 
  *   The first  row will be traversed from x0 to x1, 
  *   The second row will be traversed from x1 to x0, 
  *   The third  row will be traversed from x0 to x1 again, etc. 
  */

  /* Typedef the pointer-to-member-function needed to call (In/De)crementX. */
typedef void (Raster::*RasterIterator)(void);
  /* Create a static array referencing both functions used to iterate X. */
static const RasterIterator Traversals[] = { &Raster::IncrementX, 
                                             &Raster::DecrementX };
  /* Macro used to make calls to pointer-to-member-function more readable. */
#define ITERATE(raster, traversal) (raster.*traversal)()

static void DrawRightLine(Raster &raster, RasterIterator iterate, int length);

/*
 * Fills in a solid axis aligned rectangle, using the current foreground color.
 * The coordinates (x, y) give the screen coordinates of the lower left hand
 * corner of the rectangle. The rectangle should be W pixels wide and H pixels
 * tall.
 */
void FillRect(Raster &raster, int x, int y, int width, int height)
{
    /* Go to the starting point of the rectangle being drawn. */
  raster.GotoPoint(x, y);
    /* 
     * Prevent writing offset that occurs when switching 
     * from iterating in one direction to another. 
     */
  --width;

    /* Loop through each row of the rectangle. */
  for (int iRow = 0; iRow < height; ++iRow)
  {
      /* Set the function currently needed to iterate along the x-axis. */
    const RasterIterator &fnTraverseX = Traversals[iRow % 2];
      /* Loop through each pixel in the row setting its color. */
    for (int iCol = 0; iCol < width; ++iCol)
    {
        /* Set the the current pixel in the buffer to the foreground color. */
      raster.WritePixel();
        /* Move to the next un-written to pixel in the current row. */
      ITERATE(raster, fnTraverseX);
    }
      /* Set the the current pixel in the buffer to the foreground color. */
    raster.WritePixel();
      /* Move up to the next row. */
    raster.IncrementY();
  }
}

/*
 * Draws the boundary of an axis aligned rectangle, using the current
 * foreground color (the interior is not filled in). The coordinates (x, y)
 * give the screen coordinates of the lower left hand corner of the rectangle.
 * The rectangle should be W pixels wide and H pixels tall.
 */
void DrawRect(Raster &raster, int x, int y, int width, int height)
{
    /* Go to the starting point of the rectangle being drawn. */
  raster.GotoPoint(x, y);
  
    /* 
     * Draw each border from its starting point 
     * (starting with the bottom left corner) 
     * to the pixel just before the next edge's start. 
     */
  DrawRightLine(raster, &Raster::IncrementX, --width);
  DrawRightLine(raster, &Raster::IncrementY, --height);
  DrawRightLine(raster, &Raster::DecrementX,   width);
  DrawRightLine(raster, &Raster::DecrementY,   height);
}

void DrawRightLine(Raster &raster, RasterIterator iterate, int length)
{
  for (/* length = length */; length; --length)
  {
    raster.WritePixel();
    ITERATE(raster, iterate);
  }
}
