/*!
*****************************************************************************
\file   Affine.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #1
\date   05/13/2015

\brief
  This is the definition file for the basic implementation of the
  following structs:

    - Hcoords
    - Point
    - Vector
    - Affine

  as well as some overloaded operators and utility functions in order to use
  the above structs.


  - Hcoords::Hcoords(void)
    Default constructor. Returns [0, 0, 0] (the zero vector).

  - Hcoords::Hcoords(float X, float Y, float W)
    Non-default constructor. Returns [X, Y, W].

  - Point::Point(void)
    Default constructor.
    Returns a point with components (0, 0); (i.e., the origin).

  - Point::Point(float X, float Y)
    Constructor to initialize the components of a point.

  - Vector::Vector(void)
    Default constructor. Returns a vector with components (0, 0).

  - Vector::Vector(float X, float Y)
    Constructor to initialize the components of a vector. Returns a vector
    with components <X, Y>.

  - bool Vector::Normalize(void)
    Normalize a vector. If the original vector is the zero vector,
    the function should return false; otherwise, it returns true.

  - Affine::Affine(void)
    Default constructor. Returns the affine transformation corresponding to
    the trivial affine transformation whose linear part is the 0
    transformation, and whose translation part is the 0 vector.

  - Affine::Affine(const Vector &Lx, const Vector &Ly, const Point &disp)
    Constructor to initialize an affine transformation. The quantities
    Lx, Ly, disp give the values of the columns of the transformation.

  - Hcoords operator+ (const Hcoords &vec_U, const Hcoords &vec_V)
    Returns the sum (vec_U + vec_V) of two three-tuple vectors.

  - Hcoords operator- (const Hcoords &vec_U, const Hcoords &vec_V)
    Returns the difference (vec_U - vec_V) of two three-tuple vectors.

  - Hcoords operator- (const Hcoords &vec_V)
    Returns the component-wise negation (-vec_V) of a three-tuple vector.

  - Hcoords operator* (float scalar, const Hcoords &vec_V)
    Returns the product (k * v) of a scalar and a three-tuple vector.

  - Hcoords operator* (const Affine &aff_A, const Hcoords &vec_V)
    Returns the result (Av) of applying the affine transformation A to the
    three-tuple vector v.

  - Affine operator* (const Affine &aff_A, const Affine &aff_B)
    Returns the composition A * B (matrix product) of the affine
    transformations A and B.

  - float dot(const Vector &vec_U, const Vector &vec_V)
    Returns the dot product u * v of two-dimensional vectors.

  - float abs(const Vector &vec_V)
    Returns the length |v| of a two-dimensional vector.

  - Affine Rot(float angle)
    Returns the affine transformation Rt for rotation
    by the angle [angle] (in radians) with respect to the origin.

  - Affine Trans(const Vector &vec_V)
    Returns the affine transformation T_v for translation by the vector v.

  - Affine Scale(float scalar)
    Returns the affine transformation (k * H) for uniform scaling by k with
    respect to the origin.

  - Affine Scale(float scalar_x, float scalar_y)
    Returns the affine transformation H((k * x), (n * y)) for inhomogeneous
    scaling by factors r x and ry with respect to the origin.

*******************************************************************************/

  /* Hcoords, Point, Vector, Affine, <cmath>: std::sqrt, std::cos, std::sin */
#include "Affine.h"

/*!
******************************************************************************
  \fn Hcoords::Hcoords(void)

  \brief
    Default constructor.

  \return
    Returns [0, 0, 0] (the zero vector).
*******************************************************************************/
Hcoords::Hcoords(void)
  : x(0), y(0), w(0)
{
}

/*!
******************************************************************************
  \fn Hcoords::Hcoords(float X, float Y, float W)

  \brief
    Non-default constructor.

  \param X
    The x-component of this Hcoords object.

  \param Y
    The y-component of this Hcoords object.

  \param W
    The w-component of this Hcoords object.

  \return
    Returns [X, Y, W].
*******************************************************************************/
Hcoords::Hcoords(float X, float Y, float W)
  : x(X), y(Y), w(W)
{
}

