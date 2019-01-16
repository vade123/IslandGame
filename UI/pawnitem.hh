/* file: pawnitem.hh
 * description: Header for class PawnItem. Class is used for drawing pawns for
 * the gameboard.
 */

#ifndef PAWNITEM_HH
#define PAWNITEM_HH

#include "gamepixmapitem.hh"
#include "hexitem.hh"
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace Student {

class PawnItem : public GamePixmapItem
{
    Q_OBJECT
public:
    /**
     * @brief Constructor, sets pawnItems size, its owners id, pawns id and parent hex
     * @param size Pixmaps size
     * @param ownerId Identification of the player who owns the pawn
     * @param pawnId Identification of the pawn
     * @param parentHex Pointer to the hex where the pawn is located
     */
    PawnItem(QSize size, int ownerId, int pawnId, HexItem *parentHex = nullptr);
    ~PawnItem() = default;

    /**
     * @brief setPawnPixmap Sets pawnItems pixmap to correct color, or white if selected
     * @param selected True, if the pawnItem is selected
     */
    void setPawnPixmap(bool selected);

private:  
    QSize size_;
    int ownerId_;
};

}
#endif // PAWNITEM_HH
