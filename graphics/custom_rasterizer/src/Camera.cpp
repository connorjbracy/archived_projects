/*!
*****************************************************************************
\file   Camera.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #3
\date   05/27/2015

\brief
  This file contains the implemention definitions of the Camera class as well
  as the definitions of some non-class bound helper functions used to
  manipulate Camera objects.
*******************************************************************************/
#include "Camera.h"
#include "Affine.h"

//jsh:
//#define center center
//#define right  right
//#define up     up
//#define width  width
//#define height height


/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/

  /*
   * Default Constructor. Creates a camera whose world space viewport is the
   * standard square: the square centered at the origin with width and height
   * of 2.
   */
Camera::Camera(void)
  : center(0.0f, 0.0f),
    right (1.0f, 0.0f),
    up    (0.0f, 1.0f),
    width (2.0f),
    height(2.0f)
{
}

  /*
   * Non-Default constructor creates a camera whose world space view-port is
   * the rectangle centered at C, with right vector u, up vector v,
   * width [W], and height [H]. The (unit) vector v points in the same
   * direction as the vector [up], and is a 90-degree counter-clockwise
   * rotation of the (unit) vector u.
   */
Camera::Camera(const Point &_center, const Vector &_up, float _width, float _height)

  : center(_center),
    right(),
    up(_up),
    width (_width),
    height(_height)
{
    /*
     * Normalize the [up] vector before assigning its 90-degree-clockwise
     * rotation to the [right] vector in order to avoid normalizing twice.
     */
  up.Normalize();
    /*
     * Flip order and sign of the [up] vector's components as necessary in
     * order to rotate it without much computation.
     */
  right.x =  up.y;
  right.y = -up.x;
}

  /* Returns the center of the camera in world coordinates. */
Point Camera::Center(void) const { return center; }

  /* Returns the right vector of the camera in world coordinates. */
Vector Camera::Right(void) const { return right; }

  /* Returns the up vector of the camera in world coordinates. */
Vector Camera::Up(void) const { return up; }

  /* Returns the width of the camera viewport in world coordinates. */
float Camera::Width(void) const { return width; }

  /* Returns the height of the camera viewport in world coordinates. */
float Camera::Height(void) const { return height; }

  /*
   * Moves the camera [x_displacement] world space units in a direction
   * parallel the the camera's right vector. A reference to the camera is
   * returned.
   */
Camera &Camera::MoveRight(float x_displacement)
{
    /*
     * Directly transform the center instead of using an Affine in order to
     * increase efficiency.
     */
  center.x += (x_displacement * right.x);
  center.y += (x_displacement * right.y);

  return *this;
}

  /*
   * Moves the camera y world space units in a direction parallel the the
   * camera's up vector. A reference to the camera is returned.
   */
Camera &Camera::MoveUp(float y_displacement)
{
    /*
     * Directly transform the center instead of using an Affine in order to
     * increase efficiency.
     */
  center.x += (y_displacement * up.x);
  center.y += (y_displacement * up.y);

  return *this;
}

  /*
   * Rotates the camera [angle_offset] radians counterclockwise about the
   * camera's center. A reference to the camera is returned.
   */
Camera &Camera::Rotate(float angle_offset)
{
    /* Rotate the [up] vector by the requested amount. */
  up = (Rot(angle_offset) * up);

    /*
     * "Rotate" the [right] vector by assigning the known 90-degree-clockwise
     * rotation of the [up] vector in order to prevent unneeded calculations.
     */
  right.x =  up.y;
  right.y = -up.x;

  return *this;
}

  /*
   * Scales the dimensions of the camera viewport rectangle by a factor of f
   * with respect the the camera's center. Note that the camera's aspect
   * ratio is preserved after the call to Zoom. A reference to the camera is
   * returned.
   */
Camera &Camera::Zoom(float scalar)
{
  width  *= scalar;
  height *= scalar;

  return *this;
}


/*############################################################################*/
/*####################### Non-member Helper Functions ########################*/
/*############################################################################*/

/*
 * Returns the transformation that maps the camera space coordinate system of
 * cam to the world space coordinate system.
 */
Affine CameraToWorld(const Camera &cam)
{
  return Affine(cam.Right(),
                cam.Up(),
                cam.Center());
}

/*
 * Returns the transformation that maps the world space coordinate system to
 * the camera space coordinate system of cam.
 */
Affine WorldToCamera(const Camera &cam)
{
  return Inverse(CameraToWorld(cam));
}

/*
 * Returns the transformation that maps the camera space coordinate system of
 * cam to normalized device coordinates based on the standard square. This
 * transformation maps the viewport in camera space (the axis aligned
 * rectangle centered at the origin with the same dimensions as the world
 * space viewport), to the square centered at the origin with width and
 * height of 2.
 */
Affine CameraToNDC(const Camera &cam)
{
  return Affine(Vector((2.0f / cam.Width()),                  0.0f),
                Vector(                0.0f, (2.0f / cam.Height())),
                Point());
}

/*
 * Returns the transformation that maps the normalized device coordinates
 * based on the standard square to the camera space coordinate system of cam.
 * This transformation maps the standard square to the camera space viewport
 * rectangle.
 */
Affine NDCToCamera(const Camera &cam)
{
  return Affine(Vector((cam.Width() / 2.0f),                  0.0f),
                Vector(                0.0f, (cam.Height() / 2.0f)),
                Point());
}

