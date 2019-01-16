/* file: gamepixmapitem.cpp
 * description: Implementation for the class GamePixmapItem.
 */

#include "gamepixmapitem.hh"

namespace Student {

GamePixmapItem::GamePixmapItem(std::string type, int id, HexItem *parentHex) :
    type_(type),
    id_(id),
    parentHex_(parentHex),
    currentSlot_(0)
{
}

void GamePixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        emit selected(id_);
        event->accept();
    }
}


void GamePixmapItem::setLocationOnBoard(HexItem *source, HexItem *target,
                                        QGraphicsScene *scene, int i)
{
    // selecting the same hex as the source does nothing
    if (source == target) {
        return;
    }

    // remove the item from the previous location
    if (parentHex_ != nullptr) {
        scene->removeItem(this);
    }

    // no slot-index given
    if (i < 0) {
        // check if there's available item slots on the target hex
        // and set new position and parent hex for the item (if all
        // slots are occupied, nothing is changed)
        for (int j = 0; j < 3; ++j) {
            if (!target->slotFull(type_, j)) {
                setPos(target->getSlot(type_, j));
                target->changeSlotOccupation(type_, j);
                if (source != nullptr) {
                    source->changeSlotOccupation(type_, currentSlot_);
                }
                parentHex_ = target;
                currentSlot_ = j;
                break;
            }
        }
    }
    // set location on the given slot-index if possible
    else {
        if (!target->slotFull(type_, i)) {
            setPos(target->getSlot(type_, i));
            target->changeSlotOccupation(type_, i);
            if (source != nullptr) {
                source->changeSlotOccupation(type_, currentSlot_);
            }
            parentHex_ = target;
            currentSlot_ = i;
        }
    }

    // restore items pixmap to normal and re-add item to the scene
    setPixmap(pixmap_);
    scene->addItem(this);
}

void GamePixmapItem::setItemPixmap(QPixmap pixmap)
{
    pixmap_ = pixmap;
    setPixmap(pixmap);
}

void GamePixmapItem::changeType(std::string type)
{
    type_ = type;
}

int GamePixmapItem::currentSlot()
{
    return currentSlot_;
}

}
