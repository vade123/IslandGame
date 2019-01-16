/* file: player.cpp
 * description: Implementation for the class Player.
 */

#include "player.hh"

namespace Student {

Player::Player(int id, unsigned pawns) :
    playerId_(id),
    startingCoord_({0,0,0}),
    pawns_(pawns),
    actionsLeft_(3),
    points_(0)
{
    setStartingCoordAndColor();
}

int Player::getPlayerId() const
{
    return playerId_;
}

void Player::setActionsLeft(unsigned int actionsLeft)
{
    actionsLeft_ = actionsLeft;
}

unsigned int Player::getActionsLeft() const
{
    return actionsLeft_;
}

unsigned int Player::getPoints() const
{
    return points_;
}

void Player::addPoints(unsigned int points)
{
    points_ += points;
}

unsigned int Player::getPawns() const
{
    return pawns_;
}

void Player::removePawn()
{
    pawns_ -= 1;
}

void Player::setStartingCoordAndColor()
{
    switch (playerId_) {
    case 1:
        startingCoord_ = {-1,1,0};
        playerColor_ = "red";
        break;
    case 2:
        startingCoord_ = {1,-1,0};
        playerColor_ = "blue";
        break;
    case 3:
        startingCoord_ = {0,-1,1};
        playerColor_ = "green";
        break;
    case 4:
        startingCoord_ = {0,1,-1};
        playerColor_ = "yellow";
        break;
    case 5:
        startingCoord_ = {1,0,-1};
        playerColor_ = "orange";
        break;
    case 6:
        startingCoord_ = {-1,0,1};
        playerColor_ = "pink";
        break;
    }
}

Common::CubeCoordinate Player::getStartingCoord()
{
    return startingCoord_;
}

std::string Player::getPlayerColor()
{
    return playerColor_;
}

}