/*!
******************************************************************************
  \fn Point::Point(void)

  \brief
    Default constructor.

  \return
    Returns a point with components (0, 0); (i.e., the origin).
*******************************************************************************/
Point::Point(void)
  : Hcoords(0.0f, 0.0f, 1.0f)
{
}

/*!
******************************************************************************
  \fn Point::Point(float X, float Y)

  \brief
    Non-default constructor to initialize the components of a point.

  \param X
    The x-component of this Point object.

  \param Y
    The y-component of this Point object.

  \return
    Returns a point with components (X, Y).
*******************************************************************************/
Point::Point(float X, float Y)
  : Hcoords(X, Y, 1.0f)
{
}

/*!
******************************************************************************
  \fn Vector::Vector(void)

  \brief
    Default constructor.

  \return
    Returns a vector with components (0, 0).
*******************************************************************************/
Vector::Vector(void)
  : Hcoords()
{
}

/*!
******************************************************************************
  \fn Vector::Vector(float X, float Y)

  \brief
    Constructor to initialize the components of a vector.

  \param X
    The x-component of this Vector object.

  \param Y
    The y-component of this Vector object.

  \return
    Returns a vector with components <X, Y>.
*******************************************************************************/
Vector::Vector(float X, float Y)
  : Hcoords(X, Y, 0.0f)
{
}

/*!
******************************************************************************
  \fn bool Vector::Normalize(void)

  \brief
    Normalize a vector. The components of the Vector structure are changed
    to yield a unit vector pointing in the same direction.

  \return
    If the original vector is the zero vector,
    the function should return false; otherwise, it returns true.
*******************************************************************************/
bool Vector::Normalize(void)
{
    /* If we are the zero-vector, just return false. */
  if ((x == 0) && (y == 0))
// COMMENT:
// -- In general, floating point numbers should not be compared for
//    exact equality.  Instead, check for approximate equality or
//    inequality.  You can use the 'Hcoords::Near' function for this:
//        if (Hcoords::Near(x,0) && Hcoords::Near(y,0))
    return false;

    /* Calculate the unit scalar once for multiple use. */
  float unitScalar = std::sqrt((x * x) + (y * y));

  x /= unitScalar;
  y /= unitScalar;

  return true;
}

/*!
******************************************************************************
  \fn Affine::Affine(void)

  \brief
    Default constructor.  Note that the resulting matrix is not the 3 x 3
    matrix whose entries are all zeros; rather it is the same as the matrix for
    uniform scaling by 0 with respect to the origin, H_0.

  \return
    Returns the affine transformation corresponding to the trivial affine
    transformation whose linear part is the 0 transformation, and whose
    translation part is the 0 vector.
*******************************************************************************/
Affine::Affine(void)
{
    /*
     * [row] is initialized to a 3 x 3 0-matrix,
     * so we just need to alter the homogeneous point value to be as it should.
     */
  row[2][2] = 1;
}

/*!
******************************************************************************
  \fn Affine::Affine(const Vector &Lx, const Vector &Ly, const Point &disp)

  \brief
    Constructor to initialize an affine transformation. The quantities
    Lx, Ly, disp give the values of the columns of the transformation.

  \param Lx
    The first column vector of this Affine matrix.

  \param Ly
    The second column vector of this Affine matrix.

  \param disp
    The third column vector of this Affine matrix.

  \return
    Returns the Affine matrix whose column vectors are initialized
    to Lx, Ly, and disp.
*******************************************************************************/
Affine::Affine(const Vector &Lx, const Vector &Ly, const Point &disp)
{
  row[0] = Hcoords(Lx.x, Ly.x, disp.x);
  row[1] = Hcoords(Lx.y, Ly.y, disp.y);
  row[2] = Hcoords(Lx.w, Ly.w, disp.w);
}

