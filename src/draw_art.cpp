#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

// Draw Sierpinski Triangle with n iterations
void SierpinskiTriangle(canvas& drawer, point p1, point p2, point p3, int n, bool filled)
{
   // First, draw the outlined triangle p1p2p3
   drawer.begin(OUTLINED_TRIANGLES);
   drawer.vertex(p1);
   drawer.vertex(p2);
   drawer.vertex(p3);
   drawer.end();

   // find the mid point of p1p2, p1p3, and p2p3
   point p1p2 = drawer.mid_point(p1, p2);
   point p1p3 = drawer.mid_point(p1, p3);
   point p2p3 = drawer.mid_point(p2, p3);

   if (filled){
      // fill the triangle with vertices p1p2, p1p3, p2p3
      drawer.begin(TRIANGLES);
      drawer.vertex(p1p2);
      drawer.vertex(p2p3);
      drawer.vertex(p1p3);
      drawer.end();
   }
   

   // call SierpinskiTriangle recursively with the other partitioned triangles
   if (n-1 > 0){
      SierpinskiTriangle(drawer, p1, p1p2, p1p3, n-1, filled);
      SierpinskiTriangle(drawer, p1p2, p2, p2p3, n-1, filled);
      SierpinskiTriangle(drawer, p1p3, p2p3, p3, n-1, filled);
   }
}

// Draw fractal hexagon with n iterations
void FractalHexagon(canvas& drawer, point c, point v, int n, bool filled)
{
   if(n == 1){
      // draw the filled polygon
      if(filled){
         c.r = rand() % 255;
         c.g = rand() % 255;
         c.b = rand() % 255;
         drawer.begin(POLYGONS);
         drawer.center(c);
         drawer.side(6);
         drawer.orientation(v.x, v.y);
         drawer.end();
         drawer.clear_polygon_vertices();
      }
      else{
         c.r = rand() % 255;
         c.g = rand() % 255;
         c.b = rand() % 255;
         drawer.begin(OUTLINED_POLYGONS);
         drawer.center(c);
         drawer.side(6);
         drawer.orientation(v.x, v.y);
         drawer.end();
         drawer.clear_polygon_vertices();
      }
      
   }
   else{
      // find the vertices of the current hexagon
      drawer.polygon(c, v, 6);

      // record the vertices
      point p1 = drawer.polygon_vertices()[0];
      point p2 = drawer.polygon_vertices()[1];
      point p3 = drawer.polygon_vertices()[2];
      point p4 = drawer.polygon_vertices()[3];
      point p5 = drawer.polygon_vertices()[4];
      point p6 = drawer.polygon_vertices()[5];
      drawer.clear_polygon_vertices();

      point new_c = drawer.mid_point(p1,p2);
      point new_v = drawer.directional_vector(new_c, p2);
      FractalHexagon(drawer, new_c, new_v, n-1, filled);

      new_c = drawer.mid_point(p2,p3);
      new_v = drawer.directional_vector(new_c, p3);
      FractalHexagon(drawer, new_c, new_v, n-1, filled);

      new_c = drawer.mid_point(p3,p4);
      new_v = drawer.directional_vector(new_c, p4);
      FractalHexagon(drawer, new_c, new_v, n-1, filled);

      new_c = drawer.mid_point(p4,p5);
      new_v = drawer.directional_vector(new_c, p5);
      FractalHexagon(drawer, new_c, new_v, n-1, filled);

      new_c = drawer.mid_point(p5,p6);
      new_v = drawer.directional_vector(new_c, p6);
      FractalHexagon(drawer, new_c, new_v, n-1, filled);

      new_c = drawer.mid_point(p6,p1);
      new_v = drawer.directional_vector(new_c, p1);
      FractalHexagon(drawer, new_c, new_v, n-1, filled);
   }
}

