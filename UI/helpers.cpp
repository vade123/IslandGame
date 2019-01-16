/* file: helpers.cpp
 * description: Implementation for the helper functions.
 */

#include "helpers.hh"

namespace Student {

QPointF hexCorner(int size, int i)
{
    double angleDeg = (60 * i) - 30;
    double angleRad = (M_PI / 180) * angleDeg;
    QPointF point;
    point.setX(size * cos(angleRad));
    point.setY(size * sin(angleRad));
    return point;
}

QPolygonF hexagonCorners(int size)
{
    QPolygonF points;
    for (int i = 0; i < 6; ++i){
        points << hexCorner(size, i);
    }
    return points;
}

QPointF cubeToPixel(Common::CubeCoordinate cube, int size)
{
    qreal x = cube.x;
    qreal y = cube.z;
    x = size * ((sqrt(3) * x)  +  ((sqrt(3)/2) * y));
    y = size * ((3./2) * y);
    return QPointF(x, y);
}

}
