/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once
#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <cmath>

using namespace cs225;
class Image : public PNG {
    private:
        //void expand (double`);
        //void shrink (double);
    public:
        //void lighten();
        void lighten(double = 0.1);
        //void darken();
        void darken(double = 0.1);
        //void saturate();
        void saturate(double = 0.1);
        //void desaturate();
        void desaturate(double = 0.1);
        void grayscale();
        void rotateColor(double);
        void illinify();
        void scale(double);
        void scale(unsigned, unsigned);
};

#endif
