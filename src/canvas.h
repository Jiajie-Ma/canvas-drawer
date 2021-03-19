#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "ppm_image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES, POINTS, POLYGONS, CIRCLES, SECTORS, OUTLINED_TRIANGLES, OUTLINED_POLYGONS, OUTLINED_CIRCLES};
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

      // Specify a vertex by a point
      void vertex(point p);

      // Specifiy a center point at raster position (x,y)
      // x corresponds to the column; y to the row
      void center(int x, int y);

      // Specify a center by a point
      void center(point p);

      // Specify an orientation vector (x,y)
      void orientation(int x, int y);

      // Specify the number of sides
      void side(int n);

      // Specify the radius of a circle
      void radius(int r);

      // Specify the angle of a sector
      void angle(float theta);

      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

      // Linear interpolation of canvas background by giving colors to the four corners
      void background(ppm_pixel tl, ppm_pixel tr, ppm_pixel bl, ppm_pixel br);

      // Drawing a point
      void draw_point();

      // Line interpolation using the Bresenham algorithm
      void draw_line();

      // draw_line method that is called by other drawing methods
      void draw_line(point p1, point p2);

      // Triangle interpolation using barycentric coordinates
      void draw_triangle(bool filled);

      // draw_triangle method that is called by other drawing methods
      void draw_triangle(point p1, point p2, point p3, bool filled);

      // Draw a regular polygon with a specified center, number of sides, and orientation of the shape
      void draw_polygon(bool filled);

      // Draw a circle/sector with a specified center, angle and radius
      void draw_circle(bool filled);

      // Draw a sector with a specified center, angle and orientation
      void draw_sector();

      // Helper function for canvas::draw_line method that draws a line (between a and b) whose slope is between -1 and 1 (exclusive)
      void drawLineLow(point a, point b);

      // Helper function for canvas::draw_line method that draws a line whose slope is  > 1 or < 1
      void drawLineHigh(point a, point b);

      // find the midpoint of a given line
      point mid_point(point a, point b) const;

      // return the recorded vertices for a polygon
      std::vector<point> polygon_vertices() const;

      // get the color of a pixel in canvas
      ppm_pixel pixel_color(int row, int col) const;

      // clear the elements in _polygon_vertices
      void clear_polygon_vertices();

      // find the directional vector between two points
      point directional_vector(point p1, point p2) const;

      // find the vertices of an n-polygon without drawing
      void polygon(point c, point v, int n);

   private:
      ppm_image _canvas;
      PrimitiveType _type; // current primitive to draw
      ppm_pixel _color; // current color for vertex
      std::vector<point> _vertices; // current vertices to draw
      std::vector<point> _centers; // current vertices/centers to draw
      std::vector<point> _orientations; // current orientation vector to draw
      std::vector<int> _radii; // current radius of circle to draw
      std::vector<int> _sides; // current number of sides for a polygon to draw
      std::vector<float> _angles; // current angle for a sector to draw
      std::vector<point> _polygon_vertices; // record the vertices of a polygon for artwork purpose
   };
}

#endif


