/* file: player.hh
 * description: Header for class Player.
 */

#ifndef PLAYER_HH
#define PLAYER_HH

#include "iplayer.hh"
#include "pawn.hh"
#include <map>
#include <memory>

namespace Student {

class Player : public Common::IPlayer
{
public:
    Player(int id, unsigned pawns);
    ~Player() = default;

    /**
     * @brief getPlayer gets the player id.
     * @return The identifier of the player.
     * @post Exception quarantee: nothrow
     */
    int getPlayerId() const;

    /**
     * @brief setActionsLeft sets the player's remaining number of actions.
     * @param actionsLeft The number of actions left.
     * @post Actions left set to parameter value. Exception quarantee: nothrow
     */
    void setActionsLeft(unsigned int actionsLeft);

    /**
     * @brief getActionsLeft returns the amount of actions left for the player
     * @return amount of actions
     * @post Exception quarantee: nothrow
     */
    unsigned int getActionsLeft() const;

    /**
     * @brief getPoints returns the amount of points player has gained in game
     * @return amount of points
     * @post Exception quarantee: nothrow
     */
    unsigned int getPoints() const;

    /**
     * @brief addPoints adds the amount of points given in parameter to player's points
     * @param points The amount of points to be added
     * @post Exception quarantee: nothrow
     */
    void addPoints(unsigned int points);

    /**
     * @brief getPawnsLeft returns the amount of pawns left for the player
     * @return amount of pawns
     * @post Exception quarantee: nothrow
     */
    unsigned int getPawns() const;

    /**
     * @brief removePawn reduces player's pawn amount by one
     */
    void removePawn();

    /**
     * @brief setStartingCoordAndColor sets the starting coordinates and the
     *        player color depending on the playerId_
     */
    void setStartingCoordAndColor();

    /**
     * @brief getStartingCoord returns the starting coordinates of the player
     * @return startingCoord_
     */
    Common::CubeCoordinate getStartingCoord();

    /**
     * @brief getPlayerColor returns the color of the player
     * @return playerColor_
     */
    std::string getPlayerColor();

private:
    int playerId_;
    Common::CubeCoordinate startingCoord_;
    std::string playerColor_;
    unsigned pawns_;
    unsigned actionsLeft_;
    unsigned points_;
};

}

#endif // PLAYER_HH
