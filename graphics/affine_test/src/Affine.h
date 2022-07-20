/* Affine.h                                  */
/* -- 2D point/vector/transformation package */
/* cs200 5/15                                */

#ifndef CS200_AFFINE_H
#define CS200_AFFINE_H

#include <cmath>
#include <cassert>

/*############################################################################*/
/*########################## Structure Definitions ###########################*/
/*############################################################################*/

/*!
******************************************************************************
  \struct Hcoords
  \brief
    Homogeneous three component vector (base class).

    Operations include:

    - Default constructor.
    - Non-default constructor.
    - Indexing a component for a reference.
    - Const indexing an component for a value.
    - Determining if two given floating point values are approximately equal.
*******************************************************************************/
struct Hcoords
{
  float x;
  float y;
  float w;

    /* Default constructor. Returns [0, 0, 0] (the zero vector). */
  Hcoords(void);
    /* Non-default constructor. Returns [X, Y, W]. */
  Hcoords(float X, float Y, float W);

    /*
     * Returns the i-th component of a homogeneous coordinate vector.
     * If i != 0, 1, 2, the result is undefined.
     */
  float &operator[] (int i)       { return *(&x + i); }
  float  operator[] (int i) const { return *(&x + i); }

    /*
     * Convenience function to compare two floating point numbers:
     * returns true if x and y are close enough to be considered equal.
     */
  static bool Near(float x, float y) { return (std::abs(x - y) < 1e-5f); }
};


/*!
******************************************************************************
  \struct Point : Hcoords
  \brief
    A two-dimensional point represented in homogeneous coordinates.

    Operations include:

    - Default constructor.
    - Non-default constructor.
    - Copy constructor (from base class).
    - Indexing a component for a reference.
    - Const indexing an component for a value.
    - Determining if two given floating point values are approximately equal.
*******************************************************************************/
struct Point : Hcoords
{
    /*
     * Default constructor.
     * Returns a point with components (0, 0); (i.e., the origin).
     */
  Point(void);
    /* Constructor to initialize the components of a point. */
  Point(float X, float Y);
    /*
     * Conversion operator to attempt to convert to a point.
     * This will fail, and the program will crash, if w != 1.
     */
  Point(const Hcoords &v) : Hcoords(v) { assert(Near(w, 1)); }
};


/*!
******************************************************************************
  \struct Vector : Hcoords
  \brief
    A two-dimensional vector represented in homogeneous coordinates.

    Operations include:

    - Default constructor.
    - Non-default constructor.
    - Copy constructor (from base class).
    - Indexing a component for a reference.
    - Const indexing an component for a value.
    - Determining if two given floating point values are approximately equal.
    - Normalizing the vector.
*******************************************************************************/
struct Vector : Hcoords
{
    /* Default constructor. Returns a vector with components (0, 0). */
  Vector(void);
    /*
     * Constructor to initialize the components of a vector. Returns a vector
     * with components <X, Y>.
     */
  Vector(float X, float Y);
    /*
     * Conversion operator to attempt to convert to a vector. This will fail,
     * and the program will crash, if w != 0.
     */
  Vector(const Hcoords &v) : Hcoords(v) { assert(Near(w, 0)); }

    /*
     * Normalize a vector. The components of the Vector structure are changed
     * to yield a unit vector pointing in the same direction. If the original
     * vector is the zero vector, the function should return false; otherwise,
     * it returns true.
     */
  bool Normalize(void);
};


/*!
******************************************************************************
  \struct Affine
  \brief
    A 3x3 homogeneous coordinate matrix.

    Operations include:

    - Default constructor.
    - Non-default constructor.
    - Indexing a component for a reference.
    - Const indexing an component for a value.
*******************************************************************************/
struct Affine
{
  Hcoords row[3];

    /*
     * Default constructor. Returns the affine transformation corresponding to
     * the trivial affine transformation whose linear part is the 0
     * transformation, and whose translation part is the 0 vector. Note that
     * the resulting matrix is not the 3 x 3 matrix whose entries are all
     * zeros; rather it is the same as the matrix for uniform scaling by 0 with
     * respect to the origin, H_0.
     */
  Affine(void);
    /*
     * Constructor to initialize an affine transformation. The quantities
     * Lx, Ly, D give the values of the columns of the transformation.
     */
  Affine(const Vector &Lx, const Vector &Ly, const Point &disp);

    /* Returns the i-th row of an affine transformation. */
        Hcoords &operator[] (int i)       { return row[i]; }
  const Hcoords &operator[] (int i) const { return row[i]; }
};

/*############################################################################*/
/*########################## Function Declarations ###########################*/
/*############################################################################*/

  /* Returns the sum (u + v) of two three-component vectors. */
Hcoords operator+ (const Hcoords &u, const Hcoords &v);
  /* Returns the difference (u − v) of two three-component vectors. */
Hcoords operator- (const Hcoords &u, const Hcoords &v);
  /* Returns the component-wise negation (-v) of a three-component vector */
Hcoords operator- (const Hcoords &v);
  /* Returns the product (k * v) of a r and a three-component vector. */
Hcoords operator* (float r, const Hcoords &v);
  /*
   * Returns the result (Av) of applying the affine transformation A to the
   * three-component vector v.
   */
Hcoords operator* (const Affine &A, const Hcoords &v);
  /*
   * Returns the composition A * B (matrix product) of the affine
   * transformations A and B.
   */
Affine  operator* (const Affine &A, const Affine &B);

  /* Returns the dot product ~u · ~v of two-dimensional vectors. */
float dot(const Vector &u, const Vector &v);
  /* Returns the length |v| of a two-dimensional vector. */
float abs(const Vector &v);

  /*
   * Returns the affine transformation Rt for rotation
   * by the angle [angle] (in radians) with respect to the origin.
   */
Affine Rot(float t);
  /* Returns the affine transformation T_v for translation by the vector v. */
Affine Trans(const Vector &v);
  /*
   * Returns the affine transformation (k * H) for uniform scaling by k with
   * respect to the origin.
   */
Affine Scale(float r);
  /*
   * Returns the affine transformation H((k * x), (n * y)) for inhomogeneous
   * scaling by factors r x and ry with respect to the origin.
   */
Affine Scale(float rx, float ry);

  /* Note that this function is NOT implemented in assignment #1: */
Affine Inverse(const Affine &A);


#endif

