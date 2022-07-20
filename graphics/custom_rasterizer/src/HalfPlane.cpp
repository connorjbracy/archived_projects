/*!
*****************************************************************************
\file   HalfPlane.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #7
\date   07/17/2015

\brief
  This file contains the implementation of the functions necessary for 
  initializing a half-plane and clipping line segments to half-planes.
*******************************************************************************/
#include "HalfPlane.h"

/*
 * Computes the dot product of the half­plane p, which specified by its
 * homogeneous coordinate representation, and the point Q.
 */
float dot(const Hcoords &halfPlane, const Point &pt_Q)
{
  return (halfPlane.x * pt_Q.x) +
         (halfPlane.y * pt_Q.y) +
         (halfPlane.w * pt_Q.w);
}

/*
 * Computes the homogeneous coordinate representation of the half­plane with
 * boundary passing through the points A and B, and containing the interior
 * point P . Note that that return value p should be such that:
 *   p · A = 0, p · B = 0, and p · P < 0.
 * You are to assume that the points A, B, P are non­colinear.
 */
Hcoords HalfPlane(const Point &pt_A, const Point &pt_B, const Point &pt_Inside)
{
    /* Create the half-plane using the normal vector of the line AB */
  Hcoords halfPlane((pt_A.y - pt_B.y), (pt_B.x - pt_A.x), 0.0f);
  halfPlane.w = -dot(halfPlane, pt_A);

  return ((dot(halfPlane, pt_Inside) < 0.0f)? halfPlane : -halfPlane);
}

/*
 * Computes the intersection interval I = [a, b] that corresponds to the
 * intersection of the half­plane p and the line segment P Q with
 * endpoints P, Q. If the intersection is empty, then I = ; i.e., a > b.
 * If the I is not empty, then the intersection of p and P Q is the line
 * segment P Q , where P = P + a(Q - P ) and Q = P + b(Q - P ).
 */
Interval ClipSegment(const Hcoords &halfPlane,
                     const Point   &pt_P,
                     const Point   &pt_Q)
{
  float dot_P = dot(halfPlane, pt_P);
  float dot_Q = dot(halfPlane, pt_Q);

  auto get_t_intersect = [dot_P, dot_Q] { return (dot_P / (dot_P - dot_Q)); };

    /* If both points are INSIDE the half-plane, return the full interval. */
  if ((dot_P < 0.0f) && (dot_Q < 0.0f))
    return Interval(0.0f, 1.0f);
    /* If both points are OUTSIDE the half-plane, return an empty interval. */
  else if ((dot_P >= 0.0f) && (dot_Q >= 0.0f))
    return Interval(1.0f, 0.0f);
    /* If [pt_P] is INSIDE and [pt_Q] is OUTSIDE, return the interval [0,t]. */
  else if (dot_P < 0.0f)
     return Interval(0.0f, get_t_intersect());
    /* If [pt_Q] is INSIDE and [pt_P] is OUTSIDE, return the interval [t,0]. */
  else
     return Interval(get_t_intersect(), 1.0f);
}
