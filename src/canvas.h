#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "ppm_image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES};
   // point struct that stores the position and color of a point
   struct point
  {
     int x;
     int y;
     unsigned char r;
     unsigned char g;
     unsigned char b;
  };
  
   class canvas
   {
   public:
      canvas(int w, int h);
      virtual ~canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // Line interpolation using the Bresenham algorithm
      void draw_line();

      // helper function for canvas::draw_line method that draws a line (between a and b) whose slope is between -1 and 1
      void drawLineLow(point a, point b);

      // helper function for canvas::draw_line method that draws a line whose slope is  > 1 or < 1
      void drawLineHigh(point a, point b);

   private:
      ppm_image _canvas;
      PrimitiveType _type; // current primitive to draw
      ppm_pixel _color; // current color for vertex
      std::vector<point> _vertices; // current vertices to draw
   };
}

#endif


