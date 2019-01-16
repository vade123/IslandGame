/* file: helpers.hh
 * description: Header for helper-functions used in calculating the hexes.
 */

#ifndef HELPERS_HH
#define HELPERS_HH

#include "cubecoordinate.hh"
#include <QPointF>
#include <QPolygonF>
#include <QtMath>

namespace Student {

/**
 * @brief hexCorner Calculates a hexagons corner assuming center is at (0, 0)
 * @param size Hexagons radius from center to a corner in pixels
 * @param i Number of the corner
 * @return Location of a hexagons corner in pixel coordinates
 */
QPointF hexCorner(int size, int i);

/**
 * @brief hexagonCorners Calls hexCorner and adds the corners to a container
 * @param size Hexagons radius from center to a corner in pixels
 * @return Locations of all six corners of a hexagon
 */
QPolygonF hexagonCorners(int size);

/**
 * @brief cubeToPixel Convers cube coordinates to pixel coordinates
 * @param cube Hexagons coordinates in cube coordinates
 * @param size Hexagons radius from center to a corner in pixels
 * @return Hexagons coordinates in pixel coordinates
 */
QPointF cubeToPixel(Common::CubeCoordinate cube, int size);

}

#endif // HELPERS_HH
