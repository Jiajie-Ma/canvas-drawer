#include "canvas.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;
using namespace agl;

// helper function that computes the y-distnace of p from the line ab
float ydistance(point p, point a, point b)
{
   return (static_cast<float>(b.y) - static_cast<float>(a.y)) * (static_cast<float>(p.x) - static_cast<float>(a.x)) 
   - (static_cast<float>(b.x) - static_cast<float>(a.x)) * (static_cast<float>(p.y) - static_cast<float>(a.y));
}

// For a given poitn p, clamp values outside of the range of the canvas
point clamp(point p, int w, int h)
{
   if (p.x > w-1)
   {
      p.x = w-1;
   }
   if (p.x < 0)
   {
      p.x = 0;
   }
   if (p.y > h-1)
   {
      p.y = h-1;
   }
   if (p.y < 0)
   {
      p.y = 0;
   }

   return p;
}

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
      while(!_vertices.empty())
      {
         draw_line();
      }
   }
   else if (_type == TRIANGLES)
   {
      while(!_vertices.empty())
      {
         draw_triangle();
      }
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
   // First, check if there are (at least) two points given in _vertices. We will use the first two points in _vertices.
   assert((_vertices.size() >= 2) && "At least two points are required to draw a line!");
   point a = clamp(_vertices[0], _canvas.width(), _canvas.height()); // start point
   point b = clamp(_vertices[1], _canvas.width(), _canvas.height()); // end point
   // remove the two points from _vertices
   _vertices.erase (_vertices.begin(), _vertices.begin()+2);

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

void canvas::draw_triangle()
{
   // First, check if there are (at least) three points given in _vertices. We will use the first three points in _vertices.
   assert((_vertices.size() >= 3) && "At least three points are required to draw a triangle!");
   point a = clamp(_vertices[0], _canvas.width(), _canvas.height()); 
   point b = clamp(_vertices[1], _canvas.width(), _canvas.height()); 
   point c = clamp(_vertices[2], _canvas.width(), _canvas.height());
   // remove the three points from _vertices
   _vertices.erase (_vertices.begin(), _vertices.begin()+3);

   // if a,b,c are collinear, we draw the line that contains all three points (e.g. if b is between a and c, then the line segment ac is drawn)
   if ((b.x - a.x) * (c.y - a.y) == (c.x - a.x) * (b.y - a.y)){
      // throw a warning
      std::cout << "Three colinear points are given to draw a triangle! The minimal line segment that contains the three points is drawn instead." << std::endl;
      if ((a.x <= b.x <= c.x || c.x <= b.x <= a.x) && (a.y <= b.y <= c.y || c.y <= b.y <= a.y)){
         _vertices.push_back(a);
         _vertices.push_back(c);
         draw_line();
      }
      else if ((b.x <= a.x <= c.x || c.x <= a.x <= b.x) && (b.y <= a.y <= c.y || c.y <= a.y <= b.y)){
         _vertices.push_back(b);
         _vertices.push_back(c);
         draw_line();
      }
      else{
         _vertices.push_back(a);
         _vertices.push_back(b);
         draw_line();
      }
   }
   // otherwise, apply barycentric_fill algorithm
   else{
      // plug points into line equations
      float f_alpha = ydistance(a,b,c);
      float f_beta = ydistance(b,a,c);
      float f_gamma = ydistance(c,a,b);

      int ymin = min(a.y, min(b.y, c.y));
      int ymax = max(a.y, max(b.y, c.y));
      int xmin = min(a.x, min(b.x, c.x));
      int xmax = max(a.x, max(b.x, c.x));

      // visit each pixel in the bounding box
      for (int i = ymin; i <= ymax; i++)
      {
         for (int j = xmin; j <= xmax; j++)
         {
            // computing barycentric coordinates of the pixel
            point p;
            p.x = j;
            p.y = i;
            float alpha = ydistance(p,b,c)/f_alpha;
            float beta = ydistance(p,a,c)/f_beta;
            float gamma = ydistance(p,a,b)/f_gamma;

            // check if the pixel is inside the triangle
            if(alpha >= 0.0 && beta >= 0.0 && gamma >= 0.0)
            {
               // set up the mysterious point (-1,-1)
               point mp;
               mp.x = -1.0;
               mp.y = -1.0;
               // decide the color of the pixel using linear interpolation of a.color, b.color and c.color
               ppm_pixel color;
               color.r = floor(alpha * static_cast<float>(a.r) + beta * static_cast<float>(b.r) + gamma * static_cast<float>(c.r));
               color.g = floor(alpha * static_cast<float>(a.g) + beta * static_cast<float>(b.g) + gamma * static_cast<float>(c.g));
               color.b = floor(alpha * static_cast<float>(a.b) + beta * static_cast<float>(b.b) + gamma * static_cast<float>(c.b));
               // handle adjacent edges
               if (alpha > 0.0 && beta > 0.0 && gamma > 0.0)
               {
                  _canvas.set(i, j, color);
               }
               else if (alpha == 0.0 && f_alpha * ydistance(mp, b, c) >= 0.0)
               {
                  _canvas.set(i, j, color);
               }
               else if (beta == 0.0 && f_beta * ydistance(mp, a, c) >= 0.0)
               {
                  _canvas.set(i, j, color);
               }
               else if (gamma == 0.0 && f_gamma * ydistance(mp, a, b) >= 0.0)
               {
                  _canvas.set(i, j, color);
               }
            }
         }
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
