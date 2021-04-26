#include "Image.h"
#include "StickerSheet.h"

int main() {



    Image a,b,c,d;

    a.readFromFile("img3.png");
    b.readFromFile("img2.png");
    c.readFromFile("alma.png");
    c.scale(0.15);
    d.readFromFile("img4.png");

    Image alma;
    //alma.scale(2000,2000);
    alma.readFromFile("alma.png");

    StickerSheet page(alma, 5);

    page.addSticker(a, 1, 1);
    page.addSticker(b, 500, 200);
    page.addSticker(c, 100, 400);
    page.addSticker(d, 200, 0);

    Image ret = page.render();
    ret.writeToFile("myImage.png");
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //



    return 0;
}
