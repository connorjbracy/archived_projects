/*!
*****************************************************************************
\file   DrawFrame.cpp
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Project #2
\date   07/19/2015

\brief
  This file contains the implementation for rasterizing and drawing objects to
  a pixel buffer in software.
*******************************************************************************/
#include "Affine.h"
#include "Camera.h"
#include "Clip.h"
#include "DrawFrame.h"
#include "MyMesh.h"
#include "RasterUtilities.h"
#include "Raster.h"
#include <ctime>


/*############################################################################*/
/*########################## Local Helper Functions ##########################*/
/*############################################################################*/

  /* Generate a random float in the range [a, b]. */
float frand(float a = 0.0f, float b = 1.0f)
{
  return (a +
          ((b - a) *
            (static_cast<float>(rand()) /
              static_cast<float>(RAND_MAX))));
}

  /* Generate a random int in the range [a, b]. */
int irand(float a = 0.0f, float b = 1.0f)
{ return static_cast<int>(frand(a, b)); }


/*############################################################################*/
/*############################# Global Constants #############################*/
/*############################################################################*/

  /* Pre-calculate pi. */
static const float PI       = (4.0f * atan(1.0f));
  /* Base-line rotation rate for objects. */
static const float gRotRate = (2 * (PI / frand(5.0f,  8.0f)));
  /* World origin. */
static const Point gOrigin(0.0f, 0.0f);
  /* Number of dynamic objects to generate. */
static const int gDynamicObjCount = 50;

/*############################################################################*/
/*############################ Class Definitions #############################*/
/*############################################################################*/

  /* Short-hand typedef to make code cleaner. */
typedef Raster::byte Cval;

/*!
******************************************************************************
  \struct Color

  \brief
    Small wrapper class with default constructor for easily managing an RGB
    value.
*******************************************************************************/
struct Color
{
  Cval red, grn, blu;

  Color(Cval r = 0, Cval g = 0, Cval b = 0) : red(r), grn(g), blu(b) { ; }
};

/*!
******************************************************************************
  \struct Object

  \brief
    Pure virtual base struct used for managing MyMesh objects as
    world-space objects.
*******************************************************************************/
struct Object
{
    /* Store the color of the faces for this object. */
  Color mFaceColor;
    /* Default constructor. */
  Object(Color faceColor = Color(255, 255, 255)) : mFaceColor(faceColor) { ; }
    /* Virtual destructor. */
  virtual ~Object(void) { ; }
    /* Pure virtual transform method used to update the object. */
  virtual void Transform(const Affine &worldToDev, float curnTime) = 0;
    /*
     * Pure virtual method used to get a reference to the derived classes'
     * mesh in order to rasterize it.
     */
  virtual MyMesh &GetMesh(void) = 0;
};

/*!
******************************************************************************
  \struct Tile : public Object

  \brief
    Derived Object specialized to be static background tiles.
*******************************************************************************/
struct Tile : public Object
{
    /* Represent each Tile by a SquareMesh. */
  SquareMesh mMesh;
    /* Precalculate and store the unchanging object-to-world transform. */
  Affine     mOjbectToWorld;
    /* Default constructor. */
  Tile(const Vector &pos       = Vector(0.0f, 0.0f),
       const Color  &faceColor = Color(255, 255, 255))
    : Object(faceColor),
      mMesh(),
      mOjbectToWorld(Trans(pos) * Trans(gOrigin - mMesh.Center()))
  { ; }
    /* Virtual destructor. */
  virtual ~Tile(void) { ; }
    /*
     * Transform the baseline vertices according to this Tile's
     * position in space with relation to the current viewing camera.
     */
  virtual void Transform(const Affine &worldToDev, float)
  { mMesh.Transform(worldToDev * mOjbectToWorld); }
    /* Return a reference to this Tile's mesh. */
  virtual MyMesh &GetMesh(void) { return mMesh; }
};

/*!
******************************************************************************
  \struct DynamicObj : Object

  \brief
    Derived Object specialized for rotating objects displaced from the
    origin made up of a CustomMesh.
*******************************************************************************/
struct DynamicObj : public Object
{
    /* Represent each DynamicObj by a CustomMesh. */
  CustomMesh mMesh;
    /*
     * This object will need to rotate so store each constant transformation
     * on either side of the rotation transformation in the transformation
     * pipeline separately.
     */
  Affine     mObjectToOrigin;
  Affine     mOriginToPosition;
  Vector     mPosition;
  Vector     mVelocity;

