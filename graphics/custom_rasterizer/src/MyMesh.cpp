/*!
*****************************************************************************
\file   MyMesh.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Project #2
\date   07/19/2015

\brief
  This file contains the definitions for the MyMesh and SquareMesh classes'
  methods, as well as the layout of its mesh.
*******************************************************************************/
#include "MyMesh.h"
#include "Affine.h"



/*############################################################################*/
/*############################################################################*/
/*############################ Base MyMesh Class #############################*/
/*############################################################################*/
/*############################################################################*/


/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/

  /* Default Constructor. */
MyMesh::MyMesh(const std::vector<Point> &rVertices,
               const std::vector<Edge>  &rEdges,
               const std::vector<Face>  &rFaces,
               const Vector             &rDimensions,
                     Point               center)
  : Mesh(),
    mrVertices(rVertices),
    mrEdges(rEdges),
    mrFaces(rFaces),
    mrDimensions(rDimensions),
    mTempVerts(),
    mCenter(center) { ; }

  /* Default Destructor. */
MyMesh::~MyMesh(void) { ; }

  /* Get the dimensions of the mesh. */
Vector MyMesh::Dimensions(void) { return mrDimensions; }

  /* Get the center point of the mesh. */
Point MyMesh::Center(void) { return mCenter; }

  /* Get the number of vertices in the mesh. */
int MyMesh::VertexCount(void) { return mrVertices.size(); }

  /* Get the number of edges in of the mesh. */
int MyMesh::EdgeCount(void) { return mrEdges.size(); }

  /* Get the number of faces in of the mesh. */
int MyMesh::FaceCount(void) { return mrFaces.size(); }

  /* Access the i_th vertex of the mesh. */
Point MyMesh::GetVertex(int i) { return mrVertices[i]; }

  /* Access the i_th edge of the mesh. */
Mesh::Edge MyMesh::GetEdge(int i) { return mrEdges[i]; }

  /* Access the i_th face of the mesh. */
Mesh::Face MyMesh::GetFace(int i) { return mrFaces[i]; }

  /* set the center point of the mesh. */
void MyMesh::SetCenter(const Point &center) { mCenter = center; }

  /* Transform the defined vertices of this mesh and store in [mTempVerts]. */
void MyMesh::Transform(const Affine &transform)
{
  mTempVerts.clear();
  std::for_each(mrVertices.begin(), mrVertices.end(),
                [this, transform] (const Point &vertex)
                { mTempVerts.push_back(transform * vertex); } );
}



/*############################################################################*/
/*############################################################################*/
/*######################### Static Mesh Layout Data ##########################*/
/*############################################################################*/
/*############################################################################*/



/*############################################################################*/
/*######################## CustomMesh Primitive Data #########################*/
/*############################################################################*/

  /* Array of vertices used in this mesh. */
const Point PrimitiveCustomMesh::mVertices[mVertCount] =
                                                    { Point( 0.0f,  1.0f), //  0
                                                      Point( 2.0f,  1.0f), //  1
                                                      Point( 1.0f,  0.0f), //  2
                                                      Point( 0.0f,  3.0f), //  3
                                                      Point( 2.0f,  3.0f), //  4
                                                      Point( 0.0f,  5.0f), //  5
                                                      Point(-2.0f,  3.0f), //  6
                                                      Point(-2.0f,  1.0f), //  7
                                                      Point( 0.0f, -1.0f), //  8
                                                      Point( 2.0f, -1.0f), //  9
                                                      Point( 0.0f, -3.0f), // 10
                                                      Point( 2.0f, -3.0f), // 11
                                                      Point( 0.0f, -5.0f), // 12
                                                      Point(-2.0f, -3.0f), // 13
                                                      Point(-2.0f, -1.0f), // 14
                                                      Point(-1.0f,  0.0f), // 15
                                                    };

  /* Array of edges used in this mesh. */
const Mesh::Edge PrimitiveCustomMesh::mEdges[mEdgeCount] =
                                                     { Mesh::Edge( 3,  0), //  0
                                                       Mesh::Edge( 0,  9), //  1
                                                       Mesh::Edge( 9, 11), //  2
                                                       Mesh::Edge(11, 12), //  3
                                                       Mesh::Edge(12, 13), //  4
                                                       Mesh::Edge(13, 14), //  5
                                                       Mesh::Edge(14, 15), //  6
                                                       Mesh::Edge(10,  8), //  7
                                                       Mesh::Edge( 8,  7), //  8
                                                       Mesh::Edge( 7,  6), //  9
                                                       Mesh::Edge( 6,  5), // 10
                                                       Mesh::Edge( 5,  4), // 11
                                                       Mesh::Edge( 4,  1), // 12
                                                       Mesh::Edge( 1,  2), // 13
                                                     };

  /* Array of faces used in this mesh. */
