
#ifndef _MAZE_H
#define _MAZE_H
#include <vector>
#include <queue>
#include <stack>
#include "cs225/PNG.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <map>
#include "dsets.h"

using namespace std;
using namespace cs225;


class SquareMaze {

      public:
                SquareMaze();
                    void makee(int width, int height);
                        bool canTra(int x, int y, int dir) const;
                            void setWall(int x, int y, int dir, bool exists);
                                vector<int> solveMaze();
                                    PNG* drawMaze() const;
                                        PNG* drawMazeWithSolution();

                                            typedef pair<int, int> map_cell;

                                              private:
                                                map_cell findEnd(map<map_cell, int> distances);
                                                    vector<int> getSolutionPath(map_cell start, map_cell end,
                                                                    map<map_cell, map_cell> paths);

                                                        vector<bool> right_walls_;
                                                            vector<bool> bottom_walls_;

                                                                int width_;
                                                                    int height_;

                                                                        vector<int> solution_path_;
};

#endif