    /* Store the rate of rotation for this object. */
  float      mRotationRate;
    /* Default constructor. */
  DynamicObj(const Vector &pos       = Vector(0.0f, 0.0f),
                   float   scale     = 1.0f,
                   float   rotRate   = 0.0f,
             const Vector &velocity  = Vector(0.0f, 0.0f),
             const Color  &faceColor = Color(255, 255, 255))
    : Object(faceColor),
      mMesh(),
      mObjectToOrigin(Scale(scale) * Trans(gOrigin - mMesh.Center())),
      mOriginToPosition(Trans(Vector(pos.x, pos.y))),
      mPosition(pos),
      mVelocity(velocity),
      mRotationRate(rotRate)
  { ; }
    /* Virtual destructor. */
  virtual ~DynamicObj(void) { ; }

    /*
     * Transform the baseline vertices according to this DynamicObj's
     * position in space with relation to the current viewing camera
     * while updating its current rotation.
     */
  virtual void Transform(const Affine &worldToDev, float curnTime)
  {
    mPosition = (mVelocity + mPosition);

    mOriginToPosition = Trans(Vector(mPosition.x, mPosition.y));

    mMesh.Transform(worldToDev                        *
                    mOriginToPosition                 *
                    Rot(mRotationRate *
                        static_cast<float>(curnTime)) *
                    mObjectToOrigin);
  }
    /* Return a reference to this DynamicObj's mesh. */
  virtual MyMesh &GetMesh(void) { return mMesh; }
};

/*!
******************************************************************************
  \struct CamObj : public Object

  \brief
    Derived Object specialized to be the SquareMesh representing the dynamic
    camera.
*******************************************************************************/
struct CamObj : public Object
{
    /* Represent each DynamicObj by a SquareMesh. */
  SquareMesh mMesh;
    /*
     * This object will need to rotate so store each constant transformation
     * on either side of the rotation transformation in the transformation
     * pipeline separately.
     */
  Affine     mObjectToOrigin;
  Affine     mOriginToWorld;
    /* Store the rate of rotation for this object. */
  float      mRotationRate;
    /* Default constructor. */
  CamObj(const Vector &pos       = Vector(0.0f, 0.0f),
         const Vector &scale     = Vector(1.0f, 1.0f),
               float   rotRate   = 0.0f,
         const Color  &faceColor = Color(255, 255, 255))
    : Object(faceColor),
      mMesh(),
      mObjectToOrigin(Scale(scale.x, scale.y) * Trans(gOrigin - mMesh.Center())),
      mOriginToWorld(Trans(Vector(pos.x, pos.y))),
      mRotationRate(rotRate)
  { ; }
    /* Virtual destructor. */
  virtual ~CamObj(void) { ; }
    /*
     * Initialize the scale of this object according to the given
     * scaling which corresponds to the dimensions of the dynamic camera.
     */
  void InitScale(const Vector &scale)
  { mObjectToOrigin = (Scale(scale.x, scale.y) * mObjectToOrigin); }
    /*
     * Update the the transformation matrices responsible for transforming
     * the SquareMesh to the dynamic camera's current position and orientation.
     */
  void Update(const Point &newCenter, const Affine &rotate)
  {
      /*
       * Update the object's orientation separate from its world position so
       * the update to the position does not affect subsequent transformations.
       */
    mObjectToOrigin = (rotate * mObjectToOrigin);
    mOriginToWorld  = (Trans(Vector(newCenter.x, newCenter.y)) * mObjectToOrigin);
  }
    /*
     * Transform the baseline vertices according to the updated transformation
     * matrix according to the dynamic camera's current position and orientation.
     */
  virtual void Transform(const Affine &worldToDev, float curnTime)
  { mMesh.Transform(worldToDev * mOriginToWorld); }
    /* Return a reference to this SquareMesh's mesh. */
  virtual MyMesh &GetMesh(void) { return mMesh; }
};

/*!
******************************************************************************
  \class CamManager

  \brief
    Manages both the static and dynamic camera use to view the scene as well as
    the mesh used to represent the dynamic camera when viewing through
    the static camera.
*******************************************************************************/
class CamManager
{
/*############################################################################*/
/*############################# Private Defines ##############################*/
/*############################################################################*/
private:

