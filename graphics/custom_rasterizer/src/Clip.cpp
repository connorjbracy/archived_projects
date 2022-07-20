/*!
*****************************************************************************
\file   Clip.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #7
\date   07/17/2015

\brief
  This file contains the implementation of the Clip class member functions 
  necessary for clipping lines and polygons to a convex polygon.
*******************************************************************************/
#include "Clip.h"
#include "HalfPlane.h"
  
  /* Stores the number of vertices in the standard square. */
static const int   gStdSqVertCount = 4;
  /* Pre-define an array of vertices corresponding to the standard square. */
static const Point gStdSqVerts[gStdSqVertCount] = { Point(-1.0f, -1.0f), 
                                                    Point(-1.0f,  1.0f), 
                                                    Point( 1.0f,  1.0f), 
                                                    Point( 1.0f,  -1.0f) };

  /* Helper function for constructors to localize initialization proceedure. */
static void SetHalfPlaneRange(      std::vector<Hcoords> &half_planes,
                              const Point                *begin, 
                                    int                   size)
{
      /* 
       * Loop through vertex pairs to initialize the half-planes of our polygon.
       * Get a reference to the necessary points to instantiate the current 
       * half-plane in order to prevent undefined behavior with using [i] 
       * multiple times in one statement with side-effects to [i] (e.g. i++). 
       */
  for (int i = 0; i < size; )
  {
      /* Get a reference to the current point and iterate to the next point. */
    const Point &pt0 = begin[i++];
      /* Get a reference to the next point and temporarily iterate to the next point. */
    const Point &pt1 = begin[i++ % size];
      /* 
       * Since our polygon is guaranteed to be convex, any other point on 
       * the polygon will be inside the current half-plane so use the following 
       * point as our "inside" point, then iterate back to the next point to be used 
       * in the half-plane instantiation. 
       */
    half_planes.push_back(HalfPlane(pt0, pt1, begin[i-- % size]));
  }
}

/*
 * Default constructor creates an instance of the class for clipping to the
 * standard square; i.e., the square with vertices (±1, ±1).
 */
Clip::Clip(void)
{
  SetHalfPlaneRange(half_planes, gStdSqVerts, gStdSqVertCount);
}

/*
 * Non­default constructor creates an instance of the class for clipping to
 * the convex polygon with n vertices stored in the array clip to verts.
 * The vertices should be assumed to be those of a convex polygon, ordered
 * either in a clockwise or counterclockwise manner.
 */
Clip::Clip(int n, const Point *points)
{
  SetHalfPlaneRange(half_planes, points, n);
}

/*
 * Computes the result of clipping the line segment AB to the convex polygon.
 * If there is no intersection between the line segment and the polygon, the
 * return value is false. Otherwise, the return value is true, and the values
 * of A and B are modified to hold the endpoints of the clipped line segment.
 */
bool Clip::operator()(Point &pt_P, Point &pt_Q)
{
    /* Loop through each half-plane to clip the given line segment. */
  for (auto iter = half_planes.begin(); iter != half_planes.end(); ++iter)
  {
      /* Get the interval of intersection for the current half-plane. */
    Interval interval = ClipSegment(*iter, pt_P, pt_Q);
      /* 
       * If we found an empty interval (i.e. [pt_P] and [pt_Q] are both 
       * outside the current half-plane) stop clipping the segment 
       * entirely as it is outside out polygon. 
       */
    if (interval.IsEmpty())
      return false;

    const Point P = pt_P;

      /* If we found a valid interval, clip the points to the current interval. */
    pt_P = (P + (interval.bgn * (pt_Q - P)));
    pt_Q = (P + (interval.end * (pt_Q - P)));
  }
    /* If we made it this far, we clipped the line segment. */
  return true;
}

/*
 * Computes the result of clipping the polygon P with vertices stored in the
 * vector clip verts to the convex polygon. If there is no intersection
 * between P and the convex polygon, the return value is false.
 * Otherwise, the return value is true, and the vector clip verts is modified
 * to hold the vertices of the clipped polygon.
 */
bool Clip::operator()(std::vector<Point> &verts)
{
  //bool hasClipped = false;

    /* Loop through each half-plane of the clipping plane. */
  for (auto plane_iter = half_planes.begin(); plane_iter != half_planes.end(); ++plane_iter)
  {
      /* Store the input vertices. */
    temp_points = verts;
      /* Clear the ouput vertices. */
    verts.clear();
      /* Get the number of input vertices. */
    int size = temp_points.size();
      /* Loop through each vertex pair of the given vertices and clip them to the clipping plane. */
    for (int i = 0; i < size; )
    {
        /* Get the current vertex. */
      Point &vertex0 = temp_points[i++];
        /* Get the next vertex.    */
      Point &vertex1 = temp_points[i % size];
        /* If vertex0 is inside the half-plane... */
      if (dot(*plane_iter, vertex0) <= 0.0f)
      {
          /* ...them add vertex0 to the list of output vertices. */
        verts.push_back(vertex0);
          /* If vertex1 is outside the half-plane, add the clipped vertex of vertex1 to the output vertices. */
        if (dot(*plane_iter, vertex1) > 0.0f)
        {
          verts.push_back(vertex0 + (ClipSegment(*plane_iter, vertex0, vertex1).end * (vertex1 - vertex0)));
        }
      }
        /* Otherwise, vertex0 is outside the half-plane so check if vertex1 is inside... */
      else if (dot(*plane_iter, vertex1) <= 0.0f)
      {
          /* ...and clip vertex0 if vertex1 is inside the half-plane. */
        verts.push_back(vertex0 + (ClipSegment(*plane_iter, vertex0, vertex1).bgn * (vertex1 - vertex0)));
      }
    }
  }

  return (verts.size() > 0);
}
