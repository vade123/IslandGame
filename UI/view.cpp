/* file: view.cpp
 * description: Implementation for the class View.
 */

#include "view.hh"

namespace Student {

View::View() :
    pan_(false),
    panStartX_(0),
    panStartY_(0)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Things look better when drawn big and scaled down.
    scale(0.25, 0.25);
}

void View::wheelEvent(QWheelEvent *event)
{
    // How much zooming with one mouse wheel tick
    static const double zoomFactor = 1.15;

    // Boundaries for zooming
    static const double zoomMin = 0.1;
    static const double zoomMax = 100;

    // Current zoom for checking boundaries
    static double currentZoom = 1.0;

    QPointF oldPos = mapToScene(event->pos());

    if ((event->delta() > 0) && (currentZoom < zoomMax)) {
        scale(zoomFactor, zoomFactor);
        currentZoom *= zoomFactor;
    } else if (currentZoom > zoomMin) {
        scale(1 / zoomFactor, 1 / zoomFactor);
        currentZoom /= zoomFactor;
    } else {
        // If the boundaries are not met, do nothing.
    }
    QPointF newPos = mapToScene(event->pos());
    QPointF delta = newPos - oldPos;
    translate(delta.x(), delta.y());

}

void View::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        pan_ = true;
        panStartX_ = event->x();
        panStartY_ = event->y();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void View::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        pan_ = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    if (pan_)
    {
        horizontalScrollBar()->setValue(
                    horizontalScrollBar()->value() - (event->x() - panStartX_));
        verticalScrollBar()->setValue(
                    verticalScrollBar()->value() - (event->y() - panStartY_));
        panStartX_ = event->x();
        panStartY_ = event->y();
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}
}