  /*############################### Constants ################################*/
  static const int   scmCamCount   = 2; /* Define the number of cameras.       */
  static const float scmStaticSize;     /* The dimension of the static camera. */
  static const float scmDynamicSize;    /* The dimension of the dynamic camera */

/*############################################################################*/
/*############################## Public Defines ##############################*/
/*############################################################################*/
public:

  /*############################### Constants ################################*/
  static const int scmStaticCam  = 0; /* The index of the static camera.     */
  static const int scmDynamicCam = 1; /* The index of the dynamic camera.    */


/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/
public:

  /*######################## Constructor/Initializers ########################*/
    /* Default constructor. */
  CamManager(void) { ; }
    /* Late "contstructor" (used to initialize staticly declared CamManagers). */
  void Init(int width, int height, float rotRate = frand((0.5f * gRotRate),
                                                         (1.5f * gRotRate)))
  {
      /* Store the rate of rotation for the dynamic camera. */
    mRotRate = rotRate;
      /* Store the viewport dimensions. */
    mScreenWidth  = width;
    mScreenHeight = height;
      /* Initialize both cameras and the path of the dynamic camera. */
    InitCameras();
      /* Initialize the Clip object that will clip objects to the screen. */
    InitClip();
  }
    /* Initialize both cameras and the path of the dynamic camera. */
  void InitCameras(void)
  {
      /* Calculate the aspect ratio. */
    float aspectRatio = (static_cast<float>(mScreenWidth) /
                         static_cast<float>(mScreenHeight));
      /* Precaluclate the scaled width of each camera. */
    float staticCamWidth  = (aspectRatio * scmStaticSize);
    float dynamicCamWidth = (aspectRatio * scmDynamicSize);

      /* Initialize both cameras according to the screen dimensions. */
    mCameras[scmStaticCam] = Camera(Point(0.0f, 0.0f),
                                    Vector(0.0f, 1.0f),
                                    staticCamWidth,
                                    scmStaticSize);
    mCameras[scmDynamicCam] = Camera(Point (0.0f, 0.0f),
                                     Vector(0.0f, 1.0f),
                                     dynamicCamWidth,
                                     scmDynamicSize);

      /* Initialize the mesh representing the dynamic camera. */
    mDynamicCamSquare.InitScale(Vector(dynamicCamWidth, scmDynamicSize));
      /*
       * Scale the static camera width by half to contain
       * the path inside the static camera's viewport.
       */
    staticCamWidth *= 0.5f;
      /*
       * Initialize the amplitude and angular frequency of
       * the dynamic camera's path.
       */
    mCamPathAmp     = Vector(frand(0.0f, staticCamWidth),
                             frand(0.0f, staticCamWidth));
    mCamPathAngFreq = Vector(frand(0.25f, 0.75f), frand(0.25f, 0.75f));
  }
    /* Initialize the Clip object that will clip objects to the screen. */
  void InitClip(void)
  {
      /*
       * Pre-calculate the scaled screen dimensions used in the next few
       * steps to prevent redundant calculations and improve readibility.
       */
    float borderedWidth  = (mScreenWidth  - 1.0f);
    float borderedHeight = (mScreenHeight - 1.0f);
    float scaleWidth     = (0.5f * (mScreenWidth  - 1.0f));
    float scaleHeight    = (0.5f * (mScreenHeight - 1.0f));

      /*
       * Calculate the Affine transformation from
       * normalized-device-coordinates to our device coordinates.
       */
    mNDC_to_Dev = Trans(Vector(scaleWidth, scaleHeight)) *
                  Scale(scaleWidth, scaleHeight);
      /* Create a temp array of vertices representing our viewport. */
    Point screenVerts[4] = { Point(         0.0f,           0.0f),
                             Point(         0.0f, borderedHeight),
                             Point(borderedWidth, borderedHeight),
                             Point(borderedWidth,           0.0f)
                           };
      /*
       * Instantiate our clip object that we will use
       * to clip objects to the viewport.
       * */
    mClipToScreen = Clip(4, screenVerts);
  }

