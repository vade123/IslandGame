/* file: hexitem.hh
 * description: Header for class GamePixmapItem.
 */

#ifndef GAMEPIXMAPITEM_HH
#define GAMEPIXMAPITEM_HH

#include "hexitem.hh"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>

namespace Student {

/**
 * @brief Base class for different game items that use pixmap as their
 * game icons. Items can be selected with a mouse click and moved to
 * another location (hex) on the gameboard.
 */
class GamePixmapItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:

    /**
     * @brief Constructor, sets the type and id of the item
     */
    GamePixmapItem(std::string type, int id, HexItem *parentHex = nullptr);

    /**
     * @brief Virtual destructor
     */
    virtual ~GamePixmapItem() = default;

    /**
     * @brief mousePressEvent emits selected-signal with item's id
     * @param event QGraphicsScene's mousePressEvent
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);

    /**
     * @brief setLocationOnBoard moves the item from source hex to target hex
     * @param source current location of the item
     * @param target location to be moved into
     * @param scene QGraphicsScene where items are added/removed from
     * @param i index of the slot where location is set (if not given,
     *        location will be set on the first unoccupied slot)
     */
    virtual void setLocationOnBoard(HexItem *source, HexItem *target,
                                    QGraphicsScene *scene, int i = -1);

    /**
     * @brief setItemPixmap sets items pixmap to given parameter
     * @param pixmap new pixmap
     */
    virtual void setItemPixmap(QPixmap pixmap);

    /**
     * @brief changeType changes the type of the item
     * @param type to be changed to
     */
    virtual void changeType(std::string type);

    /**
     * @brief currentSlot returns the current slot of the item in its parent hex
     * @return currentSlot_
     */
    virtual int currentSlot();

signals:
    void selected(int id);

private:
    std::string type_;
    int id_;
    Student::HexItem *parentHex_;
    QPixmap pixmap_;
    int currentSlot_;
};

}
#endif // GAMEPIXMAPITEM_HH
