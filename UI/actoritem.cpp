/* file: actoritem.cpp
 * description: Implementation for the class ActorItem.
 */

#include "actoritem.hh"

namespace Student {

ActorItem::ActorItem(QSize size, int actorId, std::string actorType,
                     HexItem *parentHex) :
    GamePixmapItem ("actor", actorId, parentHex),
    size_(size),
    actorType_(actorType)
{
    setActorPixmap(false);
    setZValue(10);
}

void ActorItem::setActorPixmap(bool selected)
{
    // vortex doesnt have selected option
    if (actorType_ == "vortex") {
        setItemPixmap(QPixmap(":/actor-images/vortex.png").
                  scaled(3*size_, Qt::KeepAspectRatio));
        changeType("vortex");
    } else {
        // set pixmap to be highlighted (white)
        if (selected) {
            if (actorType_ == "shark") {
                setPixmap(QPixmap(":/actor-images/shark-selected.png").
                        scaled(size_, Qt::KeepAspectRatio));
            }
            else if (actorType_ == "kraken") {
                setPixmap(QPixmap(":/actor-images/kraken-selected.png").
                        scaled(size_, Qt::KeepAspectRatio));
            }
            else if (actorType_ == "seamunster") {
                setPixmap(QPixmap(":/actor-images/seamunster-selected.png").
                        scaled(size_, Qt::KeepAspectRatio));
            } else {
                // Game doesn't know the actor
                throw std::invalid_argument("invalid actor type");
            }
        } else {
            QPixmap pixmap;
            if (actorType_ == "shark") {
                pixmap = QPixmap(":/actor-images/shark.png").
                        scaled(size_, Qt::KeepAspectRatio);
            }
            else if (actorType_ == "kraken") {
                pixmap = QPixmap(":/actor-images/kraken.png").
                        scaled(size_, Qt::KeepAspectRatio);
            }
            else if (actorType_ == "seamunster") {
                pixmap = QPixmap(":/actor-images/seamunster.png").
                        scaled(size_, Qt::KeepAspectRatio);
            } else {
                // Game doesn't know the actor
                throw std::invalid_argument("invalid actor type");
            }
            setItemPixmap(pixmap);
        }
    }
}

}