  /*################################ Getters #################################*/
    /*
     * Get the World-to-Device transformation matrix corresponding
     * to the current camera.
     */
  Affine  GetWorldToDev(int camNum) { return (mNDC_to_Dev                   *
                                              CameraToNDC(mCameras[camNum]) *
                                              WorldToCamera(mCameras[camNum])); }
    /* Get the width of the screen's dimensions. */
  int   GetScreenW(void)    const { return mScreenWidth;  }
    /* Get the height of the screen's dimensions. */
  int   GetScreenH(void)    const { return mScreenHeight; }
    /* Get the height of the static cam's dimensions. */
  float GetStaticCamH(void) const { return scmStaticSize; }
    /* Get the height of the static cam's dimensions. */
  float GetStaticCamW(void) const { return scmStaticSize; }
    /* Get a reference to the CamObject in order to rasterize it. */
  Object &GetCamObj(void)         { return mDynamicCamSquare; }

  /*################################ Mutators ################################*/
    /*
     * Update the camera's position and orientation according to its randomly
     * defined path, then update the mesh representing it accordingly.
     */
  void Update(float curnTime, float dt)
  {
      /* Get a reference to the dynamic camera. */
    const Camera &dynamicCam = mCameras[scmDynamicCam];
      /*
       * Update the center of the camera according to the passage of
       * time and its defined Lissajous curve.
       */
    Point newCenter((mCamPathAmp.x * std::sin(2 * PI * mCamPathAngFreq.x * curnTime)),
                    (mCamPathAmp.y * std::sin(2 * PI * mCamPathAngFreq.y * curnTime)));
      /*
       * Calculate the current rotation matrix of the camera
       * according to the passage of time.
       */
    Affine rotate = Rot(mRotRate * dt);
      /* Create a new Camera object reflecting the changes to the dynamic camera. */
    mCameras[scmDynamicCam] = Camera(newCenter,
                                     (rotate * dynamicCam.Up()),
                                     dynamicCam.Width(),
                                     dynamicCam.Height());
      /* Update the square representing the dynamic camera accordingly. */
    mDynamicCamSquare.Update(dynamicCam.Center(), rotate);
  }

  /*################################ Helpers #################################*/
    /* Clip a triangle to the screen. */
  bool ClipToScreen(std::vector<Point> &verts) { return mClipToScreen(verts);      }
    /* Clip a line segment to the screen. */
  bool ClipToScreen(Point &pt_P, Point &pt_Q)  { return mClipToScreen(pt_P, pt_Q); }

/*############################################################################*/
/*############################# Private Members ##############################*/
/*############################################################################*/
private:
    /* Store the screen dimensions. */
  int    mScreenWidth;
  int    mScreenHeight;
    /* Store both the static and dynamic camera. */
  Camera mCameras[scmCamCount];
    /*
     * Store a CamObj that will represent the dynamic camera
     * when viewing through the static camera.
     */
  CamObj mDynamicCamSquare;
    /* Store the x/y-components of the dynamic camera's path amplitude. */
  Vector mCamPathAmp;
    /* Store the x/y-components of the dynamic camera's path angular frequency. */
  Vector mCamPathAngFreq;
    /* Store the rate of rotation for the dynamic camera. */
  float  mRotRate;
    /* Store the Clip object that will clip objects to the screen. */
  Clip   mClipToScreen;
    /*
     * Store the constant transformation between normalized-device-coordinates
     * and our device coordinates.
     */
  Affine mNDC_to_Dev;
};
  /* Initialize the dimensions of both cameras. */
const float CamManager::scmStaticSize   = 10.0f;
const float CamManager::scmDynamicSize  =  5.0f;


/*############################################################################*/
/*######################## Local Function Prototypes #########################*/
/*############################################################################*/

static void Rasterize  (Raster &raster, Object &object);
static void RasterFaces(Raster &raster, Object &object);
static void RasterEdges(Raster &raster, Object &object, Color color = Color());


/*############################################################################*/
/*############################### Global Data ################################*/
/*############################################################################*/

  /* The two colors to use for the background tiles. */
static const Color mTileColors[2] = { Color(255, 127,   0),
                                      Color(  0, 127, 255)
                                    };
  /* Global CamManager to manage both the static and dynamic cameras. */
static CamManager  gCameras;
  /* std::vector<...> to manage the background tiles. */
static std::vector<Tile>       gBckGrndTiles;
  /* std::vector<...> to manage the dynamic objects. */
static std::vector<DynamicObj> gDynamicObjects;
  /* Store the tentative time elapsed since the start of the program. */
float gCurrentTime = 0.0;


/*############################################################################*/
/*######################### Function Implementation ##########################*/
/*############################################################################*/

