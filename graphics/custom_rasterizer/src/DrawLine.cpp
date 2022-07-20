/*!
*****************************************************************************
\file   DrawLine.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #4
\date   06/04/2015

\brief
  This is the definition file for the functions used to rasterize a line.

*******************************************************************************/
#include "RasterUtilities.h"

  /* Typedef the pointer-to-member-function needed to call (In/De)crementX. */
typedef void (Raster::*RasterIterator)(void);
  /* Macro used to make calls to pointer-to-member-function more readable. */
#define ITERATE(raster, traversal) (raster.*traversal)()
  /* 
   * A bit mask that will absolute value a floating point number 
   * when &'ed together.  0111..1111
   */
const unsigned ABS_MASK = 0x7FFFFFFF;

  /* Quick rounding function. */
inline
int MyRound(float f) { return static_cast<int>(f + 0.5f); }
  /* Draw a line that is vertical (undefined). */
static void DrawUndefinedLine(Raster &raster, const Point &pt_P, const Point &pt_Q);

  /* 
   * Union of float and unsigned int so that we can do 
   * bit manipulation on the floating point value. 
   */
typedef union 
{
    /* The slope of the line. */
  float    m;
    /* A way to access the floating point value for bit-manipulation. */
  unsigned abs;
} Slope;

  /* A struct to encapsulate rasterizing a line segment. */
typedef struct
{
    /* The domain the line lies on. */
  int domain_min, domain_max;
    /* The range the line spans. */
  int range_int, range_old;
    /* Track the real equation of the line to convert to int form. */
  float range_float;
    /* The slope of the line being rasterized. */
  float slope;
    /* 
     * Pointer-to-member-function used to iterate 
     * the Raster object the line is being rasterized to. 
     */
  RasterIterator domain_iter, range_iter;

    /* Initialize the slope of the line based on two Points. */
  bool InitSlope(const Point &pt_P, const Point &pt_Q);
    /* Set the iterators used when rasterizing. */
  void InitIterators(RasterIterator domain_iterator, 
                     RasterIterator pos_range_iterator, 
                     RasterIterator neg_range_iterator);
    /* 
     * Initialize the values necessary for iterating through the pixels needed 
     * to rasterize an object based off of arbitrary points and regardless of 
     * which axis we are iterating with respect to. 
     */
  void InitLinePoints(float d_min, float d_max, float r_min);
    /* Rasterize the line to the Raster object. */
  void Rasterize(Raster &raster);
} LineSegment;

  /*
   * Initialize the slope of the line based on two Points. 
   */
bool LineSegment::InitSlope(const Point &pt_P, const Point &pt_Q)
{
    /* If the x-values of the line are the same, prevent undefined behavior. */
  if (Hcoords::Near(pt_P.x, pt_Q.x))
  {
    slope = 0.0f;
    return false;
  }
  else
    slope = ((pt_Q.y - pt_P.y) / (pt_Q.x - pt_P.x));

  return true;
}

  /*
   * Set the iterators used when rasterizing. 
   */
void LineSegment::InitIterators(RasterIterator domain_iterator, 
                                RasterIterator pos_range_iterator, 
                                RasterIterator neg_range_iterator)
{
  domain_iter = domain_iterator;

  if (slope >= 0.0f)
    range_iter = pos_range_iterator;
  else
    range_iter = neg_range_iterator;
}

  /* 
    * Initialize the values necessary for iterating through the pixels needed 
    * to rasterize an object based off of arbitrary points and regardless of 
    * which axis we are iterating with respect to. 
    */
void LineSegment::InitLinePoints(float d_min, float d_max, float r_min)
{
  domain_min  = MyRound(d_min);
  domain_max  = MyRound(d_max);
  range_float = (r_min + (slope * (static_cast<float>(domain_min) - d_min)));
  range_int   = MyRound(range_float);
}

  /*
   * Rasterize the line to the Raster object. 
   */
