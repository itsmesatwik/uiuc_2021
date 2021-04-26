/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"
//#include "cs225/RGB_HSL.h"

using namespace std;


Point<3> convertToXYZ(LUVAPixel pixel) {
    return Point<3>( pixel.l, pixel.u, pixel.v );
}

MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    /**
     * @todo Implement this function!
     */
    int row = theSource.getRows();
    int col = theSource.getColumns();
    MosaicCanvas *mosayic = new MosaicCanvas(row, col);

    vector<Point<3>> pixelPoints;
    map<Point<3>, int> pointTileMap;

                        /*
                         *     * Insert each point to the points vector and to the map
                         *         */
    for (size_t i = 0; i < theTiles.size(); i++) {
        Point<3> point = convertToXYZ(theTiles[i].getAverageColor());
        pixelPoints.push_back(point);
        pointTileMap.insert(pair<Point<3>, int>(point, i));
    }

    KDTree<3> kdTree(pixelPoints);

                                /*
                                 *     * Find the closest point of each region in our source image and map it to
                                 *         * our mosaic image
                                 *             */
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            LUVAPixel pix = theSource.getRegionColor(i, j);
            Point<3> closestPoint = kdTree.findNearestNeighbor(convertToXYZ(pix));
            size_t pos = pointTileMap[closestPoint];
            mosayic->setTile(i, j, &theTiles[pos]);
        }
    }

    return mosayic;
}