/*!
******************************************************************************
  \fn void InitFrame(int width, int height)

  \brief
    Initialize the frame buffer according to the given intial dimensions

  \param width
    The initial width of the frame buffer.

  \param height
    The initial height of the frame buffer.
*******************************************************************************/
void InitFrame(int width, int height)
{
    /* Seed the random number generator. */
  srand(static_cast<unsigned int>(time(NULL)));
    /* Initialize the cameras according to the screen dimensions. */
  gCameras.Init(width, height);

    /*
     * Calculate the dimensions of the background tile grid by.
     * Use (1.5x Screen Width) to ensure the tiles cover all of the
     * static camera's viewport.(the tiles will be in a square grid,
     * regardless of the potential rectangular shape of the screen).
     */
  int count = static_cast<int>(gCameras.GetStaticCamW() * 1.5f);
    /*
     * Set the start x-position of the tiles on each row.
     * In our device coordinates, the origin is in the center of the
     * screen so start to start all the way at the left, start at the
     * negative of half the screen's width. To ensure that all of the
     * static camera's background is covered, 3/4 is used instead of 1/2
     */
  const float xStartPos = -(gCameras.GetStaticCamW() * 0.75f);
    /*
     * Set the starting y-position for the background grid based on
     * the camera height. As with the rows, start at half of the
     * screen's height due to the origin being in the center of the screen.
     */
  float yPos = (gCameras.GetStaticCamW() * 0.5f);

    /* Loop through each row of background tiles. */
  for (int iRow = 0, iColor = 0; iRow < count; ++iRow)
  {
      /* Set the start position for the start of the row. */
    float xPos = xStartPos;
      /* Loop through each column in this row and create the checkered tiles. */
    for (int iCol = 0; iCol < count; xPos += 1.0f, ++iColor, ++iCol)
      gBckGrndTiles.push_back(Tile(Vector(xPos, yPos),
                                   mTileColors[iColor % 2]));
      /* Move to the next row. */
    yPos -= 1.0f;
      /* Ensure that each row starts on the opposite color of the previous. */
    iColor += ((count + 1) % 2);
  }

    /* Get the screens (min/max)imum x-values */
  float screenWidth = (gCameras.GetStaticCamW() * 0.5f);
    /*
     * Generate the dynamic objects at random positions
     * (contained within the screen) and random sizes/rotation rates.
     */
  for (int i = 0; i < gDynamicObjCount; ++i)
    gDynamicObjects.push_back(DynamicObj(Vector(frand(-screenWidth, screenWidth),
                                                frand(-screenWidth, screenWidth)),
                                         frand(0.10f, 0.25f),
                                         (PI / frand(2.0f, 10.0f)),
                                         Vector(frand(-0.0125f, 0.0125f), frand(-0.0125f, 0.0125f)),
                                         Color(irand(0.0f, 255.0f),
                                               irand(0.0f, 255.0f),
                                               irand(0.0f, 255.0f))));
  
//    /* Default constructor. */
//  DynamicObj(const Vector &pos       = Vector(0.0f, 0.0f),
//                   float   scale     = 1.0f,
//                   float   rotRate   = 0.0f,
//             const Vector &velocity  = Vector(0.0f, 0.0f),
//             const Color  &faceColor = Color(255, 255, 255))
}

/*!
******************************************************************************
  \fn void DrawFrame(Raster &raster, double dt, int camNum)

  \brief
    Draw the current frame to the given Raster object's frame buffer.

  \param raster
    The Raster object that the frame will be drawn to.

  \param dt
    The time passed (in seconds) since the last frame.

  \param camNum
    The camera ID number for the camera that is currently being viewed.
*******************************************************************************/
void DrawFrame(Raster &raster, double dt, int camNum)
{
    /* Set background to white. */
  raster.SetColor(255, 255, 255);
  FillRect(raster, 0, 0, gCameras.GetScreenW(), gCameras.GetScreenH());
    /* Get the world-to-device transform for the current camera. */
  Affine worldToDev = gCameras.GetWorldToDev(camNum);
    /* Loop through each tile in the background and draw them to the pixel buffer. */
  std::for_each(gBckGrndTiles.begin(), gBckGrndTiles.end(),
                [&raster, worldToDev] (Tile &tile)
                { tile.Transform(worldToDev, gCurrentTime); Rasterize(raster, tile); } );

    /* Loop through each tile in the background and draw them to the pixel buffer. */
  std::for_each(gDynamicObjects.begin(), gDynamicObjects.end(),
                [&raster, worldToDev] (DynamicObj &tile)
                { tile.Transform(worldToDev, gCurrentTime); Rasterize(raster, tile); } );

    /* Update the cameras. */
  gCameras.Update(gCurrentTime, static_cast<float>(dt));
    /*
     * If we are currently viewing through the static camera,
     * draw the dynamic camera's square representation.
     */
  if (camNum == CamManager::scmStaticCam)
  {
      /* Get a reference to the CamObj representing the dynamic camera. */
    Object &camObj = gCameras.GetCamObj();
      /* Transform it's vertices. */
    camObj.Transform(worldToDev, gCurrentTime);
      /*
       * Draw only the edge so we can still see the objects
       * that the dynamic camera would be viewing.
       */
    RasterEdges(raster, camObj, Color(155, 25, 25));
  }
    /* Update the current time. */
  gCurrentTime += static_cast<float>(dt);
}

