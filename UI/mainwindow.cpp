/* file: mainwindow.cpp
 * description: Implementation for the class MainWindow.
 */

#include "mainwindow.hh"
#include "ui_mainwindow.h"

namespace Student {

MainWindow::MainWindow(unsigned players, unsigned pawns, QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    boardScene_(new QGraphicsScene),
    boardView_(new View),
    pawnItems_({}),
    actorItems_({}),
    transportItems_({}),
    gameBoard_(std::make_shared<GameBoard>()),
    gameState_(std::make_shared<GameState>()),
    players_({}),
    gameEngine_(nullptr),
    pawnToBeMoved_(0),
    actorToBeMoved_(0),
    transportToBeMoved_(0)
{
    ui_->setupUi(this);
    setCentralWidget(ui_->horizontalWidget);
    ui_->viewLayout->addWidget(boardView_);
    setPlayers(players, pawns);
    initializeGameEngine();
    initializePawns();
    drawBoard();
    updateInfo();
    // // ui_->hexInfo->hide();
    connect(ui_->spinWheelButton, &QPushButton::clicked,
            this, &MainWindow::spinWheel);
    connect(ui_->skipButton, &QPushButton::clicked,
            this, &MainWindow::skipMovement);
    ui_->skipButton->setEnabled(true);
    ui_->spinWheelButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete boardScene_;
    delete boardView_;
    delete ui_;
}

void MainWindow::setPlayers(unsigned int players, unsigned int pawns)
{
    for (unsigned i = 0; i < players; ++i) {
        int playerId = static_cast<int>(i+1);
        std::shared_ptr<Player> newPlayer =
                std::make_shared<Player>(playerId, pawns);
        players_[playerId] = newPlayer;
    }
}

void MainWindow::pawnSelected(int pawnId)
{
    if (gameState_->currentGamePhase() == Common::GamePhase::MOVEMENT) {
        if (pawnToBeMoved_ != 0) {
            pawnItems_.at(pawnToBeMoved_)->setPawnPixmap(false);
            pawnToBeMoved_ = 0;
            toggleHexHighlighting(false);
        } else {
            pawnToBeMoved_ = pawnId;
            pawnItems_.at(pawnToBeMoved_)->setPawnPixmap(true);
            toggleHexHighlighting(true);
        }
    }
}

void MainWindow::actorSelected(int actorId)
{
    if (gameState_->currentGamePhase() == Common::GamePhase::SPINNING) {
        if ((ui_->spinWheelButton->isEnabled() != true) &&
            (transportToBeMoved_ == 0)) {
            if (actorToBeMoved_ != 0) {
                actorItems_.at(actorToBeMoved_).second->setActorPixmap(false);
                actorToBeMoved_ = 0;
                toggleHexHighlighting(false);
            } else {
                actorToBeMoved_ = actorId;
                actorItems_.at(actorToBeMoved_).second->setActorPixmap(true);
                toggleHexHighlighting(true);
            }
        }
    }
}

void MainWindow::transportSelected(int transportId)
{
    if (gameState_->currentGamePhase() == Common::GamePhase::MOVEMENT) {
        Common::CubeCoordinate coords =
                gameBoard_->getTransportCoords(transportId);
        std::shared_ptr<Common::Transport> transport =
                gameBoard_->getHex(coords)->giveTransport(transportId);
        int i = static_cast<int>(transport->getPawnsInTransport().size());
        // transport cant be selected if there's no pawns in it
        if (i == 0) {
            return;
        }
    } else if (gameState_->currentGamePhase() == Common::GamePhase::SINKING) {
        return;

    // currentGamePhase == SPINNING
    } else {
        if ((transportItems_.at(transportId).first == "boat") ||
            (ui_->spinWheelButton->isEnabled() == true) ||
            (actorToBeMoved_ != 0) ) {
            return;
        }
    }

    if (transportToBeMoved_ != 0) {
        transportItems_.at(transportToBeMoved_).second->setTransportPixmap(false);
        transportToBeMoved_ = 0;
        toggleHexHighlighting(false);
    } else {
        transportToBeMoved_ = transportId;
        transportItems_.at(transportToBeMoved_).second->setTransportPixmap(true);
        toggleHexHighlighting(true);
    }
}

void MainWindow::hexClicked(Common::CubeCoordinate coords)
{
    if (gameState_->currentGamePhase() == Common::GamePhase::MOVEMENT) {
        movePawn(coords);
        moveTransport(coords);
        toggleHexHighlighting(false);

    } else if (gameState_->currentGamePhase() == Common::GamePhase::SINKING) {
        initializeActor(coords);

    // currentGamePhase == SPINNING
    } else {
        moveActor(coords);
        moveTransport(coords);
        toggleHexHighlighting(false);
    }
}

void MainWindow::movePawn(Common::CubeCoordinate target)
{
    if (pawnToBeMoved_ == 0){
        return;
    }

    Common::CubeCoordinate source = gameBoard_->getPawnCoords(pawnToBeMoved_);
    std::shared_ptr<Common::Pawn> pawn =
            gameBoard_->getHex(source)->givePawn(pawnToBeMoved_);
    PawnItem *pawnItem = pawnItems_.at(pawnToBeMoved_);
    HexItem *sourceHexItem = hexItems_.at(source);
    HexItem *targetHexItem = hexItems_.at(target);

    try {
        int movesLeft = gameEngine_->movePawn(source, target, pawnToBeMoved_);
        for (auto transport : gameBoard_->getHex(source)->getTransports()) {
                    if (transport->isPawnInTransport(pawn)) {
                        transport->removePawn(pawn);
                    }
                }
        pawnItem->setLocationOnBoard(sourceHexItem, targetHexItem, boardScene_);

        for (auto transport : gameBoard_->getHex(target)->getTransports()) {
            movePawnToTransport(pawnToBeMoved_, transport->getId());
        }

        checkActors(target);

        if (gameBoard_->getHex(target)->getPieceType() == "Coral") {
            gameState_->endGame(gameState_->currentPlayer());
            checkGameEnd();
        }

        if (movesLeft == 0) {
            gameState_->changeGamePhase(Common::GamePhase::SINKING);
            ui_->skipButton->setEnabled(false);
        }
    }
    catch (Common::IllegalMoveException &e) {
        pawnItem->setPawnPixmap(false);
        showPopup(QString::fromStdString(e.msg()));
    }
    pawnToBeMoved_ = 0;
    updateInfo();
}

void MainWindow::moveActor(Common::CubeCoordinate target)
{
    if (actorToBeMoved_ == 0) {
        return;
    }

    Common::CubeCoordinate source = gameBoard_->getActorCoords(actorToBeMoved_);
    ActorItem *actorItem = actorItems_.at(actorToBeMoved_).second;
    HexItem *sourceHexItem = hexItems_.at(source);
    HexItem *targetHexItem = hexItems_.at(target);

    if (gameBoard_->getHex(source)->giveActor(actorToBeMoved_)->getActorType()
            != wheelInfo_.first) {
        actorItem->setActorPixmap(false);
        showPopup("Illegal actor move");
        actorToBeMoved_ = 0;
        updateInfo();
        return;
    }
    if (source == target) {
        actorItem->setActorPixmap(false);
        showPopup("Illegal actor move");
        actorToBeMoved_ = 0;
        updateInfo();
        return;
    }
    // A single hex can fit 3 actors.
    if (gameBoard_->getHex(target)->getActors().size() >= 3) {
        actorItem->setActorPixmap(false);
        showPopup("Illegal actor move");
        actorToBeMoved_ = 0;
        updateInfo();
        return;
    }

    try {
        gameEngine_->moveActor(source, target, actorToBeMoved_, wheelInfo_.second);
        actorItem->setLocationOnBoard(sourceHexItem, targetHexItem, boardScene_);
        actorDoAction(actorToBeMoved_);
        gameState_->changeGamePhase(Common::GamePhase::MOVEMENT);
        changePlayer();
        ui_->spinInfo->setText("");
        wheelInfo_ = {};
    }
    catch (Common::IllegalMoveException &e) {
        actorItem->setActorPixmap(false);
        showPopup(QString::fromStdString(e.msg()));
    }
    actorToBeMoved_ = 0;
    updateInfo();
}

void MainWindow::moveTransport(Common::CubeCoordinate target)
{
    if (transportToBeMoved_ == 0) {
        return;
    }
    Common::CubeCoordinate source =
            gameBoard_->getTransportCoords(transportToBeMoved_);

    TransportItem *transportItem =
            transportItems_.at(transportToBeMoved_).second;

    if (source == target) {
        transportItem->setTransportPixmap(false);
        showPopup("Illegal transport move");
        transportToBeMoved_ = 0;
        updateInfo();
        return;
    }
    if (gameState_->currentGamePhase() == Common::GamePhase::MOVEMENT) {
        moveTransportInMovement(source, target, transportItem);
    }
    else if (gameState_->currentGamePhase() == Common::GamePhase::SPINNING) {
        moveTransportInSpinning(source, target, transportItem);
    } else {
        // If currentGamePhase is not MOVEMENT or SPINNING, transports do not move.
    }
    transportToBeMoved_ = 0;
    updateInfo();
}

void MainWindow::moveTransportInMovement(Common::CubeCoordinate source,
                                         Common::CubeCoordinate target,
                                         TransportItem *transportItem)
{
    HexItem *sourceHexItem = hexItems_.at(source);
    HexItem *targetHexItem = hexItems_.at(target);

    if ((transportItem->getType() == "boat") &&
            (targetHexItem->transportSlotsFull() > 0)) {
        transportItem->setTransportPixmap(false);
        showPopup("Illegal transport move");
        return;
    }
    if ((transportItem->getType() == "dolphin") &&
            (targetHexItem->transportSlotsFull() >= 3)) {
        transportItem->setTransportPixmap(false);
        showPopup("Illegal transport move");
        return;
    }
    int movesLeft = 0;
    try {
        movesLeft =
                gameEngine_->moveTransport(source, target, transportToBeMoved_);
   } catch (Common::IllegalMoveException &e) {
        transportItem->setTransportPixmap(false);
        showPopup(QString::fromStdString(e.msg()));
        return;
    }
    if (transportItem->getType() == "dolphin") {
        for (auto pawn : gameBoard_->getHex(target)->giveTransport(
                 transportToBeMoved_)->getPawnsInTransport()) {
            movePawnWithDolphin(sourceHexItem, targetHexItem, transportItem,
                                pawnItems_.at(pawn->getId()));
        }
    } else {
        transportItem->setLocationOnBoard(
                    sourceHexItem, targetHexItem, boardScene_);
        for (auto pawn : gameBoard_->getHex(target)->giveTransport(
                 transportToBeMoved_)->getPawnsInTransport()) {
            pawnItems_.at(pawn->getId())->setLocationOnBoard(
                        sourceHexItem, targetHexItem, boardScene_);
        }
    }
    for (auto pawn : gameBoard_->getHex(target)->getPawns()) {
        movePawnToTransport(pawn->getId(), transportToBeMoved_);
    }
    checkActors(target);
    if (movesLeft == 0) {
        gameState_->changeGamePhase(Common::GamePhase::SINKING);
        ui_->skipButton->setEnabled(false);
    }
}

void MainWindow::moveTransportInSpinning(Common::CubeCoordinate source,
                                         Common::CubeCoordinate target,
                                         TransportItem *transportItem)
{
    HexItem *sourceHexItem = hexItems_.at(source);
    HexItem *targetHexItem = hexItems_.at(target);
    if (gameBoard_->getHex(source)->giveTransport(
                transportToBeMoved_)->getTransportType() != wheelInfo_.first) {
        transportItem->setTransportPixmap(false);
        showPopup("Illegal transport move");
        return;
    }
    if (transportItem->getType() == "boat") {
        transportItem->setTransportPixmap(false);
        showPopup("Illegal transport move");
        return;
    }
    if ((transportItem->getType() == "dolphin") &&
               (targetHexItem->transportSlotsFull() >= 3)) {
        transportItem->setTransportPixmap(false);
        showPopup("Illegal transport move");
        return;
    }
    try {
        gameEngine_->moveTransportWithSpinner(
                    source, target, transportToBeMoved_, wheelInfo_.second);
    } catch (Common::IllegalMoveException &e) {
        transportItem->setTransportPixmap(false);
        showPopup(QString::fromStdString(e.msg()));
        return;
    }
    std::vector<std::shared_ptr<Common::Pawn>> pawnsInTransport =
            gameBoard_->getHex(target)->giveTransport(
                transportToBeMoved_)->getPawnsInTransport();

    if ((pawnsInTransport.size() > 0) && (wheelInfo_.second != "D")) {
        for (auto pawn : pawnsInTransport) {
            movePawnWithDolphin(sourceHexItem, targetHexItem, transportItem,
                                pawnItems_.at(pawn->getId()));
        }
    } else {
        transportItem->setLocationOnBoard(
                    sourceHexItem, targetHexItem, boardScene_);
    }
    for (auto pawn : gameBoard_->getHex(target)->getPawns()) {
        movePawnToTransport(pawn->getId(), transportToBeMoved_);
    }
    checkActors(target);
    gameState_->changeGamePhase(Common::GamePhase::MOVEMENT);
    changePlayer();
    ui_->spinInfo->setText("");
    wheelInfo_ = {};
}

bool MainWindow::removePawns(Common::CubeCoordinate location)
{
    bool removed = false;
    std::vector<std::shared_ptr<Common::Pawn>> pawns =
            gameBoard_->getHex(location)->getPawns();
    std::vector<std::shared_ptr<Common::Transport>> transports =
            gameBoard_->getHex(location)->getTransports();
    for (std::shared_ptr<Common::Pawn> pawn : pawns) {
        bool isInTransport = false;
        for (std::shared_ptr<Common::Transport> transport : transports) {
            if (transport->isPawnInTransport(pawn)) {
                isInTransport = true;
            }
        }
        if (!isInTransport) {
            int pawnId = pawn->getId();
            players_.at(pawn->getPlayerId())->removePawn();
            int slot = pawnItems_.at(pawnId)->currentSlot();
            hexItems_.at(location)->changeSlotOccupation("pawn", slot);
            delete pawnItems_.at(pawnId);
            pawnItems_.erase(pawnId);
            gameBoard_->removePawn(pawnId);
            removed = true;
        }
    }
    return removed;
}

bool MainWindow::removeActors(Common::CubeCoordinate location)
{
    bool removed = false;
    std::vector<std::shared_ptr<Common::Actor>> actors =
            gameBoard_->getHex(location)->getActors();
    for (std::shared_ptr<Common::Actor> actor : actors) {
        int actorId = actor->getId();
        if (actor->getActorType() != "vortex") {
            int slot = actorItems_.at(actorId).second->currentSlot();
            hexItems_.at(location)->changeSlotOccupation("actor", slot);
            delete actorItems_.at(actorId).second;
            actorItems_.erase(actorId);
            gameBoard_->removeActor(actorId);
            removed = true;
        }
    }
    return removed;
}

bool MainWindow::removeTransports(Common::CubeCoordinate location)
{
    bool removed = false;
    std::vector<std::shared_ptr<Common::Transport>> transports =
            gameBoard_->getHex(location)->getTransports();
    for (std::shared_ptr<Common::Transport> transport : transports) {
        int transportId = transport->getId();
        transport->removePawns();
        int slot = transportItems_.at(transportId).second->currentSlot();
        hexItems_.at(location)->changeSlotOccupation(
                    transport->getTransportType(), slot);
        delete transportItems_.at(transportId).second;
        transportItems_.erase(transportId);
        gameBoard_->removeTransport(transportId);
        removed = true;
    }
    return removed;
}

void MainWindow::actorDoAction(int actorId)
{
    bool removed = false;
    QString msg = "";

    Common::CubeCoordinate location = gameBoard_->getActorCoords(actorId);
    std::shared_ptr<Common::Actor> actor =
            gameBoard_->getHex(location)->giveActor(actorId);
    std::string actorType = actor->getActorType();
    if (actorType == "shark") {
        removed = removePawns(location);
        msg =  "Shark: omnom good pawns thank";
    } else if (actorType == "kraken") {
        removed = removeTransports(location);
        msg = "kraken smash boat and snacc dolphin mur";
        for (auto otherActor : gameBoard_->getHex(location)->getActors()) {
            if (otherActor->getActorType() == "shark") {
                actorDoAction(otherActor->getId());
            }
        }
    } else if (actorType == "seamunster") {
        removed = removeTransports(location);
        bool tmp = removePawns(location);
        if (tmp) {
            removed = true;
        }
        msg = "very taste boat, dolphin and mr pawn best wishes seamunster";
    } else if (actorType == "vortex") {
        std::vector<Common::CubeCoordinate> neighbours =
                gameBoard_->getHex(location)->getNeighbourVector();
        removeTransports(location);
        removePawns(location);
        removeActors(location);
        for ( auto neighbour : neighbours) {
            removeTransports(neighbour);
            removePawns(neighbour);
            removeActors(neighbour);
        }
        removed = true;
        msg = "WhiuS WHous regards vortex";
    } else {
        // Game doesn't know the actor
        throw std::invalid_argument("invalid actor type");
    }
    actor->doAction();
    if (removed) {
        showPopup(msg);
        checkPawns(gameState_->currentPlayer());
        checkGameEnd();
    }
}

void MainWindow::movePawnToTransport(int pawnId, int transportId)
{
    Common::CubeCoordinate coords = gameBoard_->getTransportCoords(transportId);

    std::shared_ptr<Common::Transport> transport = gameBoard_->getHex(
                coords)->giveTransport(transportId);

    std::shared_ptr<Common::Pawn> pawn =
            gameBoard_->getHex(coords)->givePawn(pawnId);

    if ((transport->getCapacity() > 0) &&
            (!transport->isPawnInTransport(pawn))) {

        if (transport->getTransportType() == "boat") {
            gameBoard_->getHex(coords)->giveTransport(transportId)->addPawn(
                        gameBoard_->getHex(coords)->givePawn(pawnId));

        } else if (transport->getTransportType() == "dolphin") {
            if (pawnItems_.at(pawnId)->currentSlot() ==
                    transportItems_.at(transportId).second->currentSlot()) {
                gameBoard_->getHex(coords)->giveTransport(transportId)->addPawn(
                            gameBoard_->getHex(coords)->givePawn(pawnId));
            }
        } else {
            // Game doesn't know the transport
            throw std::invalid_argument("invalid transport type");
        }
    }
}

void MainWindow::movePawnWithDolphin(HexItem *sourceHex,
                                     HexItem *targetHex,
                                     TransportItem *transportItem,
                                     PawnItem *pawnItem)
{
    bool slotPairFound = false;
    for (int i = 0; i < 3; ++i) {
        if ((!targetHex->slotFull("dolphin", i)) &&
                (!targetHex->slotFull("pawn", i))) {

            transportItem->setLocationOnBoard(
                        sourceHex, targetHex, boardScene_, i);

            pawnItem->setLocationOnBoard(sourceHex, targetHex, boardScene_, i);
            slotPairFound = true;
            break;
        }
    }
    if (!slotPairFound) {
        transportItem->setLocationOnBoard(sourceHex, targetHex, boardScene_);
        pawnItem->setLocationOnBoard(sourceHex, targetHex, boardScene_);
    }
}

void MainWindow::checkActors(Common::CubeCoordinate location)
{
    std::shared_ptr<Common::Hex> hex = gameBoard_->getHex(location);
    std::vector<std::shared_ptr<Common::Actor>> actors = hex->getActors();
    for (std::shared_ptr<Common::Actor> actor : actors) {
        actorDoAction(actor->getId());
    }
}

void MainWindow::changePlayer()
{
    ui_->spinInfo->setText("");
    wheelInfo_ = {};

    checkGameEnd();
    if (gameState_->currentPlayer() == static_cast<int>(players_.size())) {
        gameState_->changePlayerTurn(1);
    }
    else {
        gameState_->changePlayerTurn(gameState_->currentPlayer()+1);
    }
    checkPawns(gameState_->currentPlayer());
    players_.at(gameState_->currentPlayer())->setActionsLeft(3);
    ui_->skipButton->setEnabled(true);
    ui_->spinWheelButton->setEnabled(false);

    if (gameState_->currentGamePhase() != Common::GamePhase::MOVEMENT) {
        gameState_->changeGamePhase(Common::GamePhase::MOVEMENT);
    }
    updateInfo();
}

void MainWindow::checkPawns(int id)
{
    if (players_.at(id)->getPawns() == 0) {
        showPopup("Player " + QString::number(id) + " has no pawns left!");
        changePlayer();
    }
}

void MainWindow::checkGameEnd()
{
    std::vector<int> playersNotOut;
    for (auto player : players_) {
        if (player.second->getPawns() != 0 ) {
            playersNotOut.push_back(player.second->getPlayerId());
        }
    }
    if (playersNotOut.size() == 1) {
        gameState_->endGame(playersNotOut.at(0));
    }
    if (gameState_->checkWinCondition() == true) {
        showPopup("Player " +
                  QString::number(gameState_->getWinnerId()) + " wins!");
        close();
    }
}

void MainWindow::toggleHexHighlighting(bool value)
{
    for (auto it : hexItems_) {
        HexItem* hex = it.second;
        hex->setHiglighted(value);
    }
}

void MainWindow::spinWheel()
{
    wheelInfo_ = gameEngine_->spinWheel();
    ui_->spinInfo->setText(QString::fromStdString(wheelInfo_.first +
                                                  " moves " +
                                                  wheelInfo_.second
                                                  + " steps."));
    ui_->spinInfo->update();
    ui_->spinWheelButton->setEnabled(false);

    int i = 0;
    for (auto pair : actorItems_) {
        if (pair.second.first == wheelInfo_.first) {
            ++i;
        }
    }
    if (i == 0) {
        for (auto pair : transportItems_) {
            if (pair.second.first == wheelInfo_.first) {
                ++i;
            }
        }
    }

    if (i == 0) {
        showPopup("No movable actors available");
        changePlayer();
        updateInfo();
    }
}

void MainWindow::skipMovement()
{
    if (gameState_->currentGamePhase() == Common::GamePhase::MOVEMENT) {
        gameState_->changeGamePhase(Common::GamePhase::SINKING);
        ui_->skipButton->setEnabled(false);
        updateInfo();
    } else if (gameState_->currentGamePhase() == Common::GamePhase::SPINNING) {
        changePlayer();
    } else {
        // If currentGamePhase is not MOVEMENT or SPINNING, can't skip moving.
    }
}

void MainWindow::drawBoard()
{
    QBrush brush(Qt::darkGray, Qt::SolidPattern);

    // copy the hexes from gameboard
    auto board = gameBoard_->getBoard();

    for (auto it = board.begin(); it != board.end(); ++it) {
        Common::CubeCoordinate cubeCoords = it->first;
        QPointF pixelCoords = cubeToPixel(cubeCoords, HEX_SIZE);
        QPen pen(Qt::black);
        pen.setWidth(3);
        HexItem *hex = new HexItem(cubeCoords,
                                                     pixelCoords,
                                                     HEX_SIZE, pen);
        connect(hex, &HexItem::clicked, this, &MainWindow::hexClicked);
        connect(hex, &HexItem::hover, this, &MainWindow::updateHexInfo);
        setBrushColor(it->second->getPieceType(), brush);
        addHexToScene(hex, brush);
        initializePawnItems(hex);

        // checking if there's transports in the tile
        for (auto transport : gameBoard_->getHex(cubeCoords)->getTransports()) {
            QSize size = ACTOR_PIXMAP_SIZE;
            if (transport->getTransportType() == "boat") {
                size = BOAT_PIXMAP_SIZE;
            }
            TransportItem *transportItem = new TransportItem(
                        size, transport->getId(), transport->getTransportType());
            connect(transportItem, &TransportItem::selected,
                   this, &MainWindow::transportSelected);

            transportItems_[transport->getId()] =
                    std::make_pair(transport->getTransportType(), transportItem);
            transportItem->setLocationOnBoard(nullptr, hex, boardScene_);
        }
    }
    boardView_->setScene(boardScene_);
}

void MainWindow::initializePawns()
{
    for (auto player : players_) {
        Common::CubeCoordinate playerCoords = player.second->getStartingCoord();
        unsigned pawns = player.second->getPawns();
        unsigned playerId = static_cast<unsigned>(player.second->getPlayerId());
        for (unsigned j = 0; j < pawns; ++j) {
            unsigned pawnId = (playerId * 10) + j + 1;
            gameBoard_->addPawn(static_cast<int>(playerId),
                                static_cast<int>(pawnId),
                                playerCoords);
        }
    }
}

void MainWindow::addHexToScene(HexItem *hex, QBrush &brush)
{
    hex->setBrush(brush);
    hex->setPolygon(hexagonCorners(HEX_SIZE));
    boardScene_->addItem(hex);
    hex->setPos(hex->getPixelCoords().rx(), hex->getPixelCoords().ry());
    hexItems_[hex->getCubeCoords()] = hex;
}

void MainWindow::setBrushColor(std::string pieceType, QBrush &brush)
{
    if (pieceType == "Peak") {
        brush.setColor(Qt::darkGray);
    }
    else if (pieceType == "Mountain") {
        brush.setColor(Qt::lightGray);
    }
    else if (pieceType == "Forest") {
        brush.setColor(Qt::darkGreen);
    }
    else if (pieceType == "Beach") {
        brush.setColor(Qt::yellow);
    }
    else if (pieceType == "Water") {
        brush.setColor(Qt::blue);
    }
    else if (pieceType == "Coral") {
        brush.setColor(Qt::magenta);
    } else {
        // Game doesn't know the piece
        throw std::invalid_argument("invalid piece type");
    }
}

void MainWindow::initializePawnItems(HexItem *hexItem)
{
    Common::CubeCoordinate cubeCoords = hexItem->getCubeCoords();

    std::shared_ptr<Common::Hex> hexPtr = gameBoard_->getHex(cubeCoords);
    std::vector<std::shared_ptr<Common::Pawn>> pawnVector = hexPtr->getPawns();

    int pawnsInTile = static_cast<int>(pawnVector.size());

    if (pawnsInTile == 0) {
        return;
    }

    for (int i = 0; i < pawnsInTile; ++i) {
        int ownerId = pawnVector.at(static_cast<unsigned>(i))->getPlayerId();
        int pawnId = pawnVector.at(static_cast<unsigned>(i))->getId();
        PawnItem *pawnItem = new PawnItem(PAWN_PIXMAP_SIZE, ownerId, pawnId);
        connect(pawnItem, &PawnItem::selected, this, &MainWindow::pawnSelected);
        pawnItems_[pawnId] = pawnItem;
        pawnItem->setLocationOnBoard(nullptr, hexItem, boardScene_);
    }
}

void MainWindow::initializeActor(Common::CubeCoordinate coords)
{
    std::string creatableType = "";
    try {
        creatableType = gameEngine_->flipTile(coords);
    } catch (Common::IllegalMoveException &e) {
        showPopup(QString::fromStdString(e.msg()));
        return;
    }
    HexItem *hex = hexItems_.at(coords);
    hex->setBrush(QBrush(Qt::blue));
    updateHexInfo(coords);
    hex->update();

    if (isActor(creatableType)) {
        for (auto actor : gameBoard_->getHex(coords)->getActors()) {
            ActorItem *actorItem = new ActorItem(
                        ACTOR_PIXMAP_SIZE, actor->getId(), creatableType);
            connect(actorItem, &ActorItem::selected, this,
                    &MainWindow::actorSelected);
            actorItems_[actor->getId()] =
                    std::make_pair(creatableType, actorItem);
            actorItem->setLocationOnBoard(nullptr, hex, boardScene_);
            actorDoAction(actor->getId());
            if (creatableType == "vortex") {
                delete actorItems_.at(actor->getId()).second;
                actorItems_.erase(actor->getId());
                gameBoard_->removeActor(actor->getId());
            }
        }
    } else {
        for (auto transport : gameBoard_->getHex(coords)->getTransports()) {
            QSize size = ACTOR_PIXMAP_SIZE;
            if (creatableType == "boat") {
                size = BOAT_PIXMAP_SIZE;
            }
            TransportItem *transportItem = new TransportItem(
                        size, transport->getId(), creatableType);
            connect(transportItem, &TransportItem::selected,
                   this, &MainWindow::transportSelected);
            transportItems_[transport->getId()] =
                    std::make_pair(creatableType, transportItem);
            transportItem->setLocationOnBoard(nullptr, hex, boardScene_);
            for (auto pawn : gameBoard_->getHex(coords)->getPawns()) {
                movePawnToTransport(pawn->getId(), transport->getId());
            }
        }
    }
    if (gameState_->currentGamePhase() ==Common::GamePhase::SINKING) {
       gameState_->changeGamePhase(Common::GamePhase::SPINNING);
    }
    updateInfo();
    ui_->skipButton->setEnabled(true);
    ui_->spinWheelButton->setEnabled(true);
}

void MainWindow::updateInfo()
{
    int currentPlayerId = gameState_->currentPlayer();
    std::string currentPlayerColor =
            players_.at(currentPlayerId)->getPlayerColor();
    std::string infoText = std::string("IN TURN: \n") + "Player " +
                           std::to_string(currentPlayerId) + "\n" +
                           "(" + currentPlayerColor + " pawns)";
    ui_->currentPlayer->setText(QString::fromStdString(infoText));
    Common::GamePhase currentGamePhase = gameState_->currentGamePhase();
    std::string currentGamePhaseStr = "";

    if (currentGamePhase == Common::GamePhase::MOVEMENT) {
        currentGamePhaseStr = "MOVEMENT";
        unsigned movesLeft = players_.at(currentPlayerId)->getActionsLeft();
        ui_->movesLeft->setText(
                    QString::fromStdString(
                        std::to_string(movesLeft) + " moves left"));
    } else if (currentGamePhase == Common::GamePhase::SINKING) {
        currentGamePhaseStr = "SINKING";
        ui_->movesLeft->setText(" ");
    } else {
        currentGamePhaseStr = "SPINNING";
        ui_->movesLeft->setText(" ");
    }
    ui_->currentGamePhase->setText(QString::fromStdString(currentGamePhaseStr));
}

void MainWindow::updateHexInfo(Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Hex> hex = gameBoard_->getHex(coord);

    std::string pieceType = "Piece type: " + hex->getPieceType();
    ui_->hexPieceType->setText(QString::fromStdString(pieceType));

    std::string pawnsInHex = "Pawns: " + std::to_string(hex->getPawnAmount());
    ui_->hexPawns->setText(QString::fromStdString(pawnsInHex));

    if (hex->getPieceType() == "Water") {
        setActorsText(hex);
        setTransportsText(hex);

    } else {
        ui_->hexActors->setText("");
        ui_->hexTransports->setText("");
    }

    ui_->hexInfo->show();
}

void MainWindow::setActorsText(std::shared_ptr<Common::Hex> hex)
{
    int actors = static_cast<int>(hex->getActors().size());
    std::string actorsText = "Actors: ";

    if (actors == 0) {
        actorsText += std::to_string(0);
    } else {
        std::map<std::string, int> actorTypes;
        for (auto actorType : hex->getActorTypes()) {
            if (actorTypes.find(actorType) == actorTypes.end()) {
                actorTypes[actorType] = 1;
            } else {
                actorTypes.at(actorType) += 1;
            }
        }
        for (auto type : actorTypes) {
            if (type.first == "shark") {
                actorsText += std::to_string(type.second) + "x Shark ";
            } else if (type.first == "kraken") {
                actorsText += std::to_string(type.second) + "x Kraken ";
            } else if (type.first == "seamunster") {
                actorsText += std::to_string(type.second) + "x Seamunster ";
            } else if (type.first == "vortex") {
                // Vortexes do not need to be shown in the info as they disappear.
            } else {
                // Game doesn't know the actor
                throw std::invalid_argument("invalid actor type");
            }
        }
    }
    ui_->hexActors->setText(QString::fromStdString(actorsText));
}

void MainWindow::setTransportsText(std::shared_ptr<Common::Hex> hex)
{
    int transports = static_cast<int>(hex->getTransports().size());
    std::string transportsText = "Transports: ";

    if (transports == 0) {
        transportsText += std::to_string(0);
    } else {
        std::map<std::string, int> transportTypes;
        for (auto transport : hex->getTransports()) {
            if (transportTypes.find(transport->getTransportType()) ==
                    transportTypes.end()) {
                transportTypes[transport->getTransportType()] = 1;
            } else {
                transportTypes.at(transport->getTransportType()) += 1;
            }
        }
        for (auto type : transportTypes) {
            if (type.first == "boat") {
                std::string boats = std::to_string(type.second) + "x Boat ";
                transportsText += boats;
            } else if (type.first == "dolphin") {
                std::string dolphins =
                        std::to_string(type.second) + "x Dolphin ";
                transportsText += dolphins;
            } else {
                // Game doesn't know the transport
                throw std::invalid_argument("invalid transport type");
            }
        }
    }
    ui_->hexTransports->setText(QString::fromStdString(transportsText));
}


bool MainWindow::isActor(std::string type)
{
    if ((type == "shark") ||
        (type == "kraken") ||
        (type == "seamunster") ||
        (type == "vortex")) {
        return true;
    }
    return false;
}

void MainWindow::initializeGameEngine()
{
    std::vector<std::shared_ptr<Common::IPlayer>> players;

    for (auto player : players_) {
        players.push_back(
                    std::static_pointer_cast<Common::IPlayer>(player.second));
    }

    gameEngine_ = Common::Initialization::getGameRunner(gameBoard_,
                                                            gameState_,
                                                            players);
}

void MainWindow::showPopup(QString msg)
{
    QMessageBox error;
    error.setText(msg);
    error.setWindowFlags(Qt::Popup);
    error.setFont(QFont("Rocks__G", 15));
    error.exec();
}

}
