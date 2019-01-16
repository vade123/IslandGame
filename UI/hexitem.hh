/* file: hexitem.hh
 * description: Header for class HexItem. Class is used for drawing the hexes
 * for the gameboard.
 */

#ifndef HEXITEM_HH
#define HEXITEM_HH

#include "cubecoordinate.hh"
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

namespace Student {

class HexItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    /**
     * @brief Constructor, sets coordinates and size for the hexItem.
     * @param cubeCoords coordinates in cube coordinates
     * @param pixelCoords coordinates in pixel coordinates
     * @param size hexItems size from center to a corner in pixels
     */
    HexItem(Common::CubeCoordinate cubeCoords, QPointF pixelCoords,
            int size, QPen pen);
    ~HexItem() = default;

    /**
     * @brief getCubeCoords return hexItems cube coordinates.
     * @return cube coordinates
     */
    Common::CubeCoordinate getCubeCoords();

    /**
     * @brief getPixelCoords return hexItems pixel coordinates.
     * @return pixel coordinates
     */
    QPointF getPixelCoords();

    /**
     * @brief mousePressEvent emits clicked-signal with hex's cube coordinates
     * @param event QGraphicsScene's mousePressEvent
     */
    void mousePressEvent(QGraphicsSceneMouseEvent * event);

    /**
     * @brief hoverEnterEvent emits hover-signal with hex's cube coordinates \n
     * and highlights the hex if highlighted_ is set as true
     * @param event QGraphicsScene's hoverEnterEvent
     */
    void hoverEnterEvent(QGraphicsSceneHoverEvent * event);

    /**
     * @brief hoverLeaveEvent sets hex to its normal state after being highlighted
     * @param event QGraphicsScene's hoverLeaveEvent
     */
    void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

    /**
     * @brief slotFull checks if slot is occupied
     * @param type of the slot
     * @param i map-index of the slot
     * @return true, if slot is occupied
     */
    bool slotFull(std::string type, int i);

    /**
     * @brief changeSlotOccupation changes the bool value of the slot
     * @param type of the slot
     * @param i map-index of the slot
     * @post slot's bool value (occupation) is changed
     */
    void changeSlotOccupation(std::string type, int i);

    /**
     * @brief getSlot returns the slot's pixel-coordinates
     * @param type of the slot
     * @param i map-index of the slot
     * @return slot's pixel coordinates as QPointF
     */
    QPointF getSlot(std::string type, int i);

    /**
     * @brief transportSlotsFull checks if there's room for more transports in hex
     * @return the amount of occupied slots, boat takes 3 slots
     */
    int transportSlotsFull();

    /**
     * @brief setHighlighted changes the value of highlighted_ to param value
     * @param value bool value to be changed to
     */
    void setHiglighted(bool value);

signals:
    void clicked(Common::CubeCoordinate coords);
    void hover(Common::CubeCoordinate coords);

private:

    // initializes the slots where GamePixmapItems will be drawn
    void initializeSlots();
    // sets coordinates for pawn slots
    void setPawnSlots(qreal a);
    // sets coordinates for actor slots
    void setActorSlots(qreal a, qreal b);
    // sets coordinates for dolphin slots
    void setDolphinSlots(qreal a, qreal b);

    Common::CubeCoordinate cubeCoords_;
    QPointF pixelCoords_;
    int size_;
    QPen pen_;

    int slotAmount_;
    // <slot index, <pixel-coords, occupied/not occupied>>
    std::map<int, std::pair<QPointF, bool>> pawnSlots_;
    std::map<int, std::pair<QPointF, bool>> actorSlots_;
    std::map<int, std::pair<QPointF, bool>> dolphinSlots_;
    // <pixel-coords, occupied/not occupied>
    std::pair<QPointF, bool> boatSlot_;
    // pixel-coords of the vortex slot
    QPointF vortexSlot_;

    // when true, hex's lines are drawn with red/green and thicker than normal
    bool highlighted_;
};

}

#endif // HEXITEM_HH
