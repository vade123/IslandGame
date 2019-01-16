/* file: gamestate.cpp
 * description: Implementation for the class GameState.
 */

#include "gamestate.hh"

namespace Student {

GameState::GameState() :
    currentGamePhase_(Common::GamePhase::MOVEMENT),
    currentPlayerId_(1),
    winCondition_(false),
    winnerId_(0)
{
}

Common::GamePhase GameState::currentGamePhase() const
{
    return currentGamePhase_;
}

int GameState::currentPlayer() const
{
    return currentPlayerId_;
}

void GameState::changeGamePhase(Common::GamePhase nextPhase)
{
    currentGamePhase_ = nextPhase;
}

void GameState::changePlayerTurn(int nextPlayer)
{
    currentPlayerId_ = nextPlayer;
}

bool GameState::checkWinCondition()
{
    return winCondition_;
}

void GameState::endGame(int winnerId)
{
    winCondition_ = true;
    winnerId_ = winnerId;
}

int GameState::getWinnerId()
{
    return winnerId_;
}

}
