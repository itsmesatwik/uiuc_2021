#include "Image.h"
/*
void Image::lighten() {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            HSLAPixel &pixel = getPixel(i, j);
            pixel.l = (pixel.l + 0.1 > 1) ? 1.0 : (pixel.l + 0.1);
        }
    }
}
*/

void Image::lighten(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            HSLAPixel &pixel = getPixel(i,j);
            pixel.l = (pixel.l + amount > 1) ? 1.0 : (pixel.l + amount);
        }
    }
}

void Image::darken(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            HSLAPixel &pixel = getPixel(i, j);
            pixel.l = (pixel.l - amount < 0)? 0 : (pixel.l - amount);
        }
    }
}

/*
void Image::darken(double amount) {
}
*/

/*void Image::saturate() {

}*/

void Image::saturate(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            HSLAPixel &pixel = getPixel(i, j);
            pixel.s = (pixel.s + amount > 1)? 1 : (pixel.s + amount);
        }
    }
}

/*
void Image::desaturate() {
}*/

void Image::desaturate(double amount) {
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            HSLAPixel &pixel = getPixel(i, j);
            pixel.s = (pixel.s - amount < 0) ? 0 : (pixel.s - amount);  
        }
    }
}

void Image::grayscale() {

    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel & pixel = getPixel(x, y); 
            pixel.s = 0;
        }
    }
}
void Image::rotateColor(double degrees) {
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel &pixel = getPixel(x, y);
            if (degrees < 0)
                pixel.h = (pixel.h - degrees < 0)? (360 - (degrees - pixel.h)) : (pixel.h - degrees);
            else 
                pixel.h = (pixel.h + degrees > 360)? (0 + (pixel.h + degrees - 360)) : (pixel.h + degrees);
        }
    }
}

void Image::illinify() {
    for (unsigned x = 0; x < width(); x++) {
        for (unsigned y = 0; y < height(); y++) {
            HSLAPixel &pixel = getPixel(x,y);
            double angle_diff1_ = std::abs((pixel.h - 11));
            double angle_diff2_ = std::abs((pixel.h - 216));
            double dist_to_orange_ = (angle_diff1_ > 180) ? (360 - angle_diff1_): angle_diff1_;
            double dist_to_blue_ = (angle_diff2_ > 180) ? (360 - angle_diff2_): angle_diff2_;
            pixel.h = (dist_to_blue_ < dist_to_orange_) ? 216: 11;
        }
    }
}

void Image::scale(double factor) {
    if (factor == 1)
        return;
    PNG copyImage = PNG(*this);
    resize(width()*factor, height()*factor);
    for (unsigned i = 0; i < width(); i++) {
        for (unsigned j = 0; j < height(); j++) {
            HSLAPixel &origpixel = copyImage.getPixel(i/factor, j/factor);
            HSLAPixel &outpixel = getPixel(i,j);
            outpixel = origpixel;
        }
    }

}

void Image::scale(unsigned w, unsigned h) {
    scale(std::min(w/width(), h/height()));
}

/*void Image::expand(double factor) {
    PNG copyImage = PNG (*this);
    // pixel(i,j) = (pixel in copy divided by amount)
    for (unsigned x = 0; x < width(); x+=factor) {
        for (unsigned y = 0; y < height(); y+=factor) {
            for (unsigned row = x; row < x + factor; row++) {
                for (unsigned col = y; col < y+factor; col++) {

                }
            }
        }
    }
}*/
