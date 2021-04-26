#include "cs225/PNG.h"
using cs225::PNG;

#include "cs225/HSLAPixel.h"
using cs225::HSLAPixel;

#include <string>



void rotate(std::string inputFile, std::string outputFile) {
  PNG inputImage;
  inputImage.readFromFile(inputFile);
  int imgWidth = inputImage.width();
  int imgHeight = inputImage.height();
  PNG rotatedImage(imgWidth, imgHeight);
  for (unsigned i = 0; i < inputImage.width(); i++) {
      for (unsigned j = 0; j < inputImage.height(); j++) {
          HSLAPixel &pixel1 = rotatedImage.getPixel(imgWidth - i - 1, imgHeight - j - 1);
          HSLAPixel &pixel2 = inputImage.getPixel(i, j);
          pixel1.h = pixel2.h;
          pixel1.l = pixel2.l;
          pixel1.s = pixel2.s;
          pixel1.a = pixel2.a;
      }
  }

  rotatedImage.writeToFile(outputFile);
}

PNG myArt(unsigned int width, unsigned int height) {
  PNG png(width, height);
  for (unsigned i = 0; i < width; i++) {
      for (unsigned j = 0; j < height; j++) {
          HSLAPixel &pixel = png.getPixel(i, j);
          if (i % 2 == 0) {
              pixel.h = 35;
              pixel.s = 1;
              pixel.a = 1;
              pixel.l = 0.5;
          }
          else if (i % 3 == 0) {
              pixel.h = 267;
              pixel.s = 1;
              pixel.a = 1;
              pixel.l = 0.6;
          }
          else {
              pixel.h = 150;
              pixel.s = 1;
              pixel.a = 1;
              pixel.l = 0.6;
          }
      }
  }
  return png;
}