void LineSegment::Rasterize(Raster &raster)
{
    /* For each pixel on the domain of this line... */
  while ((domain_min++) <= domain_max)
  {
    raster.WritePixel();                 /* Draw the pixel.                */
    range_old    = range_int;            /* Save the current range value.  */
    range_float += slope;                /* Increment the actual value.    */
    range_int    = MyRound(range_float); /* Get the integer (pixel) value. */
    if (range_old != range_int)          /* If the integer value changed,  */
      ITERATE(raster, range_iter);       /* Iterate the rasterizer cursor. */
    ITERATE(raster, domain_iter);        /* Iterate to the next pxiel.     */
  }
}

/*
 * This function will scan convert the line segment from point P to point Q in
 * screen coordinates. Note that unlike the FillRect and DrawRect functions in
 * the first programming task, the coordinates are floating point values, and
 * not necessarily pixels.
 */
void DrawLine(Raster &raster, const Point &pt_P, const Point &pt_Q)
{
  LineSegment line;
  Slope       slope_magnitude;

    /* 
     * Calculate the slope between the points.
     * If we have an undefined line, handle it. 
     */
  if (!line.InitSlope(pt_P, pt_Q))
  {
    DrawUndefinedLine(raster, pt_P, pt_Q);
    return;
  }

    /* Get the absolute value of the line. */
  slope_magnitude.m    = line.slope;
  slope_magnitude.abs &= ABS_MASK;
  
    /* If (|m| <= 1), then scan with respect to the x-axis. */
  if (slope_magnitude.m <= 1.0f)
  {
      /* 
       * Set the proper iterators for w.r.t. x-axis 
       * and w.r.t. the sign of the slope. 
       */
    line.InitIterators(&Raster::IncrementX, 
                       &Raster::IncrementY, 
                       &Raster::DecrementY);
      /* 
       * Initialize the necessary variables to rasterize 
       * the line based on the points provided. 
       */
    if (pt_P.x < pt_Q.x)
      line.InitLinePoints(pt_P.x, pt_Q.x, pt_P.y);
    else
      line.InitLinePoints(pt_Q.x, pt_P.x, pt_Q.y);
      /* Set the Raster object to the proper starting location. */
    raster.GotoPoint(line.domain_min, line.range_int);
  }
  else /* Otherwise, scan with respect to the y-axis. */
  {
      /* 
       * Set the proper iterators for w.r.t. y-axis 
       * and w.r.t. the sign of the slope. 
       */
    line.InitIterators(&Raster::IncrementY, 
                       &Raster::IncrementX, 
                       &Raster::DecrementX);
      /* Since we are iterating w.r.t. the y-axis, invert the slope. */
    line.slope = (1.0f / line.slope);    
      /* 
       * Initialize the necessary variables to rasterize 
       * the line based on the points provided. 
       */
    if (pt_P.y < pt_Q.y)
      line.InitLinePoints(pt_P.y, pt_Q.y, pt_P.x);
    else
      line.InitLinePoints(pt_Q.y, pt_P.y, pt_Q.x);
      /* Set the Raster object to the proper starting location. */
    raster.GotoPoint(line.range_int, line.domain_min);
  }
    /* Rasterize the line to the Raster object's pixel buffer. */
  line.Rasterize(raster);
}

  /*
   * Given a Raster object and two points known to be undefined in 
   * cartesian coordinates, draw the line to the Raster object's pixel buffer.
   */
void DrawUndefinedLine(Raster &raster, const Point &pt_P, const Point &pt_Q)
{
    /* Get the pixel x-coordinate. */
  int x = MyRound(pt_P.x);
  int yMin;
  int yMax;
  
    /* Get the starting and ending pixel y-coordinates. */
  if (pt_P.y < pt_Q.y)
  {
    yMin = MyRound(pt_P.y);
    yMax = MyRound(pt_Q.y);
  }
  else
  {
    yMin = MyRound(pt_Q.y);
    yMax = MyRound(pt_P.y);
  }
    /* Set the Raster object to the proper starting location. */
  raster.GotoPoint(x, yMin);
    /* Draw the line. */
  for (/* length = length */; yMin <= yMax; ++yMin)
  {
    raster.WritePixel();
    raster.IncrementY();
  }
}