int main(int argc, char** argv)
{
   // Artwork results from canvas class
   canvas drawer(640, 640);

   // Sierppinski Triangle
   // specify a fancy background (color palette: Japanese newspaper)
   ppm_pixel tl;
   ppm_pixel tr;
   ppm_pixel bl;
   ppm_pixel br;

   tl.r = 255;
   tl.g = 2;
   tl.b = 142; 
   tr.r = 255;
   tr.g = 152;
   tr.b = 25;
   bl.r = 135;
   bl.g = 198;
   bl.b = 255;
   br.r = 116;
   br.g = 255;
   br.b = 168;
   
   
   drawer.background(tl, tr, bl, br);

   // draw Sierpniski triangle using mid_point, polygon and triangle methods (both outlined and filled)
   drawer.begin(OUTLINED_POLYGONS);
   drawer.center(320,360);
   drawer.side(3);
   drawer.orientation(0,-240);
   drawer.end();

   point p1 = drawer.polygon_vertices()[0];
   p1.r = 0;
   p1.g = 255;
   p1.b = 255;
   point p2 = drawer.polygon_vertices()[1];
   p2.r = 230;
   p2.g = 0;
   p2.b = 230;
   point p3 = drawer.polygon_vertices()[2];
   p3.r = 255;
   p3.g = 255;
   p3.b = 0;
   drawer.clear_polygon_vertices();
   
   
   SierpinskiTriangle(drawer, p1, p2, p3, 6, true);

   drawer.save("Sierpinski triangle.png");

   // Sierppinski Triangle Wall paper
   canvas drawer2(640, 320);

   for (int i = 0; i < 10; i ++)
   {
      for (int j = 0; j < 21; j ++)
      {
         point p1;
         point p2;
         point p3;
         if (j%2 == 0){
            p1.x = j*32;
            p1.y = i*32;
            p2.x = (j-1)*32;
            p2.y = (i+1)*32;
            p3.x = (j+1)*32;
            p3.y = (i+1)*32;
         }
         else{
            p1.x = j*32;
            p1.y = (i+1) * 32;
            p2.x = (j-1)*32;
            p2.y = i * 32;
            p3.x = (j+1)*32;
            p3.y = i * 32;
         }
         p1.r = 255;
         p1.g = 255;
         p1.b = 0;
         p2.r = 255;
         p2.g = 0;
         p2.b = 255;
         p3.r = 0;
         p3.g = 255;
         p3.b = 255;

         SierpinskiTriangle(drawer2, p1, p2, p3, 6, false);
      }
   }

   drawer2.save("Sierpinski triangle Wallpaper.png");

   // Illusion wallpaper
   drawer2.background(0,0,0);

   for (int i = 0; i < 10; i ++)
   {
      for (int j = 0; j < 21; j ++)
      {
         point p1;
         point p2;
         point p3;
         if (j%2 == 0){
            p1.x = j*32;
            p1.y = i*32;
            p2.x = (j-1)*32;
            p2.y = (i+1)*32;
            p3.x = (j+1)*32;
            p3.y = (i+1)*32;
         }
         else{
            p1.x = j*32;
            p1.y = (i+1) * 32;
            p2.x = (j-1)*32;
            p2.y = i * 32;
            p3.x = (j+1)*32;
            p3.y = i * 32;
         }
         p1.r = 255;
         p1.g = 255;
         p1.b = 0;
         p2.r = 255;
         p2.g = 0;
         p2.b = 255;
         p3.r = 0;
         p3.g = 255;
         p3.b = 255;

         drawer2.begin(TRIANGLES);
         drawer2.vertex(p1);
         drawer2.vertex(p2);
         drawer2.vertex(p3);
         drawer2.end();
      }
   }

   drawer2.save("Illusion.png");


   // Hexagon tiling wallpaper
   drawer.background(255, 255, 255); // set background to white

   // draw the fractal 
   point c;
   c.x = 320;
   c.y = 320;
   c.r = 0;
   c.g = 0;
   c.b = 0;
   point v;
   v.x = 320;
   v.y = 0;

   FractalHexagon(drawer, c, v, 4, true);

   drawer.save("Filled Hexagon Tiling.png");

   // set background to black
   drawer.background(0, 0, 0);

   // draw the fractal 

   FractalHexagon(drawer, c, v, 4, false);

   drawer.save("Hexagon Tiling.png");


   // colorful origami paper
   drawer.background(255, 255, 255); // set background to white

   // randomly select a point on canvas and check if we should color it (and its neighborhood)
   for (int i = 0; i < 640*40; i++)
   {
      point p;
      p.x = rand()%640;
      p.y = rand()%640;
      p.r = rand()%255;
      p.g = rand()%255;
      p.b = rand()%255;
      int J = rand()%5+3;
      int K = rand()%5+3;
      for (int j = 0; j < J; j++){
         for (int k = 0; k < K; k++)
         {
            if (rand()%3 != -1 ){
               drawer.begin(POINTS);
               drawer.color(p.r, p.g, p.b);
               drawer.vertex(p.x+j, p.y+k);
               drawer.end();
            }
         }
      }
   }
   
   drawer.save("Colorful Origami Paper.png");

   // Pokemon Ball
   drawer.background(21, 0, 255); // set the background to blue

   // outer circle
   drawer.begin(SECTORS);
   drawer.color(255,255,255);
   drawer.center(320,320);
   drawer.orientation(210, 0);
   drawer.angle(M_PI);
   drawer.color(255,31,31);
   drawer.center(320,320);
   drawer.orientation(-210, 0);
   drawer.angle(M_PI);
   drawer.end();

   // inner circle
   drawer.begin(CIRCLES);
   drawer.color(255,255,255);
   drawer.center(320,320);
   drawer.radius(60);
   drawer.end();

   // outer stroke. Calling outlined_circles primitive repeatedly to expand the line width
   drawer.begin(OUTLINED_CIRCLES);
   drawer.color(0,0,0);
   drawer.center(320,320);
   drawer.radius(210);
   drawer.center(320,320);
   drawer.radius(211);
   drawer.center(320,320);
   drawer.radius(212);
   drawer.center(320,320);
   drawer.radius(213);
   drawer.center(320,320);
   drawer.radius(214);
   drawer.center(320,320);
   drawer.radius(60);
   drawer.center(320,320);
   drawer.radius(61);
   drawer.center(320,320);
   drawer.radius(62);
   drawer.center(320,320);
   drawer.radius(63);
   drawer.center(320,320);
   drawer.radius(64);
   drawer.center(320,320);
   drawer.radius(60);
   drawer.center(320,320);
   drawer.radius(61);
   drawer.center(320,320);
   drawer.radius(62);
   drawer.center(320,320);
   drawer.radius(63);
   drawer.center(320,320);
   drawer.radius(64);
   drawer.center(320,320);
   drawer.radius(40);
   drawer.center(320,320);
   drawer.radius(41);
   drawer.center(320,320);
   drawer.radius(42);
   drawer.center(320,320);
   drawer.radius(43);
   drawer.center(320,320);
   drawer.radius(44);
   drawer.end();

   // boundary of the hemispheres
   drawer.begin(LINES);
   drawer.vertex(320-210,320);
   drawer.vertex(320-60, 320);
   drawer.vertex(320-210,319);
   drawer.vertex(320-60, 319);
   drawer.vertex(320-210,318);
   drawer.vertex(320-60, 318);
   drawer.vertex(320-210,321);
   drawer.vertex(320-60, 321);
   drawer.vertex(320-210,322);
   drawer.vertex(320-60, 322);
   drawer.vertex(320+210,320);
   drawer.vertex(320+60, 320);
   drawer.vertex(320+210,321);
   drawer.vertex(320+60, 321);
   drawer.vertex(320+210,322);
   drawer.vertex(320+60, 322);
   drawer.vertex(320+210,319);
   drawer.vertex(320+60, 319);
   drawer.vertex(320+210,318);
   drawer.vertex(320+60, 318);
   drawer.end();

   drawer.save("Pokemon Ball.png");

}