/*!
******************************************************************************
  \fn void Rasterize(Raster &raster, const Object &object)

  \brief
    Given an Raster object and an Object object, call the helper functions
    used to rasterize the object's faces and edges to the pixel buffer in the
    Raster object.

  \param raster
    The Raster object that the object will be drawn to.

  \param object
    The object to be rasterized.
*******************************************************************************/
void Rasterize(Raster &raster, Object &object)
{
  RasterFaces(raster, object);
  RasterEdges(raster, object);
}

/*!
******************************************************************************
  \fn void RasterFaces(Raster &raster, const Object &object)

  \brief
    Given an Raster object and an Object object, clip and draw the object's
    faces to the screen and draw them to the Raster object's pixel buffer.

  \param raster
    The Raster object that the object will be drawn to.

  \param object
    The object to be rasterized.
*******************************************************************************/
void RasterFaces(Raster &raster, Object &object)
{
    /* Reusable vertex buffer for clipping the current object. */
  static std::vector<Point> sClipVerts;

    /* Get a reference to the MyMesh that this object is made of. */
  MyMesh &mesh = object.GetMesh();

    /* Set the drawing color to the color of the face of the current object. */
  raster.SetColor(object.mFaceColor.red,
                  object.mFaceColor.grn,
                  object.mFaceColor.blu);
    /* Loop through each face of the current object. */
  for (int i = 0; i < mesh.FaceCount(); ++i)
  {
      /* Get a reference to each vertex of the current face. */
    const Mesh::Face &f = mesh.GetFace(i);
    const Point &P      = mesh.mTempVerts[f.index1];
    const Point &Q      = mesh.mTempVerts[f.index2];
    const Point &R      = mesh.mTempVerts[f.index3];
      /* Store the vertices in a std::vector<...> to send off for clipping. */
    sClipVerts.clear();
    sClipVerts.push_back(P);
    sClipVerts.push_back(Q);
    sClipVerts.push_back(R);
      /* If we clipped any of the object to the viewport. */
    if (gCameras.ClipToScreen(sClipVerts))
        /* Draw each new face that resulted from the clipping. */
      for (unsigned int j = 1; j < sClipVerts.size() - 1; ++j)
        FillTriangle(raster, sClipVerts[0], sClipVerts[j], sClipVerts[j + 1]);
  }
}

/*!
******************************************************************************
  \fn void RasterEdges(Raster &raster, const Object &object)

  \brief
    Given an Raster object and an Object object, clip and draw the object's
    edges to the screen and draw them to the Raster object's pixel buffer.

  \param raster
    The Raster object that the object will be drawn to.

  \param object
    The object to be rasterized.
*******************************************************************************/
void RasterEdges(Raster &raster, Object &object, Color color)
{
    /* Get a reference to the MyMesh that this object is made of. */
  MyMesh &mesh = object.GetMesh();

    /* Set the drawing color to the color of the edge of the current object. */
  raster.SetColor(color.red, color.grn, color.blu);
    /* Loop through each edge of the current object. */
  for (int i = 0; i < mesh.EdgeCount(); ++i)
  {
      /* Get a reference to each vertex of the current edge. */
    const Mesh::Edge &e = mesh.GetEdge(i);
    Point P             = mesh.mTempVerts[e.index1];
    Point Q             = mesh.mTempVerts[e.index2];
      /* If we clipped any of the object's current edge to the viewport draw it. */
    if (gCameras.ClipToScreen(P, Q))
      DrawLine(raster, P, Q);
  }
}
