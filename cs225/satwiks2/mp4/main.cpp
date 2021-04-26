
#include "cs225/PNG.h"
#include "FloodFilledImage.h"
#include "Animation.h"

#include "imageTraversal/DFS.h"
#include "imageTraversal/BFS.h"

#include "colorPicker/RainbowColorPicker.h"
#include "colorPicker/GradientColorPicker.h"
#include "colorPicker/GridColorPicker.h"
#include "colorPicker/SolidColorPicker.h"
#include "colorPicker/MyColorPicker.h"

using namespace cs225;

int main() {

  // @todo [Part 3]
  // - The code below assumes you have an Animation called `animation`
  // - The code provided below produces the `myFloodFill.png` file you must
  //   submit Part 3 of this assignment -- uncomment it when you're ready.

  PNG image;
  image.readFromFile("pacman-solid-bfs-2.png");
  FloodFilledImage image_(image);
  DFS dfs(image, Point(0,0), 0.05);
  RainbowColorPicker cherry(42);

  BFS bfs(image, Point(42,69), 0.07);
  MyColorPicker xd;

  image_.addFloodFill(dfs, cherry);
  image_.addFloodFill(bfs, xd);

  Animation animation = image_.animate(1004);
  
  PNG lastFrame = animation.getFrame( animation.frameCount() - 1 );
  lastFrame.writeToFile("myFloodFill.png");
  animation.write("myFloodFill.gif");


  return 0;
}
