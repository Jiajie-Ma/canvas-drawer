//----------------------------------------
// PPM image class
// Author: Jiajie(Jason) Ma
//----------------------------------------

#pragma once
#include <string>

namespace agl
{
  struct ppm_pixel
  {
     unsigned char r;
     unsigned char g;
     unsigned char b;
  };

  class ppm_image
  {
  public:
     ppm_image();
     ppm_image(int width, int height);
     ppm_image(const ppm_image& orig);
     ppm_image& operator=(const ppm_image& orig);

     virtual ~ppm_image();

     // load the given filename
     // returns true if the load is successful; false otherwise
     bool load(const std::string& filename);

     // save the given filename in ppm file format
     // returns true if the save is successful; false otherwise
     bool save_ppm(const std::string& filename) const;

     // save the given filename in png file format
     // returns true if the save is successful; false otherwise
     bool save(const std::string& filename) const;

     // Returns a copy of this image resized to the given width and height
     ppm_image resize(int width, int height) const;

     // Return a copy of this image flipped around the horizontal midline
     ppm_image flip_horizontal() const;

     // Return a sub-image having the given top,left coordinate and (width, height)
     ppm_image subimage(int x, int y, int w, int h) const;

     // Return a copy of this image with the given gamma correction applied to it
     ppm_image gammaCorrect(float gamma) const;

     // Return a copy of this image converted to grayscale
     ppm_image grayscale() const;

     // Replace the portion starting at (row, col) with the given image
     // Clamps the image if it doesn't fit on this image
     void replace(const ppm_image& image, int x, int y);

     // Apply the following calculation to the pixels in 
     // our image and the given image:
     //    this.pixels = this.pixels * (1-alpha) + other.pixel * alpha
     // Can assume that the two images are the same size
     ppm_image alpha_blend(const ppm_image& other, float amount) const;

     // Combine a copy of our image and a given image so that every pixel is taken to be the brightest
     // Assume that the two images are the same size
     ppm_image lightest(const ppm_image& other) const;

     // Combine a copy of our image and a given image so that every pixel is taken to be the darkest
     // Assume that the two images are the same size
     ppm_image darkest(const ppm_image& other) const;

     // Return a copy of this image whose colors are rotated
     ppm_image swirlcolor() const;

     // Return a copy of this image whose colors are inverted
     ppm_image invert(float alpha) const;

     // Given a threshold, return a copy of this image that detects the edges (with a specified color of edge (dark/bright))
     ppm_image sobel(int threshold, bool reverse) const;

     // Return a copy of this image that is applied with 5*5 Gaussian smoothing
     ppm_image gaussianblur() const;

     // Return a copy of this image that is sharpened
     ppm_image sharpen() const;

     // Get the pixel at index (row, col)
     ppm_pixel get(int row, int col) const;

     // Set the pixel at index (row, col)
     void set(int row, int col, const ppm_pixel& c);

     // return the width of this image
     int width() const;

     // return the height of the image
     int height() const;

     // clean up the memory occupied by the object
     void cleanup();

   protected:
      std::string format; // image format, e.g. "P3" by default
      int w, h; // width and height of the image
      int m; // maximum color value, e.g. "255" by default
      int*** p; // 3D array that stores the pixels of the image. The dimension is h * w * 3 and he default value for a pixel is (0,0,0).
  };
}
