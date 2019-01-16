/* file: actorItem.hh
 * description: Header for class ActorItem. Used for drawing actors.
 */

#ifndef ACTORITEM_HH
#define ACTORITEM_HH

#include "gamepixmapitem.hh"
#include "hexitem.hh"
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Student {

class ActorItem : public GamePixmapItem
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
    ActorItem(QSize size, int actorId, std::string actorType,
              HexItem *parentHex = nullptr);
    ~ActorItem() = default;

    /**
     * @brief setActorPixmap sets the correct pixmap for the item, depending
     *        on the actorType_ of the item and given parameter
     * @param selected: if true, pixmap will be highlighted (white)
     */
    void setActorPixmap(bool selected);

private:
    QSize size_;
    std::string actorType_;
};

}

#endif // ACTORITEM_HH
