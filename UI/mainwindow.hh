/* file: mainwindow.hh
 * description: Header for class MainWindow. Class handles everything
 * graphic and controls the game based on users actions and state of the game.
 */

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "startdialog.hh"
#include "gameboard.hh"
#include "gamestate.hh"
#include "player.hh"
#include "hexitem.hh"
#include "pawnitem.hh"
#include "actoritem.hh"
#include "transportitem.hh"
#include "initialize.hh"
#include "gameexception.hh"
#include "illegalmoveexception.hh"
#include "helpers.hh"
#include "view.hh"
#include <QMainWindow>
#include <QGraphicsScene>
#include <QPolygonF>
#include <QString>
#include <QMessageBox>

// a single hexes radius
const int HEX_SIZE = 120;

// item sizes in relation to hexes size
const QSize PAWN_PIXMAP_SIZE = QSize(HEX_SIZE/3*2, HEX_SIZE/3*2);
const QSize ACTOR_PIXMAP_SIZE = QSize(HEX_SIZE/12*7, HEX_SIZE/12*7);
const QSize BOAT_PIXMAP_SIZE = QSize(HEX_SIZE/7*13, HEX_SIZE/7*13);

namespace Ui {
class MainWindow;
}

namespace Student {

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(unsigned players, unsigned pawns,
                        QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    /**
     * @brief pawnSelected Receives pawn's id and stores it to pawnToBeMoved_.
     *        Sets a pawnItem to be selected or deselected and toggles hex
     *        highlighting.
     * @param pawnId Identifier of the clicked pawn.
     */
    void pawnSelected(int pawnId);

    /**
     * @brief actorSelected Receives actor's id and stores it to actorToBeMoved_.
     *        Sets an actorItem to be selected or deselected and toggles hex
     *        highlighting.
     * @param actorId Identifier of the clicked actor.
     */
    void actorSelected(int actorId);

    /**
     * @brief transportSelected Receives transport's if and stores it to transportToBeMoved_.
     *        Sets a transportItem to be selected or deselected and toggles hex
     *        highlighting.
     * @param transportId Identifier of the clicked transport.
     */
    void transportSelected(int transportId);

    /**
     * @brief hexClicked Receives hexes coordinates and either moves a pawn or
     *        sinks a tile depending on gamephase.
     * @param coords Coordinates of the clicked hex.
     */
    void hexClicked(Common::CubeCoordinate coords);

    /**
     * @brief toggleHexHighlighting Turns HexHighlighting on for all hexes.
     * @param value True, toggles highlighting on
     */
    void toggleHexHighlighting(bool value);

    /**
     * @brief spinWheel Receives a signal from spin button and spins the wheel.
     */
    void spinWheel();

    /**
     * @brief skipMovement Skips a GamePhase MOVEMENT or SPINNING.
     */
    void skipMovement();

private:

    /**
     * @brief setPlayers Initializes players after receiving info from dialog.
     * @param players Number of players
     * @param pawns Number of pawns per player
     */
    void setPlayers(unsigned int players, unsigned int pawns);

    /**
     * @brief drawBoard Copies the hexes from the gameboard and creates the hexItems.
     */
    void drawBoard();

    /**
     * @brief initializePawns Adds pawns to the game.
     */
    void initializePawns();

    /**
     * @brief addHexToScene Adds a single hexItem to the scene.
     * @param hex Pointer to the hexItem to be added.
     * @param brush Initialized based on the tile type to be added.
     */
    void addHexToScene(HexItem *hex, QBrush &brush);

    /**
     * @brief setBrushColor Sets brushes color based on piece type.
     * @param pieceType Type of the next hex.
     * @param brush
     */
    void setBrushColor(std::string pieceType, QBrush &brush);

    /**
     * @brief initializePawnItems Adds pawnItems on top of a hexItem.
     * @param hex Pointer to a hexItem.
     */
    void initializePawnItems(HexItem *hex);

    /**
     * @brief initializeActor Creates an actor and add an actorItem to top of a HexItem.
     * @param coords Coordinates of the hex.
     */
    void initializeActor(Common::CubeCoordinate coords);

    /**
     * @brief movePawn Moves a pawn on the gameboard.
     * @param target Coordinates of the target hex.
     */
    void movePawn(Common::CubeCoordinate target);

    /**
     * @brief moveActor Moves an actor on the gameboard.
     * @param target Coordinates of the target hex.
     */
    void moveActor(Common::CubeCoordinate target);

    /**
     * @brief moveTransport Moves a transport on the gameboard.
     * @param target Coordinates of the target hex.
     */
    void moveTransport(Common::CubeCoordinate target);

    /**
     * @brief moveTransportInMovement Moves a transport in the MOVEMENET phase.
     * @param source Coordinates of the source hex.
     * @param target Coordinates of the target hex.
     * @param transportItem Pointer to the TransportItem to be moved.
     */
    void moveTransportInMovement(Common::CubeCoordinate source,
                                 Common::CubeCoordinate target,
                                 TransportItem *transportItem);

    /**
     * @brief moveTransportInSpinning Moves a transport in the SPINNING phase.
     * @param source Coordinates of the source hex.
     * @param target Coordinates of the target hex.
     * @param transportItem TransportItem to be moved.
     */
    void moveTransportInSpinning(Common::CubeCoordinate source,
                                 Common::CubeCoordinate target,
                                 TransportItem *transportItem);
    /**
     * @brief removePawns Removes pawnItems and pawns in the gameboard object.
     * @param location Location of the pawns.
     * @param msg A message for the popup.
     */
    bool removePawns(Common::CubeCoordinate location);

    /**
     * @brief removeActors Removes actorItems and actor in the gameboard object.
     * @param location Location of the actors.
     * @param msg A message for the popup.
     */
    bool removeActors(Common::CubeCoordinate location);

    /**
     * @brief removeTransports Removes transportItems and transports in the gameboard object.
     * @param location Location of the pawns.
     * @param boatsOnly When true, removing only boats, otherwise removes dolphins also.
     */
    bool removeTransports(Common::CubeCoordinate location);

    /**
     * @brief actorDoAction Calls doAction for the actor and updates the graphicsItems.
     * @param actorId Identification of the actor.
     */
    void actorDoAction(int actorId);

    /**
     * @brief movePawnToTransport moves given pawn inside given transport.
     * @param pawnId Identifier of the pawn to be moved.
     * @param transportId Identifier of the transport to be moved in.
     * @param coords Coordinates of the hex where items are located.
     */
    void movePawnToTransport(int pawnId, int transportId);

    /**
     * @brief movePawnWithDolphin Moves a pawns thats riding a dolphing.
     * @param sourceHex Pointer to the source hex.
     * @param targetHex Pointer to the target hex.
     * @param transportItem Pointer to the transport.
     * @param pawnItem Pointer to the pawn.
     */
    void movePawnWithDolphin(HexItem *sourceHex, HexItem *targetHex,
                             TransportItem *transportItem, PawnItem *pawnItem);

    /**
     * @brief checkActors Checks if there is actors in a hex and calls doAction for them.
     * @param location Coordinates of the hex.
     */
    void checkActors(Common::CubeCoordinate location);

    /**
     * @brief changePlayer Changes the player in turn.
     */
    void changePlayer();

    /**
     * @brief checkPawns Checks if a player has pawns left and skips turn if not.
     * @param id Identification of the player.
     */
    void checkPawns(int id);

    /**
     * @brief checkGameEnd Checks if the game has ended, announces winner and closes the game.
     */
    void checkGameEnd();

    /**
     * @brief updateInfo Updates info about the on going game.
     */
    void updateInfo();

    /**
     * @brief updateHexInfo Updates info about hex
     * @param coord Hex's, that is currently under mouse, coordinates
     */
    void updateHexInfo(Common::CubeCoordinate coord);

    /**
     * @brief setActorsText sets text to hexActors text label
     */
    void setActorsText(std::shared_ptr<Common::Hex> hex);

    /**
     * @brief setTransportsText sets text to hexTransports text label
     */
    void setTransportsText(std::shared_ptr<Common::Hex> hex);

    /**
     * @brief isActor Checks if creatable under hex is considered as actor
     * @param type Type of the creatable
     * @return true if creatable is actor
     */
    bool isActor(std::string type);

    /**
     * @brief initializeGameEngine Initializes the gameRunner.
     */
    void initializeGameEngine();

    /**
     * @brief showPopup Show a popup.
     * @param msg Message shown in the popup.
     */
    void showPopup(QString msg);

    Ui::MainWindow *ui_;
    QGraphicsScene *boardScene_;
    View *boardView_;

    // Containers for graphical items in the game.
    std::map<int, PawnItem *> pawnItems_;
    std::map<int, std::pair<std::string, ActorItem *>> actorItems_;
    std::map<int, std::pair<std::string, TransportItem *>> transportItems_;
    std::map<Common::CubeCoordinate, HexItem *> hexItems_;

    std::shared_ptr<GameBoard> gameBoard_;
    std::shared_ptr<GameState> gameState_;
    std::map<int, std::shared_ptr<Player>> players_;
    std::shared_ptr<Common::IGameRunner> gameEngine_;

    // If a pawn is selected its id is stored here.
    int pawnToBeMoved_;
    // If an actor is selected its id is stored here.
    int actorToBeMoved_;
    // If a transport is selected its id is stored here.
    int transportToBeMoved_;
    // When the wheel is spun the results are stored here.
    std::pair<std::string, std::string> wheelInfo_;
};

}

#endif // MAINWINDOW_HH