/*!
******************************************************************************
  \fn operator+ (const Hcoords &vec_U, const Hcoords &vec_V)

  \brief
    Evaluates the sum of two three-tuple vectors.

  \param vec_U
    The first addend of the operation.

  \param vec_V
    The second addend of the operation.

  \return
    Returns the sum (vec_U + vec_V) of two three-tuple vectors.
*******************************************************************************/
Hcoords operator+ (const Hcoords &vec_U, const Hcoords &vec_V)
{
  return Hcoords((vec_U.x + vec_V.x),
                 (vec_U.y + vec_V.y),
                 (vec_U.w + vec_V.w));
}

/*!
******************************************************************************
  \fn operator- (const Hcoords &vec_U, const Hcoords &vec_V)

  \brief
    Evaluates the difference of two three-tuple vectors.

  \param vec_U
    The minuend of the operation.

  \param vec_V
    The subtrahend of the operation.

  \return
    Returns the difference (vec_U - vec_V) of two three-tuple vectors.
*******************************************************************************/
Hcoords operator- (const Hcoords &vec_U, const Hcoords &vec_V)
{
  return Hcoords((vec_U.x - vec_V.x),
                 (vec_U.y - vec_V.y),
                 (vec_U.w - vec_V.w));
}

/*!
******************************************************************************
  \fn operator- (const Hcoords &vec_V)

  \brief
    Negates a three-tuple vector.

  \param vec_V
    The vector to be negated.

  \return
    Returns the component-wise negation (-vec_V) of a three-tuple vector.
*******************************************************************************/
Hcoords operator- (const Hcoords &vec_V)
{
  return Hcoords(-vec_V.x, -vec_V.y, -vec_V.w);
}

/*!
******************************************************************************
  \fn operator* (float scalar, const Hcoords &vec_V)

  \brief
    Evaluates the product of a scalar and a three-tuple vector.

  \param scalar
    The magnitude by which [vec_V] is scaled.

  \param vec_V
    The vector to scale by magnitued [scalar].

  \return
    Returns the product ([scalar] * [vec_V]) of a scalar
    and a three-tuple vector.
*******************************************************************************/
Hcoords operator* (float scalar, const Hcoords &vec_V)
{
  return Hcoords((scalar * vec_V.x),
                 (scalar * vec_V.y),
                 (scalar * vec_V.w));
}

/*!
******************************************************************************
  \fn Hcoords operator* (const Affine &aff_A, const Hcoords &vec_V)

  \brief
    Performs an affine transformation on [vec_V] given by [aff_A].

  \param aff_A
    The affine transformation matrix by which [vec_V] is transformed.

  \param vec_V
    The subtrahend of the operation.

  \return
    Returns the result (Av) of applying the affine transformation [aff_A] to the
    three-tuple vector [vec_V].
*******************************************************************************/
Hcoords operator* (const Affine &aff_A, const Hcoords &vec_V)
{
  Hcoords result;

  for (int i = 0; i < 3; ++i)
  {
      /* Use a reference to the current row to prevent repitious indexing. */
    const Hcoords &row_ref = aff_A[i];

    result[i] = ((row_ref.x * vec_V.x) +
                 (row_ref.y * vec_V.y) +
                 (row_ref.w * vec_V.w));
  }

  return result;
}

/*!
******************************************************************************
  \fn Affine operator* (const Affine &aff_A, const Affine &aff_B)

  \brief
    Calculates a single matrix that represents the transformation of [aff_B]
    followed by [aff_A].

  \param aff_A
    The second matrix to be applied during the composition transformation.

  \param aff_B
    The first matrix to be applied during the composition transformation.

  \return
    Returns the composition A * B (matrix product) of the affine
    transformations A and B.
*******************************************************************************/
Affine operator* (const Affine &aff_A, const Affine &aff_B)
{
    /* Use references to each row in [aff_B] to prevent repitious indexing. */
  const Hcoords &B_row_0 = aff_B[0];
  const Hcoords &B_row_1 = aff_B[1];
  const Hcoords &B_row_2 = aff_B[2];

  Affine result;

  for (int i = 0; i < 3; ++i)
  {
      /* Reference the current row of operation to prevent repitious indexing */
    const Hcoords &A_row_ref      = aff_A[i];
          Hcoords &result_row_ref = result[i];

    for (int j = 0; j < 3; ++j)
      result_row_ref[j] = ((A_row_ref.x * B_row_0[j]) +
                           (A_row_ref.y * B_row_1[j]) +
                           (A_row_ref.w * B_row_2[j]));
  }
// COMMENT:
// -- You can use double subscripting to make the code simpler:
//        for (int i=0; i < 3; ++i)
//          for (int j=0; j < 3; ++j) {
//            result[i][j] = 0;
//            for (int k=0; k < 3; ++k)
//              result[i][j] += A[i][k] * B[k][j];
//          }


  return result;
}

