/*!
*****************************************************************************
\file   Raster.h
\author Connor Bracy
\par    DigiPen Login: connor.bracy
\par    DP email: connor.bracy\@digipen.edu
\par    Course: CS200
\par    Assignemnt #4
\date   06/02/2015

\brief
  This file contains the definitions of the elementary raster graphics functions
*******************************************************************************/
#ifndef CS200_RASTER_H
#define CS200_RASTER_H

#include <cassert>


class Raster
{
/*############################################################################*/
/*############################## Public Defines ##############################*/
/*############################################################################*/
public:
  typedef unsigned char byte;


/*############################################################################*/
/*############################## Public Methods ##############################*/
/*############################################################################*/
public:

  /******************************* Constructors *******************************/
    /*
     * Constructor - Creates an instance of the Raster class associated to the
     * frame buffer starting at address rgb data. The frame buffer is W pixels
     * wide, and H pixels tall, with each scan line taking up S bytes (the
     * stride of each scan line). The caller of constructor is responsible for
     * allocating the memory for the frame buffer, and the memory is expected
     * to persist over the lifetime of the class instance.
     */
  Raster(byte *rgb_data, int width, int height, int stride);
    /*
     * Writes a pixel to the frame buffer at the current point in the current
     * foreground color. See the comments below.
     */
  void WritePixel(void);

  /********************************* Setters **********************************/
    /*
     * Sets the current point to pixel location (x, y). No data is written to
     * the frame buffer.
     */
  void GotoPoint(int x, int y);
    /* Sets the current foreground color. */
  void SetColor(byte r, byte g, byte b);

  /********************************* Mutators *********************************/
    /* Moves the current point one pixel to the right. */
  void IncrementX(void);
    /* Moves the current point one pixel to the left. */
  void DecrementX(void);
    /* Moves the current point one pixel upwards. */
  void IncrementY(void);
    /* Moves the current point one pixel downwards. */
  void DecrementY(void);


/*############################################################################*/
/*############################# Private Members ##############################*/
/*############################################################################*/
private:
  byte *buffer;
  int width;
  int height;
  int stride;
  int current_index;
  int current_x;
  int current_y;
  byte red;
  byte green;
  byte blue;
};


/*############################################################################*/
/*########################## Inline Implementations ##########################*/
/*############################################################################*/

//#define NDEBUG
#ifndef NDEBUG
#define USE_CURRENT
#endif

#define CLIP_PIXELS
#ifdef CLIP_PIXELS
#define USE_CURRENT
#endif

/*!
******************************************************************************
  \fn Raster(byte *d, int w, int h, int s)

  \brief
    Constructor - Creates an instance of the Raster class associated to the
    frame buffer starting at address rgb data. The frame buffer is W pixels
    wide, and H pixels tall, with each scan line taking up S bytes (the
    stride of each scan line). The caller of constructor is responsible for
    allocating the memory for the frame buffer, and the memory is expected
    to persist over the lifetime of the class instance.
*******************************************************************************/
inline
Raster::Raster(byte *d, int w, int h, int s)
: buffer(d), width(w), height(h), stride(s),
red(0), green(0), blue(0)
{
}

/*!
******************************************************************************
  \fn WritePixel(void)

  \brief
    Writes a pixel to the frame buffer at the current point in the current
    foreground color. See the comments below.
*******************************************************************************/
inline
void Raster::WritePixel(void)
{
#ifdef CLIP_PIXELS
  if ((current_x < 0) || (width  <= current_x) ||
      (current_y < 0) || (height <= current_y))
      return;
#endif
#ifndef NDEBUG
  assert((0 <= current_x) && (current_x < width) &&
         (0 <= current_y) && (current_y < height));
#endif
  int index = current_index;
  buffer[index]   = red;
  buffer[++index] = green;
  buffer[++index] = blue;
}

/*!
******************************************************************************
  \fn GotoPoint(int x, int y)

  \brief
    Sets the current point to pixel location (x, y). No data is written to
    the frame buffer.
*******************************************************************************/
inline
void Raster::GotoPoint(int x, int y)
{
#ifdef USE_CURRENT
  current_x = x;
  current_y = y;
#endif
  current_index = ((y * stride) + (3 * x));
}

/*!
******************************************************************************
  \fn SetColor(byte r, byte g, byte b)

  \brief
    Sets the current foreground color.
*******************************************************************************/
inline
void Raster::SetColor(byte r, byte g, byte b)
{
  red   = r;
  green = g;
  blue  = b;
}

/*!
******************************************************************************
  \fn IncrementX(void)

  \brief
    Moves the current point one pixel to the right.
*******************************************************************************/
inline
void Raster::IncrementX(void)
{
#ifndef USE_CURRENT
  ++current_x;
#endif
  current_index += 3;
}

/*!
******************************************************************************
  \fn DecrementX(void)

  \brief
    Moves the current point one pixel to the left.
*******************************************************************************/
inline
void Raster::DecrementX(void)
{
#ifndef USE_CURRENT
  --current_x;
#endif
  current_index -= 3;
}

/*!
******************************************************************************
  \fn IncrementY(void)

  \brief
    Moves the current point one pixel upwards.
*******************************************************************************/
inline
void Raster::IncrementY(void)
{
#ifndef USE_CURRENT
  ++current_y;
#endif
  current_index += stride;
}

/*!
******************************************************************************
  \fn DecrementY(void)

  \brief
    Moves the current point one pixel downwards.
*******************************************************************************/
inline
void Raster::DecrementY(void)
{
#ifndef USE_CURRENT
  --current_y;
#endif
  current_index -= stride;
}


#endif

