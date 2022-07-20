/*!
*****************************************************************************
\file   MyMesh.h
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Project #2
\date   07/19/2015

\brief
  This file contains the definition of the MyMesh and SquareMesh classes.
*******************************************************************************/
#ifndef MYMESH_H
#define MYMESH_H

#include "Mesh.h"
#include "Affine.h"
#include <vector>
#include <algorithm>


/*############################################################################*/
/*############################################################################*/
/*############################ Base MyMesh Class #############################*/
/*############################################################################*/
/*############################################################################*/

/*!
******************************************************************************
  \class MyMesh : public Mesh

  \brief
    This class is a derived class of the base Mesh class with all virtual
    methods implemented. In addition to implementing the Mesh methods, this
    class adds some helper methods used to manipulate objects instantiated from
    this class as well as it acts as a base class for custom-defined meshs.
*******************************************************************************/
class MyMesh : public Mesh
{
/*############################################################################*/
/*##################### Private (Derived) Static Members #####################*/
/*############################################################################*/
private:
    /*
     * Each reference will refer to a corresponding static member of some
     * derived class. This set up allows for the MyMesh class to define the
     * behavior shared by all derived meshes (e.g. getters, accessors, etc.)
     * while allowing each derived class to define their own layout
     * (i.e. vertices/edges/faces) into static storage containers local
     * to that class. This effectively allows for inherited static members which
     * reduces the size of each object instantiated from the derived classes
     * while still allowing this base class to define how to interface with the
     * static members of the derived classes.
     */

    /* A reference to the std::vector<...> vertices of some derived mesh. */
  const std::vector<Point> &mrVertices;
    /* A reference to the std::vector<...> edges of some derived mesh. */
  const std::vector<Edge>  &mrEdges;
    /* A reference to the std::vector<...> faces of some derived mesh. */
  const std::vector<Face>  &mrFaces;
    /* A reference to the dimensions of the object-space of some derived mesh */
  const Vector             &mrDimensions;


/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/
public:

  /******************************* Constructors *******************************/
    /* Non-Default Constructor. */
  MyMesh(const std::vector<Point> &rVertices,
         const std::vector<Edge>  &rEdges,
         const std::vector<Face>  &rFaces,
         const Vector             &rDimensions,
               Point               center = Point(0.0f, 0.0f));
    /* Default Destructor. */
  virtual ~MyMesh(void);

  /********************************* Getters **********************************/
    /* Get the dimensions of the mesh. */
  Vector Dimensions(void);
    /* Get the center point of the mesh. */
  Point Center(void);
    /* Get the number of vertices in the mesh. */
  int VertexCount(void);
    /* Get the number of edges in of the mesh. */
  int EdgeCount(void);
    /* Get the number of faces in of the mesh. */
  int FaceCount(void);

  /******************************** Accessors *********************************/
    /* Access the i_th vertex of the mesh. */
  Point GetVertex(int i);
    /* Access the i_th edge of the mesh. */
  Edge GetEdge(int i);
    /* Access the i_th face of the mesh. */
  Face GetFace(int i);

  /********************************* Setters **********************************/
    /* Set the center point of the mesh. */
  void SetCenter(const Point &center);

  /********************************* Mutators *********************************/
    /* Transform the defined vertices of this mesh and store in [mTempVerts]. */
  void Transform(const Affine &transform);


/*############################################################################*/
/*############################# Private Members ##############################*/
/*############################################################################*/
private:

    /* The center point of some mesh's object-space. */
  Point  mCenter;
public:
    /* Store the temporary transformed vertices after a call to Transform(). */
  std::vector<Point> mTempVerts;
};



/*############################################################################*/
/*############################################################################*/
/*######################### Static Mesh Layout Data ##########################*/
/*############################################################################*/
/*############################################################################*/

struct PrimitiveCustomMesh
{
  static const int        mVertCount = 16;
  static const int        mEdgeCount = 14;
  static const int        mFaceCount = 16;
  static const Point      mVertices[mVertCount];
  static const Mesh::Edge mEdges[mEdgeCount];
  static const Mesh::Face mFaces[mFaceCount];
};

struct PrimitiveSquareMesh
{
  static const int        mVertCount = 4;
  static const int        mEdgeCount = 4;
  static const int        mFaceCount = 2;
  static const Point      mVertices[mVertCount];
  static const Mesh::Edge mEdges[mEdgeCount];
  static const Mesh::Face mFaces[mFaceCount];
};



/*############################################################################*/
/*############################################################################*/
/*########################### Derived Mesh Classes ###########################*/
/*############################################################################*/
/*############################################################################*/

/*!
******************************************************************************
  \class CustomMesh : public Mesh

  \brief
    This class is a derived Mesh that represents a basic square.
*******************************************************************************/
class CustomMesh : public MyMesh
{
/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/
public:

  /******************************* Constructors *******************************/
    /* Default Constructor. */
  CustomMesh(void);
    /* Default Destructor. */
  virtual ~CustomMesh(void) { ; }

/*############################################################################*/
/*########################## Private Static Members ##########################*/
/*############################################################################*/
private:
    /* A static std::vector<...> of the vertices that define this mesh. */
  static const std::vector<Point> mVertices;
    /* A static std::vector<...> of the edges that define this mesh. */
  static const std::vector<Edge>  mEdges;
    /* A static std::vector<...> of the faces that define this mesh. */
  static const std::vector<Face>  mFaces;
    /* The dimensions of this mesh's object-space. */
  static const Vector             mDimensions;
};


/*!
******************************************************************************
  \class SquareMesh : public Mesh

  \brief
    This class is a derived Mesh that represents a basic square.
*******************************************************************************/
class SquareMesh : public MyMesh
{
/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/
public:

  /******************************* Constructors *******************************/
    /* Default Constructor. */
  SquareMesh(void);
    /* Default Destructor. */
  virtual ~SquareMesh(void) { ; }

/*############################################################################*/
/*########################## Private Static Members ##########################*/
/*############################################################################*/
private:
    /* A static std::vector<...> of the vertices that define this mesh. */
  static const std::vector<Point> mVertices;
    /* A static std::vector<...> of the edges that define this mesh. */
  static const std::vector<Edge>  mEdges;
    /* A static std::vector<...> of the faces that define this mesh. */
  static const std::vector<Face>  mFaces;
    /* The dimensions of this mesh's object-space. */
  static const Vector             mDimensions;
};


#endif