/*!
******************************************************************************
  \fn dot(const Vector &vec_U, const Vector &vec_V)

  \brief
    Calculates the dot product of two-dimensional vectors.

  \param vec_U
    The left-hand side vector of the dot product.

  \param vec_V
    The right-hand side vector of the dot product.

  \return
    Returns the dot product (vec_U * vec_V) of two-dimensional vectors.
*******************************************************************************/
float dot(const Vector &vec_U, const Vector &vec_V)
{
  return ((vec_U.x * vec_V.x) + (vec_U.y * vec_V.y));
}

/*!
******************************************************************************
  \fn abs(const Vector &vec_V)

  \brief
    Calculates the length of a two-dimensional vector.

  \param vec_V
    The vector to calculate the length of.

  \return
    Returns the length |v| of a two-dimensional vector.
*******************************************************************************/
float abs(const Vector &vec_V)
{
  return std::sqrt((vec_V.x * vec_V.x) + (vec_V.y * vec_V.y));
}

/*!
******************************************************************************
  \fn Rot(float angle)

  \brief
    Calculates the affine transformation matrix for rotating
    by [angle] (in radians) with respect to the origin.

  \param angle
    The angle (in radians) of rotation.

  \return
    Returns the affine transformation matrix for rotation by the angle [angle]
    (in radians) with respect to the origin.
*******************************************************************************/
Affine Rot(float angle)
{
    /* Prevent calculating each twice. */
  float cos = std::cos(angle);
  float sin = std::sin(angle);

  return Affine(Vector( cos, sin),
                Vector(-sin, cos),
                Point());
}

/*!
******************************************************************************
  \fn Trans(const Vector &vec_V)

  \brief
    Calculates the affine transformation matrix for translating
    by the vector [vec_V].

  \param vec_V
    The vector by which the translation is done.

  \return
    Returns the affine transformation matrix for translation
    by the vector [vec_V].
*******************************************************************************/
Affine Trans(const Vector &vec_V)
{
  return Affine(Vector(1.0f, 0.0f),
                Vector(0.0f, 1.0f),
                Point(vec_V.x, vec_V.y));
}

/*!
******************************************************************************
  \fn Scale(float scalar)

  \brief
    Calculates the affine transformation matrix for uniform scaling by the
    magnitude of [scalar] with respect to the origin.

  \param scalar
    The magnitude by which the scaling is done.

  \return
    Returns the affine transformation matrix for uniform scaling by [scalar]
    with respect to the origin.
*******************************************************************************/
Affine Scale(float scalar)
{
  return Affine(Vector(scalar, 0.0f), Vector(0.0f, scalar), Point());
}

/*!
******************************************************************************
  \fn Scale(float scalar_x, float scalar_y)

  \brief
    Calculates the affine transformation matrix for inhomogeneous scaling by
    factors [scalar_x] and [scalar_y] with respect to the origin.

  \param scalar_x
    The magnitude by which the scaling is done along the x-axis.

  \param scalar_y
    The magnitude by which the scaling is done along the y-axis.

  \return
    Returns the affine transformation matrix for inhomogeneous scaling by
    factors [scalar_x] and [scalar_y] with respect to the origin.
*******************************************************************************/
Affine Scale(float scalar_x, float scalar_y)
{
  return Affine(Vector(scalar_x, 0.0f), Vector(0.0f, scalar_y), Point());
}
