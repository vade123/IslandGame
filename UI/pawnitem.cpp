/* file: pawnitem.cpp
 * description: Implementation for the class PawnItem.
 */

#include "pawnitem.hh"
#include "player.hh"

namespace Student {

PawnItem::PawnItem(QSize size, int ownerId, int pawnId, HexItem* parentHex) :
    GamePixmapItem ("pawn", pawnId, parentHex),
    size_(size),
    ownerId_(ownerId)
{
    setPawnPixmap(false);
    setZValue(10);
    setTransformationMode(Qt::SmoothTransformation);
}

void PawnItem::setPawnPixmap(bool selected)
{
    if (selected) {
        setPixmap(QPixmap(":/pawn-images/pawn-image-selected.png").
                        scaled(size_, Qt::KeepAspectRatio));
    } else {
        QPixmap pixmap;
        switch(ownerId_) {
        case 1:
            pixmap = QPixmap(":/pawn-images/pawn-image1.png").
                    scaled(size_, Qt::KeepAspectRatio);
            break;
        case 2:
            pixmap = QPixmap(":/pawn-images/pawn-image2.png").
                    scaled(size_, Qt::KeepAspectRatio);
            break;
        case 3:
            pixmap = QPixmap(":/pawn-images/pawn-image3.png").
                    scaled(size_, Qt::KeepAspectRatio);
            break;
        case 4:
            pixmap = QPixmap(":/pawn-images/pawn-image4.png").
                    scaled(size_, Qt::KeepAspectRatio);
            break;
        case 5:
            pixmap = QPixmap(":/pawn-images/pawn-image5.png").
                    scaled(size_, Qt::KeepAspectRatio);
            break;
        case 6:
            pixmap = QPixmap(":/pawn-images/pawn-image6.png")
                    .scaled(size_, Qt::KeepAspectRatio);
            break;
        }
        setItemPixmap(pixmap);
    }
}

}
