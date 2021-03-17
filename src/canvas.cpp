#include "canvas.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;
using namespace agl;

canvas::canvas(int w, int h) : _canvas(w, h)
{
   // no need to check the legality of w, h as iit is handled by ppm_image class
}

canvas::~canvas()
{
   // nothing to free as it is handled by ppm_image class
}

void canvas::save(const std::string& filename)
{
   _canvas.save_ppm(filename);
}

void canvas::begin(PrimitiveType type)
{
   // set _type with the given primitive type
   _type = type;
}

void canvas::end()
{
   // abort if the shape is not specified
   assert(_type != UNDEFINED && "The shape is not specified for the drawing!");

   // draw the shape specified by _type
   if (_type == LINES)
   {
      draw_line();
   }
   else if (_type == TRIANGLES)
   {

   }

   // clear the stored information for program security
   _vertices.clear();
   _type = UNDEFINED;
}

void canvas::vertex(int x, int y)
{
   // add a point with position (x,y) and color _color to _vertices
   point pt;
   pt.x = x;
   pt.y = y;
   pt.r = _color.r;
   pt.g = _color.g;
   pt.b = _color.b;

   _vertices.push_back(pt);
}

void canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   // set _color with the given RGB values
   _color.r = r;
   _color.g = g;
   _color.b = b;
}

void canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   // pack the given color into a pixel
   ppm_pixel color;
   color.r = r;
   color.g = g;
   color.b = b;

   // color the background of canvas pixel-wise
   for (int i = 0; i < _canvas.height(); i++)
   {
      for (int j = 0; j < _canvas.width(); j++)
      {
         _canvas.set(i,j,color);
      }
   }
}

void canvas::draw_line()
{
   // First, check if there are (at least) two points given in _vertices. We will use the last two points in _vertices.
   assert((_vertices.size() >= 2) && "At least two points are required to draw a line!");
   point a = _vertices[_vertices.size()-2]; // start point
   point b = _vertices[_vertices.size()-1]; // end point

   // clamp values outside of the range of the canvas
   if (a.x > _canvas.width()-1)
   {
      a.x = _canvas.width()-1;
   }
   if (a.x < 0)
   {
      a.x = 0;
   }
   if (b.x > _canvas.width()-1)
   {
      b.x = _canvas.width()-1;
   }
   if (b.x < 0)
   {
      b.x = 0;
   }
   if (a.y > _canvas.height()-1)
   {
      a.y = _canvas.height()-1;
   }
   if (a.y < 0)
   {
      a.y = 0;
   }
   if (b.y > _canvas.height()-1)
   {
      b.y = _canvas.height()-1;
   }
   if (b.y < 0)
   {
      b.y = 0;
   }

   // draw the line with different methods based on its slope
   int W = b.x - a.x;
   int H = b.y - a.y;
   // throw a warning if a = b
   if (W == 0 && H == 0)
   {
      std::cout << "Two same vertices are given to draw a line. The color of the vertex would be consistent with the latter." << std::endl;
      drawLineLow(a, b);
   }
   else if (abs(W) > abs(H)){
      if (a.x < b.x)
      {
         drawLineLow(a,b);
      }
      else{
         drawLineLow(b,a);
      }
   }
   else{
      if (a.y < b.y)
      {
         drawLineHigh(a,b);
      }
      else{
         drawLineHigh(b,a);
      }
   }
}

void canvas::drawLineLow(point a, point b)
{
   int W = b.x - a.x;
   int H = b.y - a.y;
   int F = 2*H - W;
   int inc = 1;

   // reverse H and the direction of the increment of y if H < 0
   if(H < 0){
      inc = -1;
      H = -H;
   }

   // Bresenham algorithm
   int x = a.x;
   int y = a.y;
   for (; x < b.x+1; x++)
   {
      // decide the color of the pixel using linear interpolation of a.color and b.color
      ppm_pixel color;
      if (b.x == a.x)
      {
         color.r = b.r;
         color.g = b.g;
         color.b = b.b;
      }
      else{
         float t = (static_cast<float>(x)-static_cast<float>(a.x))/(static_cast<float>(b.x)-static_cast<float>(a.x));
         
         color.r = floor(static_cast<float>(a.r) * (1.0 - t) + static_cast<float>(b.r) * t);
         color.g = floor(static_cast<float>(a.g) * (1.0 - t) + static_cast<float>(b.g) * t);
         color.b = floor(static_cast<float>(a.b) * (1.0 - t) + static_cast<float>(b.b) * t);
      }

      _canvas.set(y, x, color);

      if (F > 0)
      {
         y += inc;
         F += 2*(H-W);
      }
      else{
         F += 2*H;
      }
   }
}

void canvas::drawLineHigh(point a, point b)
{
   int W = b.x - a.x;
   int H = b.y - a.y;
   int F = 2*W - H;
   int inc = 1;

   // reverse W and the direction of the increment of x if W < 0
   if(W < 0){
      inc = -1;
      W = -W;
   }

   // Bresenham algorithm
   int x = a.x;
   int y = a.y;
   for (; y < b.y+1; y++)
   {
      // decide the color of the pixel using linear interpolation of a.color and b.color
      float t = (static_cast<float>(y)-static_cast<float>(a.y))/(static_cast<float>(b.y)-static_cast<float>(a.y));
      ppm_pixel color;
      color.r = floor(static_cast<float>(a.r) * (1 - t) + static_cast<float>(b.r) * t);
      color.g = floor(static_cast<float>(a.g) * (1 - t) + static_cast<float>(b.g) * t);
      color.b = floor(static_cast<float>(a.b) * (1 - t) + static_cast<float>(b.b) * t);

      _canvas.set(y, x, color);

      if (F > 0)
      {
         x += inc;
         F += 2*(W-H);
      }
      else{
         F += 2*W;
      }
   }
}

