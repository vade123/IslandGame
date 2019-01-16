/* file: gamestate.hh
 * description: Header for class GameState.
 */

#ifndef GAMESTATE_HH
#define GAMESTATE_HH

#include "igamestate.hh"

namespace Student {

class GameState : public Common::IGameState
{
public:
    GameState();
    ~GameState() = default;

    /**
     * @brief currentGamePhase tells the phase of the game.
     * @return The phase of the game.
     * @post Exception quarantee: nothrow
     */
    Common::GamePhase currentGamePhase() const;

    /**
     * @brief currentPlayer tells the player in turn.
     * @return The identifier of the player in turn.
     * @post Excetion quarantee: nothrow
     */
    int currentPlayer() const;

    /**
     * @brief changeGamePhase sets the phase of the game.
     * @param newPhase The next phase of the game.
     * @post Game phase changed. Exception quarantee: basic
     */
    void changeGamePhase(Common::GamePhase nextPhase);

    /**
     * @brief changePlayerTurn sets the player in turn.
     * @param nextPlayer The identifier of the next player in turn.
     * @post Turn changed to player nextPlayer. Exception quarantee: basic
     */
    void changePlayerTurn(int nextPlayer);

    /**
     * @brief checkWinCondition checks if winCondition_ is true
     * @return winCondition_
     */
    bool checkWinCondition();

    /**
     * @brief endGame sets winCondition_ as true
     * @param winnerId identifier of the winning player
     */
    void endGame(int winnerId);

    /**
     * @brief getWinnerId returns the id of the winning player
     * @return winnerId_
     */
    int getWinnerId();

private:
    Common::GamePhase currentGamePhase_;
    int currentPlayerId_;
    bool winCondition_;
    int winnerId_;

};

}
#endif // GAMESTATE_HH
