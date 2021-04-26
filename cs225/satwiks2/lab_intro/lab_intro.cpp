#include <iostream>
#include <cmath>
#include <cstdlib>

#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include "lab_intro.h"

using namespace cs225;

/**
 * Returns an image that has been transformed to grayscale.
 *
 * The saturation of every pixel is set to 0, removing any color.
 *
 * @return The grayscale image.
 */
PNG grayscale(PNG image) {
  /// This function is already written for you so you can see how to
  /// interact with our PNG class.
  for (unsigned x = 0; x < image.width(); x++) {
    for (unsigned y = 0; y < image.height(); y++) {
      HSLAPixel & pixel = image.getPixel(x, y);

      // `pixel` is a pointer to the memory stored inside of the PNG `image`,
      // which means you're changing the image directly.  No need to `set`
      // the pixel since you're directly changing the memory of the image.
      pixel.s = 0;
    }
  }

  return image;
}



/**
 * Returns an image with a spotlight centered at (`centerX`, `centerY`).
 *
 * A spotlight adjusts the luminance of a pixel based on the distance the pixel
 * is away from the center by decreasing the luminance by 0.5% per 1 pixel euclidean
 * distance away from the center.
 *
 * For example, a pixel 3 pixels above and 4 pixels to the right of the center
 * is a total of `sqrt((3 * 3) + (4 * 4)) = sqrt(25) = 5` pixels away and
 * its luminance is decreased by 2.5% (0.975x its original value).  At a
 * distance over 160 pixels away, the luminance will always decreased by 80%.
 * 
 * The modified PNG is then returned.
 *
 * @param image A PNG object which holds the image data to be modified.
 * @param centerX The center x coordinate of the crosshair which is to be drawn.
 * @param centerY The center y coordinate of the crosshair which is to be drawn.
 *
 * @return The image with a spotlight.
 */
PNG createSpotlight(PNG image, int centerX, int centerY) {

    for (int x = 0; x < image.width(); x++) {
        int xLength_ = abs(x - centerX);
        for (int y = 0; y < image.height(); y++) {
            HSLAPixel &pixel = image.getPixel(x, y);
            int yLength_ = abs(y - centerY);
            double dist_to_center_ = std::sqrt((yLength_*yLength_ + xLength_*xLength_));
            if ((int)dist_to_center_ >= 160) {
                pixel.l *= 0.2;
                continue;
            }
            pixel.l *= 1 - (0.005 * dist_to_center_);
        }
    }
    return image;
  
}
 

/**
 * Returns a image transformed to Illini colors.
 *
 * The hue of every pixel is set to the a hue value of either orange or
 * blue, based on if the pixel's hue value is closer to orange than blue.
 *
 * @param image A PNG object which holds the image data to be modified.
 *
 * @return The illinify'd image.
**/
PNG illinify(PNG image) {
    for (unsigned x = 0; x < image.width(); x++) {
        for (unsigned y = 0; y < image.height(); y++) {
            HSLAPixel &pixel = image.getPixel(x,y);
            double angle_diff1_ = std::abs((pixel.h - 11));
            double angle_diff2_ = std::abs((pixel.h - 216));
            double dist_to_orange_ = (angle_diff1_ > 180) ? (360 - angle_diff1_): angle_diff1_;
            double dist_to_blue_ = (angle_diff2_ > 180) ? (360 - angle_diff2_): angle_diff2_;
            pixel.h = (dist_to_blue_ < dist_to_orange_) ? 216: 11;
        }
    }
    return image;
}
 

/**
* Returns an immge that has been watermarked by another image.
*
* The luminance of every pixel of the second image is checked, if that
* pixel's luminance is 1 (100%), then the pixel at the same location on
* the first image has its luminance increased by 0.2.
*
* @param firstImage  The first of the two PNGs to be averaged together.
* @param secondImage The second of the two PNGs to be averaged together.
*
* @return The watermarked image.
*/
PNG watermark(PNG firstImage, PNG secondImage) {
    for (unsigned x = 0; x < secondImage.width(); x++) {
        if (x > firstImage.width())
            break;
        for (unsigned y = 0; y < secondImage.height(); y++) {
            if (y > firstImage.height())
                break;
            HSLAPixel &water_pixel_ = secondImage.getPixel(x, y);
            HSLAPixel &image_pixel_ = firstImage.getPixel(x, y);
            if ((int) water_pixel_.l == 1) {
                image_pixel_.l += 0.2;
                if (image_pixel_.l > 1)
                    image_pixel_.l = 1;
            }
        }
    }
    return firstImage;
}
