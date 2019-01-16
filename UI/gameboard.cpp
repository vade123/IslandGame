/* file: gameboard.cpp
 * description: Implementation for the class GameBoard.
 */

#include "gameboard.hh"

namespace Student {

GameBoard::GameBoard() :
    hexes_({}),
    pawns_({}),
    actors_({}),
    transports_({})
{
}

int GameBoard::checkTileOccupation(Common::CubeCoordinate tileCoord) const
{
    auto it = hexes_.find(tileCoord);
    if (it == hexes_.end()) {
        return -1;
    }
    return hexes_.at(tileCoord)->getPawnAmount();
}

bool GameBoard::isWaterTile(Common::CubeCoordinate tileCoord) const
{
    auto it = hexes_.find(tileCoord);
    if (it == hexes_.end()) {
        return false;
    }
    else if (it->second->isWaterTile() != true) {
        return false;
    }
    else {
        return true;
    }
}

std::shared_ptr<Common::Hex> GameBoard::getHex(Common::CubeCoordinate
                                               hexCoord) const
{
    auto it = hexes_.find(hexCoord);
    if (it == hexes_.end()) {
        return nullptr;
    }
    return it->second;
}

void GameBoard::addPawn(int playerId, int pawnId)
{
    std::shared_ptr<Common::Pawn> pawn =
            std::make_shared<Common::Pawn>();
    pawn->setId(playerId, pawnId);
    pawns_[pawnId] = pawn;
}

void GameBoard::addPawn(int playerId, int pawnId, Common::CubeCoordinate coord)
{
    std::shared_ptr<Common::Pawn> pawn =
            std::make_shared<Common::Pawn>(pawnId, playerId, coord);
    hexes_.at(coord)->addPawn(pawn);
    pawns_[pawnId] = pawn;
}

void GameBoard::movePawn(int pawnId, Common::CubeCoordinate pawnCoord)
{
    auto it = hexes_.find(pawnCoord);
    if (it != hexes_.end()) {
        auto pawn = pawns_.at(pawnId);
        hexes_.at(pawn->getCoordinates())->removePawn(pawn);
        hexes_.at(pawnCoord)->addPawn(pawn);
        pawns_.at(pawnId)->setCoordinates(pawnCoord);
    }
}

void GameBoard::removePawn(int pawnId)
{
    auto pawn = pawns_.at(pawnId);
    hexes_.at(pawn->getCoordinates())->removePawn(pawn);
    pawns_.erase(pawnId);
}

void GameBoard::addActor(std::shared_ptr<Common::Actor> actor,
                         Common::CubeCoordinate actorCoord)
{
    actors_[actor->getId()] = actorCoord;
    actor->addHex(hexes_.at(actorCoord));
}

void GameBoard::moveActor(int actorId, Common::CubeCoordinate actorCoord)
{
    auto it = hexes_.find(actorCoord);
    if (it != hexes_.end()) {
        auto actor = hexes_.at(actors_.at(actorId))->giveActor(actorId);
        actors_.at(actorId) = actorCoord;
        actor->move(hexes_.at(actorCoord));
    }
}

void GameBoard::removeActor(int actorId)
{
    auto actor = hexes_.at(actors_.at(actorId))->giveActor(actorId);
    hexes_.at(actors_.at(actorId))->removeActor(actor);
    actors_.erase(actorId);
}

void GameBoard::addHex(std::shared_ptr<Common::Hex> newHex)
{
    hexes_[newHex->getCoordinates()] = newHex;
}

void GameBoard::addTransport(std::shared_ptr<Common::Transport> transport,
                             Common::CubeCoordinate coord)
{
    transports_[transport->getId()] = coord;
    transport->addHex(hexes_.at(coord));
}

void GameBoard::moveTransport(int id, Common::CubeCoordinate coord)
{
    auto it = hexes_.find(coord);
    if (it != hexes_.end()) {
        auto transport = hexes_.at(transports_.at(id))->giveTransport(id);
        transports_.at(id) = coord;
        transport->move(hexes_.at(coord));
    }
}

void GameBoard::removeTransport(int id)
{
    auto transport = hexes_.at(transports_.at(id))->giveTransport(id);
    hexes_.at(transports_.at(id))->removeTransport(transport);
    transports_.erase(id);
}

Common::CubeCoordinate GameBoard::getPawnCoords(int id) const
{
    return pawns_.at(id)->getCoordinates();
}

Common::CubeCoordinate GameBoard::getActorCoords(int id) const
{
    return actors_.at(id);
}

Common::CubeCoordinate GameBoard::getTransportCoords(int id) const
{
    return transports_.at(id);
}

std::map<Common::CubeCoordinate, std::shared_ptr<Common::Hex>>
GameBoard::getBoard() const
{
    return hexes_;
}

}
