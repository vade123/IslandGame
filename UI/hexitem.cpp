/* file: hexitem.cpp
 * description: Implementation for the class HexItem.
 */

#include "hexitem.hh"

namespace Student {

HexItem::HexItem(Common::CubeCoordinate cubeCoords, QPointF pixelCoords,
                 int size, QPen pen) :
    cubeCoords_(cubeCoords),
    pixelCoords_(pixelCoords),
    size_(size),
    pen_(pen),
    slotAmount_(3),
    highlighted_(false)
{
    setPen(pen);
    initializeSlots();
    setAcceptHoverEvents(true);
}

Common::CubeCoordinate HexItem::getCubeCoords()
{
    return cubeCoords_;
}

QPointF HexItem::getPixelCoords()
{
    return pixelCoords_;
}

void HexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        setPen(pen_);
        setZValue(0);
        emit clicked(cubeCoords_);
        QGraphicsPolygonItem::mousePressEvent(event);
    }
}

void HexItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    // higlighted_ is set true when:
    //  (1) pawn is selected and player has to pick a hex where to move the pawn
    //  (2) on game phase SINKING
    if (highlighted_ && isUnderMouse()) {
        QPen pen(Qt::red);
        pen.setWidth(10);
        setPen(pen);
        setZValue(5);
    }
    emit hover(cubeCoords_);
    QGraphicsPolygonItem::hoverEnterEvent(event);
}

void HexItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // return the basic drawing settings of the hex
    if (!isUnderMouse()) {
        setPen(pen_);
        setZValue(0);
    }
    QGraphicsPolygonItem::hoverLeaveEvent(event);
}

bool HexItem::slotFull(std::string type, int i)
{
    if (type == "pawn") {
        return pawnSlots_.at(i).second;
    }
    else if (type == "actor") {
        return actorSlots_.at(i).second;
    }
    else if (type == "dolphin") {
        return dolphinSlots_.at(i).second;
    }
    else {
        return boatSlot_.second;
    }
}

void HexItem::changeSlotOccupation(std::string type, int i)
{
    if (type == "pawn") {
        if (slotFull(type, i)) {
            pawnSlots_.at(i).second = false;
            return;
        }
        pawnSlots_.at(i).second = true;
    }

    else if (type == "actor") {
        if (slotFull(type, i)) {
            actorSlots_.at(i).second = false;
            return;
        }
        actorSlots_.at(i).second = true;
    }

    else if (type == "dolphin") {
        if (slotFull(type, i)) {
            dolphinSlots_.at(i).second = false;
            return;
        }
        dolphinSlots_.at(i).second = true;
    }

    else if (type == "boat") {
        if (boatSlot_.second) {
            boatSlot_.second = false;
            return;
        }
        boatSlot_.second = true;
    }
    else {
        // type is vortex, do nothing.
    }
}

QPointF HexItem::getSlot(std::string type, int i)
{
    if (type == "pawn") {
        return pawnSlots_.at(i).first;
    }

    else if (type == "actor") {
        return actorSlots_.at(i).first;
    }

    else if (type == "dolphin") {
        return dolphinSlots_.at(i).first;
    }

    else if (type == "boat") {
        return boatSlot_.first;
    }

    else {
        return vortexSlot_;
    }
}

int HexItem::transportSlotsFull()
{
    if (boatSlot_.second) {
        return slotAmount_;
    }
    int i = 0;
    for (auto slot : dolphinSlots_) {
        if (slot.second.second) {
            ++i;
        }
    }
    return i;
}

void HexItem::setHiglighted(bool value)
{
    highlighted_ = value;
}

void HexItem::initializeSlots()
{
    qreal a = static_cast<qreal>(size_)/6;
    qreal b = (a/2.)*3;
    setPawnSlots(a);
    setActorSlots(a,b);
    setDolphinSlots(a,b);
    boatSlot_ = { QPointF(pixelCoords_.rx()-(3*b)-a,
                  pixelCoords_.ry()-(2*a)-(b/2)), false };
    vortexSlot_ = QPointF(pixelCoords_.rx()-(4*a)-b, pixelCoords_.ry()-(2*a));
}

void HexItem::setPawnSlots(qreal a)
{
    for (int i = 0; i < slotAmount_; ++i) {
        QPointF coord;
        switch(i) {
        case 0:
            // middle pawn slot
            coord = QPointF(pixelCoords_.rx()-a, pixelCoords_.ry()-a);
            break;
        case 1:
            // right pawn slot
            coord = QPointF(pixelCoords_.rx()+(2*a), pixelCoords_.ry()-a);
            break;
        case 2:
            // left pawn slot
            coord = QPointF(pixelCoords_.rx()-(4*a), pixelCoords_.ry()-a);
            break;
        }
        pawnSlots_[i] = {coord, false};
    }
}

void HexItem::setActorSlots(qreal a, qreal b)
{
    for (int i = 0; i < slotAmount_; ++i) {
        QPointF coord;
        switch(i) {
        case 0:
            // middle actor slot
            coord = QPointF(pixelCoords_.rx()-b, pixelCoords_.ry()-(5*a));
            break;
        case 1:
            // right actor slot
            coord = QPointF(pixelCoords_.rx()+b, pixelCoords_.ry()-(2*b));
            break;
        case 2:
            // left actor slot
            coord = QPointF(pixelCoords_.rx()-(3*b), pixelCoords_.ry()-(2*b));
            break;
        }
        actorSlots_[i] = {coord, false};
    }
}

void HexItem::setDolphinSlots(qreal a, qreal b)
{
    for (int i = 0; i < slotAmount_; ++i) {
        QPointF coord;
        switch(i) {
        case 0:
            // middle dolphin slot
            coord = QPointF(pixelCoords_.rx()-b, pixelCoords_.ry()+a);
            break;
        case 1:
            // right dolphin slot
            coord = QPointF(pixelCoords_.rx()+b, pixelCoords_.ry()+a);
            break;
        case 2:
            // left dolphin slot
            coord = QPointF(pixelCoords_.rx()-(3*b), pixelCoords_.ry()+a);
            break;
        }
        dolphinSlots_[i] = {coord, false};
    }
}

}
