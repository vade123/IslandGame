/* file: transportitem.hh
 * description: Header for class TransportItem. Class is used for drawing
 * transports for the game.
 */

#ifndef TRANSPORTITEM_HH
#define TRANSPORTITEM_HH

#include "gamepixmapitem.hh"
#include "hexitem.hh"
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Student {

class TransportItem : public GamePixmapItem
{
    Q_OBJECT
public:
    /**
     * @brief Constructor,  sets actorItem's size, actor's id, actor's type and parent hex
     * @param size Pixmaps size
     * @param actorId Identification of the actor
     * @param actorType Type of the actor
     * @param parentHex Pointer to the hex where the actor is located
     */
    TransportItem(QSize size, int transportId, std::string transportType,
                  HexItem *parentHex = nullptr);
    ~TransportItem() = default;

    /**
     * @brief setTransportPixmap sets the correct pixmap for the item, depending
     *        on the transportType_ of the item and given parameter
     * @param selected: if true, pixmap will be highlighted (white)
     */
    void setTransportPixmap(bool selected);

    /**
     * @brief getType returns the type of the transport
     * @return transportType_;
     */
    std::string getType();

private:
    QSize size_;
    std::string transportType_;
};

}

#endif // TRANSPORTITEM_HH
