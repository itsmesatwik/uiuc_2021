/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#pragma once
#ifndef STICKERSHEET_H
#define STICKERSHEET_H
#include <iostream>
#include "Image.h"
#include <utility>
#include <vector>
#include <algorithm>

using std::vector;
using std::pair;

class StickerSheet {
    private:
        Image base;
        Image* stickers;
        unsigned max_img;
        unsigned img_count;
        vector<pair<unsigned, unsigned>> x_y;

    public:
        StickerSheet(const Image&, unsigned max);
        StickerSheet(const StickerSheet&);
        ~StickerSheet();
        const StickerSheet& operator=(const StickerSheet &other);
        void changeMaxStickers(unsigned);
        int addSticker(Image &, unsigned, unsigned);
        bool translate(unsigned, unsigned, unsigned);
        void removeSticker(unsigned);
        Image* getSticker(unsigned) const;
        Image render() const;

};

#endif
 