const Mesh::Face PrimitiveCustomMesh::mFaces[mFaceCount] =
                                                 { Mesh::Face( 0,  1,  2), //  0
                                                   Mesh::Face( 0,  1,  3), //  1
                                                   Mesh::Face( 3,  1,  4), //  2
                                                   Mesh::Face( 3,  4,  5), //  3
                                                   Mesh::Face( 3,  5,  6), //  4
                                                   Mesh::Face( 3,  6,  0), //  5
                                                   Mesh::Face( 0,  6,  7), //  6
                                                   Mesh::Face( 0,  7,  8), //  7
                                                   Mesh::Face( 0,  8,  9), //  8
                                                   Mesh::Face( 9,  8, 10), //  9
                                                   Mesh::Face(10, 11,  9), // 10
                                                   Mesh::Face(10, 12, 11), // 11
                                                   Mesh::Face(10, 13, 12), // 12
                                                   Mesh::Face(10, 14, 13), // 13
                                                   Mesh::Face(10,  8, 14), // 14
                                                   Mesh::Face(14,  8, 15)  // 15
                                                 };

typedef PrimitiveCustomMesh CMesh;

  /* A static std::vector<...> of the vertices that define this mesh. */
const std::vector<Point> CustomMesh::mVertices(CMesh::mVertices,
                                               (CMesh::mVertices +
                                                CMesh::mVertCount));
  /* A static std::vector<...> of the edges that define this mesh. */
const std::vector<Mesh::Edge> CustomMesh::mEdges(CMesh::mEdges,
                                                 (CMesh::mEdges +
                                                  CMesh::mEdgeCount));
  /* A static std::vector<...> of the faces that define this mesh. */
const std::vector<Mesh::Face> CustomMesh::mFaces(CMesh::mFaces,
                                                 (CMesh::mFaces +
                                                  CMesh::mFaceCount));
  /* The dimensions of this mesh's object-space. */
const Vector CustomMesh::mDimensions(4.0f, 10.0f);


/*############################################################################*/
/*######################## SquareMesh Primitive Data #########################*/
/*############################################################################*/

  /* Array of vertices used in this mesh. */
const Point PrimitiveSquareMesh::mVertices[mVertCount] =
                                                    { Point( 0.5f,  0.5f), //  0
                                                      Point(-0.5f,  0.5f), //  1
                                                      Point(-0.5f, -0.5f), //  2
                                                      Point( 0.5f, -0.5f), //  3
                                                    };

  /* Array of edges used in this mesh. */
const Mesh::Edge PrimitiveSquareMesh::mEdges[mEdgeCount] =
                                                       { Mesh::Edge(0, 1), //  0
                                                         Mesh::Edge(1, 2), //  1
                                                         Mesh::Edge(2, 3), //  2
                                                         Mesh::Edge(3, 0), //  3
                                                       };

  /* Array of faces used in this mesh. */
const Mesh::Face PrimitiveSquareMesh::mFaces[mFaceCount] =
                                                 { Mesh::Face( 0,  1,  2), //  0
                                                   Mesh::Face( 0,  2,  3), //  1
                                                 };

typedef PrimitiveSquareMesh SMesh;

  /* A static std::vector<...> of the vertices that define this mesh. */
const std::vector<Point> SquareMesh::mVertices(SMesh::mVertices,
                                               (SMesh::mVertices +
                                                SMesh::mVertCount));
  /* A static std::vector<...> of the edges that define this mesh. */
const std::vector<Mesh::Edge> SquareMesh::mEdges(SMesh::mEdges,
                                                 (SMesh::mEdges +
                                                  SMesh::mEdgeCount));
  /* A static std::vector<...> of the faces that define this mesh. */
const std::vector<Mesh::Face> SquareMesh::mFaces(SMesh::mFaces,
                                                 (SMesh::mFaces +
                                                  SMesh::mFaceCount));
  /* The dimensions of this mesh's object-space. */
const Vector SquareMesh::mDimensions(1.0f, 1.0f);

/*############################################################################*/
/*############################################################################*/
/*########################### Derived Mesh Classes ###########################*/
/*############################################################################*/
/*############################################################################*/

CustomMesh::CustomMesh(void)
  : MyMesh(mVertices, mEdges, mFaces, mDimensions)
{
}

SquareMesh::SquareMesh(void)
  : MyMesh(mVertices, mEdges, mFaces, mDimensions)
{
}

