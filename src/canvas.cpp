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

// helper function that computes the y-distnace of (x,y) from the line ab
float ydistance(float x, float y, point a, point b)
{
   return (static_cast<float>(b.y) - static_cast<float>(a.y)) * (static_cast<float>(x) - static_cast<float>(a.x)) 
   - (static_cast<float>(b.x) - static_cast<float>(a.x)) * (static_cast<float>(y) - static_cast<float>(a.y));
}

// helper function that computes the magnitude of a vector
float magnitude(float x, float y)
{
   return sqrt(pow(x, 2) + pow(y, 2));
}

// counterclockwise rotation of a vector by theta
point rotation(float x, float y, float theta)
{
   point p;
   // apply the 2d rotation matrix
   p.x = floor(cos(theta) * x - sin(theta) * y);
   p.y = floor(sin(theta) * x + cos(theta) * y);
   return p;
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
   if (_type == POINTS)
   {
      while(!_vertices.empty())
      {
         draw_point();
         // remove the first point from _vertices
         _vertices.erase (_vertices.begin(), _vertices.begin()+1);
      }
   }
   if (_type == LINES)
   {
      while(!_vertices.empty())
      {
         draw_line();
         // remove the first two points from _vertices
         _vertices.erase (_vertices.begin(), _vertices.begin()+2);
      }
   }
   else if (_type == TRIANGLES)
   {
      while(!_vertices.empty())
      {
         draw_triangle(true);
         // remove the first three points from _vertices
         _vertices.erase (_vertices.begin(), _vertices.begin()+3);
      }
   }
   else if (_type == POLYGONS)
   {
      while(!_centers.empty())
      {
         draw_polygon(true);
         // remove the first point from _centers, first orientation vector from _orientations, and first # of sides from _sides
         _centers.erase (_centers.begin(), _centers.begin()+1);
         _orientations.erase (_orientations.begin(), _orientations.begin()+1);
         _sides.erase (_sides.begin(), _sides.begin()+1);
      }
   }
   else if (_type == CIRCLES)
   {
      while(!_centers.empty())
      {
         draw_circle(true);
         // remove the first point from _centers and first radius vector from _radii
         _centers.erase (_centers.begin(), _centers.begin()+1);
         _radii.erase(_radii.begin(), _radii.begin()+1);
      }
   }
   else if (_type == SECTORS)
   {
      while(!_centers.empty())
      {
         draw_sector();
         // remove the first point from _centers, first orientation vector from _orientations, and first angle from _angles
         _centers.erase (_centers.begin(), _centers.begin()+1);
         _orientations.erase (_orientations.begin(), _orientations.begin()+1);
         _angles.erase (_angles.begin(), _angles.begin()+1);
      }
   }
   else if (_type == OUTLINED_TRIANGLES)
   {
      while(!_vertices.empty())
      {
         draw_triangle(false);
         // remove the first three points from _vertices
         _vertices.erase (_vertices.begin(), _vertices.begin()+3);
      }
   }
   else if (_type == OUTLINED_POLYGONS)
   {
      while(!_centers.empty())
      {
         draw_polygon(false);
         // remove the first point from _centers, first orientation vector from _orientations, and first # of sides from _sides
         _centers.erase (_centers.begin(), _centers.begin()+1);
         _orientations.erase (_orientations.begin(), _orientations.begin()+1);
         _sides.erase (_sides.begin(), _sides.begin()+1);
      }
   }
   else if (_type == OUTLINED_CIRCLES)
   {
      while(!_centers.empty())
      {
         draw_circle(false);
         // remove the first point from _centers and first radius vector from _radii
         _centers.erase (_centers.begin(), _centers.begin()+1);
         _radii.erase(_radii.begin(), _radii.begin()+1);
      }
   }

   // clear the stored information for program security
   _vertices.clear();
   _radii.clear();
   _orientations.clear();
   _sides.clear();
   _angles.clear();
   _centers.clear();
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

void canvas::vertex(point p)
{
   // add a point p to _vertices
   _vertices.push_back(p);
}

void canvas::center(point p)
{
   // add a point p to _centers
   _centers.push_back(p);
}

void canvas::center(int x, int y)
{
   // add a point with position (x,y) and color _color to _centers
   point pt;
   pt.x = x;
   pt.y = y;
   pt.r = _color.r;
   pt.g = _color.g;
   pt.b = _color.b;

   _centers.push_back(pt);
}

void canvas::orientation(int x, int y)
{
   // add a vector to _orientations. Its entries can be negative
   point v;
   v.x = x;
   v.y = y;

   _orientations.push_back(v);
}

void canvas::side(int n)
{
   // add a # of sides to _sides
   _sides.push_back(n);
}

void canvas::radius(int r)
{
   // add a radius of circle to _radii
   _radii.push_back(r);
}

void canvas::angle(float theta)
{
   // add an angle to _angles
   _angles.push_back(theta);
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

void canvas::background(ppm_pixel tl, ppm_pixel tr, ppm_pixel bl, ppm_pixel br)
{
   // fill the background by filling two triangles
   point p1;
   point p2;
   point p3;
   point p4;

   p1.x = 0;
   p1.y = 0;
   p1.r = tl.r;
   p1.g = tl.g;
   p1.b = tl.b;

   p2.x = _canvas.width()-1;
   p2.y = 0;
   p2.r = tr.r;
   p2.g = tr.g;
   p2.b = tr.b;

   p3.x = 0;
   p3.y = _canvas.height()-1;
   p3.r = bl.r;
   p3.g = bl.g;
   p3.b = bl.b;

   p4.x = _canvas.width()-1;
   p4.y = _canvas.height()-1;
   p4.r = br.r;
   p4.g = br.g;
   p4.b = br.b;

   draw_triangle(p1, p3, p4, true);
   draw_triangle(p1, p2, p4, true);
}

void canvas::draw_point()
{
   // First, check if there is (at least) one point given in _vertices. We will use the first point in _vertices.
   assert((_vertices.size() >= 1) && "At least one point is required to draw a point!");
   point p = _vertices[0]; 

   // specify the color
   ppm_pixel color;
   color.r = p.r;
   color.g = p.g;
   color.b = p.b;

   // draw it!
   if (p.y >=0 && p.y < _canvas.height() && p.x >= 0 && p.x < _canvas.width())
   {
      _canvas.set(p.y, p.x, color);
   }
}

void canvas::draw_line()
{
   // First, check if there are (at least) two points given in _vertices. We will use the first two points in _vertices.
   assert((_vertices.size() >= 2) && "At least two points are required to draw a line!");
   point a = _vertices[0];
   point b = _vertices[1];

   // draw the line with different methods based on its slope
   int W = b.x - a.x;
   int H = b.y - a.y;
   // throw a warning if a = b
   if (W == 0 && H == 0)
   {
      std::cout << "WARNING: Two same vertices are given to draw a line. The color of the vertex would be consistent with the latter." << std::endl;
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

void canvas::draw_line(point p1, point p2)
{
   point a = p1; // start point
   point b = p2; // end point

   // draw the line with different methods based on its slope
   int W = b.x - a.x;
   int H = b.y - a.y;
   // throw a warning if a = b
   if (W == 0 && H == 0)
   {
      std::cout << "WARNING: Two same vertices are given to draw a line. The color of the vertex would be consistent with the latter." << std::endl;
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

void canvas::draw_triangle(bool filled)
{
   // First, check if there are (at least) three points given in _vertices. We will use the first three points in _vertices.
   assert((_vertices.size() >= 3) && "At least three points are required to draw a triangle!");
   point a =  _vertices[0]; 
   point b = _vertices[1]; 
   point c = _vertices[2];

   // if a,b,c are collinear, we draw the line that contains all three points (e.g. if b is between a and c, then the line segment ac is drawn)
   if ((b.x - a.x) * (c.y - a.y) == (c.x - a.x) * (b.y - a.y)){
      // throw a warning
      std::cout << "WARNING: Three colinear points are given to draw a triangle! The minimal line segment that contains the three points is drawn instead." << std::endl;
      if ((a.x <= b.x <= c.x || c.x <= b.x <= a.x) && (a.y <= b.y <= c.y || c.y <= b.y <= a.y)){
         draw_line(a,c);
      }
      else if ((b.x <= a.x <= c.x || c.x <= a.x <= b.x) && (b.y <= a.y <= c.y || c.y <= a.y <= b.y)){
         draw_line(b,c);
      }
      else{
         draw_line(a,b);
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

            if(filled){
               // check if the pixel is inside the triangle
               if(alpha >= 0.0 && beta >= 0.0 && gamma >= 0.0)
               {
                  // decide the color of the pixel using linear interpolation of a.color, b.color and c.color
                  ppm_pixel color;
                  color.r = floor(alpha * static_cast<float>(a.r) + beta * static_cast<float>(b.r) + gamma * static_cast<float>(c.r));
                  color.g = floor(alpha * static_cast<float>(a.g) + beta * static_cast<float>(b.g) + gamma * static_cast<float>(c.g));
                  color.b = floor(alpha * static_cast<float>(a.b) + beta * static_cast<float>(b.b) + gamma * static_cast<float>(c.b));
                  // handle adjacent edges with a mysterious points
                  bool drawAlpha = (alpha > 0 || f_alpha * ydistance(-1, -1,b,c) > 0 || f_alpha * ydistance(-1.1, -1,b,c) > 0 );
                  bool drawBeta  = (beta  > 0 || f_beta * ydistance(-1, -1,a,c) > 0 || f_beta * ydistance(-1.1, -1,a,c) > 0);
                  bool drawGamma = (gamma > 0 || f_gamma * ydistance(-1, -1,a,b) > 0 || f_gamma * ydistance(-1.1, -1,a,b) > 0);
                  if (drawAlpha && drawBeta && drawGamma) {
                     if(i >= 0 && i < _canvas.height() && j >= 0 && j < _canvas.width())
                     {
                        _canvas.set(i, j, color);
                     }
                  }
               }
            }
            else{
               // draw the edges of the triangle
               draw_line(a,b);
               draw_line(a,c);
               draw_line(b,c);
            }
         }
      }
   }
}

void canvas::draw_triangle(point p1, point p2, point p3, bool filled)
{
   point a = p1;
   point b = p2;
   point c = p3;

   // if a,b,c are collinear, we draw the line that contains all three points (e.g. if b is between a and c, then the line segment ac is drawn)
   if ((b.x - a.x) * (c.y - a.y) == (c.x - a.x) * (b.y - a.y)){
      // throw a warning
      std::cout << "WARNING: Three colinear points are given to draw a triangle! The minimal line segment that contains the three points is drawn instead." << std::endl;
      if ((a.x <= b.x <= c.x || c.x <= b.x <= a.x) && (a.y <= b.y <= c.y || c.y <= b.y <= a.y)){
         draw_line(a,c);
      }
      else if ((b.x <= a.x <= c.x || c.x <= a.x <= b.x) && (b.y <= a.y <= c.y || c.y <= a.y <= b.y)){
         draw_line(b,c);
      }
      else{
         draw_line(a,b);
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

            if(filled){
               // check if the pixel is inside the triangle
               if(alpha >= 0.0 && beta >= 0.0 && gamma >= 0.0)
               {
                  // decide the color of the pixel using linear interpolation of a.color, b.color and c.color
                  ppm_pixel color;
                  color.r = floor(alpha * static_cast<float>(a.r) + beta * static_cast<float>(b.r) + gamma * static_cast<float>(c.r));
                  color.g = floor(alpha * static_cast<float>(a.g) + beta * static_cast<float>(b.g) + gamma * static_cast<float>(c.g));
                  color.b = floor(alpha * static_cast<float>(a.b) + beta * static_cast<float>(b.b) + gamma * static_cast<float>(c.b));
                  // handle adjacent edges with a mysterious points
                  bool drawAlpha = (alpha > 0 || f_alpha * ydistance(-1, -1,b,c) > 0 || f_alpha * ydistance(-1.1, -1,b,c) > 0 );
                  bool drawBeta  = (beta  > 0 || f_beta * ydistance(-1, -1,a,c) > 0 || f_beta * ydistance(-1.1, -1,a,c) > 0);
                  bool drawGamma = (gamma > 0 || f_gamma * ydistance(-1, -1,a,b) > 0 || f_gamma * ydistance(-1.1, -1,a,b) > 0);
                  if (drawAlpha && drawBeta && drawGamma) {
                     if(i >= 0 && i < _canvas.height() && j >= 0 && j < _canvas.width())
                     {
                        _canvas.set(i, j, color);
                     }
                  }
               }
            }
            else{
               // draw the edge of the triangle that is opposite of the circle's/polygon's center
               draw_line(b,c);
            }
         }
      }
   }
}

void canvas::draw_polygon(bool filled)
{
   // First, check if there are (at least) one points given in _centers, one orientation vector in _orientations, and one # of sides given in _sides
   assert((_centers.size() >= 1) && "At least one center point is required to draw a polygon!");
   assert((_orientations.size() >= 1) && "At least one orientation vector is required to draw a polygon!");
   assert((_sides.size() >= 1) && "At least one number of sides is required to draw a polygon!");
   point c = _centers[0]; 
   point v = _orientations[0]; 
   int n = _sides[0];

   // make sure n is positive
   assert((n > 0) && "The number of sides have to be positive!");

   // angle of each slice of triangle
   float dtheta = 2*M_PI/static_cast<float>(n);

   for (int i =0; i < n; i++)
   {
      // find the two points that define the current slice of triangle with center
      float theta1 = static_cast<float>(i) * dtheta;
      float theta2 = static_cast<float>(i+1) * dtheta;
      point v1 = rotation(v.x, v.y, theta1);
      point v2 = rotation(v.x, v.y, theta2);
      point p1;
      point p2;
      p1.x = c.x + v1.x;
      p1.y = c.y + v1.y;
      p2.x = c.x + v2.x;
      p2.y = c.y + v2.y;
      // set the colors of p1, p2 to that of c by default
      p1.r = c.r;
      p2.r = c.r;
      p1.g = c.g;
      p2.g = c.g;
      p1.b = c.b;
      p2.b = c.b;
      _polygon_vertices.push_back(p1);

      // draw the slice of triangle!
      if(filled){
         draw_triangle(c,p1,p2,true);
      }
      else{
         draw_triangle(c,p1,p2,false);
      } 
   }
}

void canvas::draw_circle(bool filled)
{
   // First, check if there are (at least) one point given in _centers and one radius in _radii
   assert((_centers.size() >= 1) && "At least one center point is required to draw a circle!");
   assert((_radii.size() >= 1) && "At least one radius is required to draw a circle!");
   point c = _centers[0]; 
   int r = _radii[0]; 
   
   // make sure r is positive
   assert((r > 0) && "Radius of a circle has to be positive!");

   // angle of each slice of triangle
   float dtheta = 2*M_PI/static_cast<float>(4*r);
   // starting point
   point p;
   p.x = r;
   p.y = 0;

   for (int i =0; i < 4*r; i++)
   {
      // find the two points that define the current slice of triangle with center
      float theta1 = static_cast<float>(i) * dtheta;
      float theta2 = static_cast<float>(i+1) * dtheta;
      point v1 = rotation(p.x, p.y, theta1);
      point v2 = rotation(p.x, p.y, theta2);
      point p1;
      point p2;
      p1.x = c.x + v1.x;
      p1.y = c.y + v1.y;
      p2.x = c.x + v2.x;
      p2.y = c.y + v2.y;
      // set the colors of p1, p2 to that of c by default
      p1.r = c.r;
      p2.r = c.r;
      p1.g = c.g;
      p2.g = c.g;
      p1.b = c.b;
      p2.b = c.b;

      // draw the slice of triangle!
      if(filled){
         draw_triangle(c,p1,p2,true);
      }
      else{
         draw_triangle(c,p1,p2,false);
      }
   }
}

void canvas::draw_sector()
{
   // First, check if there are (at least) one point given in _centers, one orientation vector in _orientations, and one angle in _angles
   assert((_centers.size() >= 1) && "At least one center point is required to draw a sector!");
   assert((_orientations.size() >= 1) && "At least one orientation vector is required to draw a sector!");
   assert((_angles.size() >= 1) && "At least one angle is required to draw a sector!");
   point c = _centers[0]; 
   point v = _orientations[0]; 
   float angle = _angles[0]; 
   
   // make sure angle is between 0 and 2pi (0 exclusive)
   assert((angle > 0) && (angle <= 2*M_PI) && "Angle of a sector must be in (0, 2pi]!");

   // angle of each slice of triangle
   float r = magnitude(v.x, v.y);
   float dtheta = angle/static_cast<float>(floor(static_cast<float>(2*r/M_PI*angle)));
   // starting point
   point p;
   p.x = r;
   p.y = 0;

   for (int i =0; i < floor(static_cast<float>(2*r/M_PI*angle)); i++)
   {
      // find the two points that define the current slice of triangle with center
      float theta1 = static_cast<float>(i) * dtheta;
      float theta2 = static_cast<float>(i+1) * dtheta;
      point v1 = rotation(v.x, v.y, theta1);
      point v2 = rotation(v.x, v.y, theta2);
      point p1;
      point p2;
      p1.x = c.x + v1.x;
      p1.y = c.y + v1.y;
      p2.x = c.x + v2.x;
      p2.y = c.y + v2.y;
      // set the colors of p1, p2 to that of c by default
      p1.r = c.r;
      p2.r = c.r;
      p1.g = c.g;
      p2.g = c.g;
      p1.b = c.b;
      p2.b = c.b;
      _polygon_vertices.push_back(p1);

      // draw the slice of triangle!
      draw_triangle(c,p1,p2,true);
   }
}

void canvas::drawLineLow(point a, point b)
{
   int W = b.x - a.x;
   int H = b.y - a.y;
   int inc = 1;

   // reverse H and the direction of the increment of y if H < 0
   if(H < 0){
      inc = -1;
      H = -H;
   }

   int F = 2*H - W;

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

      if(y >= 0 && y < _canvas.height() && x >= 0 && x < _canvas.width())
      {
         _canvas.set(y, x, color);
      }

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
   int inc = 1;

   // reverse W and the direction of the increment of x if W < 0
   if(W < 0){
      inc = -1;
      W = -W;
   }

   int F = 2*W - H;

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

      if(y >= 0 && y < _canvas.height() && x >= 0 && x < _canvas.width())
      {
         _canvas.set(y, x, color);
      }

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

point canvas::mid_point(point a, point b) const
{
   // compute the mid point between a and b
   point p;
   p.x = floor((static_cast<float>(a.x) + static_cast<float>(b.x))/2.0);
   p.y = floor((static_cast<float>(a.y) + static_cast<float>(b.y))/2.0);

   // assign the "mid-color" of a.color and b.color to p
   p.r = (a.r + b.r)/2;
   p.g = (a.g + b.g)/2;
   p.b = (a.b + b.b)/2;

   return p;
}

std::vector<point> canvas::polygon_vertices() const
{
   return _polygon_vertices;
}

ppm_pixel canvas::pixel_color(int row, int col) const
{
   return _canvas.get(row, col);
}

void canvas::clear_polygon_vertices()
{
   _polygon_vertices.clear();
}

point canvas::directional_vector(point p1, point p2) const
{
   point p;
   p.x = p2.x - p1.x;
   p.y = p2.y - p1.y;
   return p;
}

void canvas::polygon(point c, point v, int n)
{
   // make sure n is positive
   assert((n > 0) && "The number of sides have to be positive!");

   // angle of each slice of triangle
   float dtheta = 2*M_PI/static_cast<float>(n);

   for (int i =0; i < n; i++)
   {
      // find the two points that define the current slice of triangle with center
      float theta1 = static_cast<float>(i) * dtheta;
      float theta2 = static_cast<float>(i+1) * dtheta;
      point v1 = rotation(v.x, v.y, theta1);
      point v2 = rotation(v.x, v.y, theta2);
      point p1;
      point p2;
      p1.x = c.x + v1.x;
      p1.y = c.y + v1.y;
      p2.x = c.x + v2.x;
      p2.y = c.y + v2.y;
      // set the colors of p1, p2 to that of c by default
      p1.r = c.r;
      p2.r = c.r;
      p1.g = c.g;
      p2.g = c.g;
      p1.b = c.b;
      p2.b = c.b;
      _polygon_vertices.push_back(p1);
   }
}