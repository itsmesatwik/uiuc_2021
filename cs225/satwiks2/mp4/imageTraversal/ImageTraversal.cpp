#include <cmath>
#include <iterator>
#include <iostream>

#include "../cs225/HSLAPixel.h"
#include "../cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator(Point p) {
  current = p;
  /** @todo [Part 1] */
}

ImageTraversal::Iterator::Iterator(ImageTraversal* traversal_, PNG image_, Point start_, double tolerance_) : traversal(traversal_) {
    tolerance = tolerance_;
    start = start_;
    image = image_;
    current = start_;
    for (int i = 0; i < int(image.width()); i++) {
      std::vector<bool> vec;
      for (int j = 0; j < int(image.height()); j++) {
        vec.push_back(false);
      }
      visited.push_back(vec);
    }
    visited[start.x][start.y] = true;
}
/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  //std::cout<<"Segfault_delta2"<<std::endl;
  // if (traversal->empty()) {
  //   return *this;
  // }
  if (traversal == NULL) {
    return *this;
  }
  if (!traversal->empty()){
    traversal->pop();
    if (current.x < image.width() - 1) {
      Point right(current.x+1, current.y);
      isValidPoint(right);
    }
    if (current.y < image.height() - 1) {
      Point below(current.x, current.y+1);
      isValidPoint(below);
    }
    if (current.x > 0) {
      Point left(current.x-1, current.y);
      isValidPoint(left);
    }
    if (current.y > 0) {
      Point above(current.x, current.y-1);
      isValidPoint(above);
    }
    //std::cout<<current.x<<current.y<<std::endl;
    current = traversal->peek();
    while (!traversal->empty() && visited[current.x][current.y]) {
      //std::cout<<"Segfault_while"<<std::endl;
      traversal->pop();
      //std::cout<<"Segfault_dead"<<std::endl;
      current = traversal->peek();
      //std::cout<<"Segfault_dying"<<std::endl;
    }
    // if (traversal->empty()) {
    //   current = Point(image.width(), image.height());
    //   traversal->pu
    // }

if (current.x != image.width()){
  //std::cout<<"Segfault_after_dying1"<<std::endl;
    visited[current.x][current.y] = true;
    //std::cout<<"Segfault_after_dying2"<<std::endl;
}
//std::cout<<"Segfault_after_dying3"<<std::endl;

  }
  /** @todo [Part 1] */
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {

  return current;
  /** @todo [Part 1] */
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  //std::cout<<"Segfault_delta1"<<std::endl;
  /** @todo [Part 1] */
  //std::cout<<"Segfault_noteq"<<std::endl;
  if (traversal == NULL && other.traversal == NULL) {
    return false;
  }
  else if ((traversal!= NULL && other.traversal != NULL) && current == other.current) {
    //std::cout<<"Segfault4"<<std::endl;
    return false;
  }
  else if (traversal == NULL && other.traversal->empty()) {
    //std::cout<<"Segfault2"<<std::endl;
    return false;
  }
  else if (traversal->empty() && other.traversal == NULL) {
    //std::cout<<"Segfault3"<<std::endl;
    return false;
  }
  else if (traversal->empty() && other.traversal->empty()) {
    //std::cout<<"Segfault1"<<std::endl;
    return false;
  }
  //std::cout<<"Segfault5"<<std::endl;
  return true;
}

void ImageTraversal::Iterator::isValidPoint(Point p) {
  if (traversal == NULL) {
    return;
  }
  if (visited[p.x][p.y]) {
    return;
  }
  HSLAPixel p1 = image.getPixel(p.x,p.y);
  HSLAPixel p2 = image.getPixel(start.x, start.y);
  //std::cout<<"Segfault_delta"<<std::endl;
  double del = calculateDelta(p1,p2);
  //std::cout<<"Segfault_delta"<<std::endl;
  if (del < tolerance) {
    //std::cout<<"Segfault_kek"<<std::endl;
    traversal->add(p);
  }
  return;
}
