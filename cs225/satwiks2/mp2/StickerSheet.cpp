#include "StickerSheet.h"

StickerSheet::StickerSheet(const Image &picture, unsigned max) {
    base = picture;
    max_img = max;
    stickers = new Image[max];
    img_count = 0;
}


StickerSheet::StickerSheet(const StickerSheet &other) {
    base = other.base;
    max_img = other.max_img;
    stickers = new Image[max_img];
    img_count = other.img_count;
    x_y = other.x_y;
    for (unsigned i = 0; i < max_img; i++) {
        stickers[i] = other.stickers[i];
    }
}

StickerSheet::~StickerSheet() {
    delete[] stickers;
    x_y.clear();
}

const StickerSheet& StickerSheet::operator=(const StickerSheet &other) {
    delete[] stickers;
    stickers = new Image[other.max_img];
    base = other.base;
    max_img = other.max_img;
    img_count = other.img_count;
    x_y = other.x_y;
    for (unsigned i = 0; i < max_img; i++)
        stickers[i] = other.stickers[i];

    return *this;

}

void StickerSheet::changeMaxStickers(unsigned max) {
    Image* temp = new Image[max];
    max_img = max;
    for (unsigned i =0; i < std::min(img_count, max); i++) {
        temp[i] = stickers[i];
    }
    delete[] stickers;
    stickers = temp;
    if (max < img_count) {
        x_y.resize(max);
        img_count = max;
    }
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
    if (img_count == max_img)
        return -1;
    stickers[img_count] = sticker;
    img_count++;
    pair<unsigned, unsigned> p(x,y);
    x_y.push_back(p);
    return img_count-1;
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {
    if ( index >= img_count)
        return false;
    pair<unsigned, unsigned> p(x,y);
    x_y[index] = p;
    return true;
}

void StickerSheet::removeSticker(unsigned index) {
    //deae stickers[inde];;
    for (unsigned i = index; i < img_count - 1; i++) {
       stickers[i] = stickers[i+1];
       x_y[i] = x_y[i+1];
    }
    x_y.pop_back();
    img_count--;

}
Image* StickerSheet::getSticker(unsigned index) const {
    if (index >=img_count)
        return NULL;
    Image *pointer = &stickers[index];
    return pointer;
}

Image StickerSheet::render() const {
    Image sheet = base;

    for (unsigned i = 0; i < img_count; i++) {
        Image sticker = stickers[i];
        unsigned x_cord = x_y[i].first;
        unsigned y_cord = x_y[i].second;

        unsigned max_width =  std::max(sheet.width(), sticker.width() + x_cord);
        unsigned max_height = std::max(sheet.height(), sticker.height() + y_cord);

        sheet.resize(max_width, max_height);
        //if (x_cord + sticker.width() >= sheet.width())
          //  sheet.resize(base.width() + (sticker.width() - (sheet.width() - x_cord)), sheet.height());
        //if (y_cord + sticker.height() + (sticker.height() - (sheet.height() - y_cord)))
          //  sheet.resize(sheet.width(), sheet.height() + (sticker.height() - (sheet.height() - y_cord)));

        for (unsigned x = 0; x < sticker.width(); x++) {
            for (unsigned y = 0; y < sticker.height(); y++) {
                if (sticker.getPixel(x,y).a != 0) {
                    HSLAPixel &pixel = sheet.getPixel(x+x_cord, y+y_cord);
                    pixel = sticker.getPixel(x,y);
                }
            }
        }
    }
    return sheet;
}
