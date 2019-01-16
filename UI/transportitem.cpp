/* file: transportitem.cpp
 * description: Implementation for the class TransportItem.
 */

#include "transportitem.hh"

namespace Student {

TransportItem::TransportItem(QSize size, int transportId,
                             std::string transportType,
                             HexItem *parentHex) :
    GamePixmapItem (transportType, transportId, parentHex),
    size_(size),
    transportType_(transportType)
{
    setTransportPixmap(false);
    setZValue(15);
}

void TransportItem::setTransportPixmap(bool selected)
{
    if (selected) {
        if (transportType_ == "dolphin") {
            setPixmap(QPixmap(":/transport-images/dolphin-selected.png").
                      scaled(size_, Qt::KeepAspectRatio));
        }
        else if (transportType_ == "boat") {
            setPixmap(QPixmap(":/transport-images/boat-selected.png").
                      scaled(size_, Qt::KeepAspectRatio));
        } else {
            // Game doesn't know the transport
            throw std::invalid_argument("invalid transport type");
        }
    } else {
        QPixmap pixmap;
        if (transportType_ == "dolphin") {
            pixmap = QPixmap(":/transport-images/dolphin.png").
                    scaled(size_, Qt::KeepAspectRatio);
        }
        else if (transportType_ == "boat") {
            pixmap = QPixmap(":/transport-images/boat.png").
                    scaled(size_, Qt::KeepAspectRatio);
        } else {
            // Game doesn't know the transport
            throw std::invalid_argument("invalid transport type");
        }
        setItemPixmap(pixmap);
    }
}

std::string TransportItem::getType()
{
    return transportType_;
}

}
