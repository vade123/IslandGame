#include <QString>
#include <QtTest>

#include "gamestate.hh"
#include "hex.hh"
#include "pawn.hh"
#include "transport.hh"
#include "dolphin.hh"
#include "boat.hh"
#include "actor.hh"
#include "kraken.hh"
#include "seamunster.hh"
#include "shark.hh"
#include "vortex.hh"

class GameStateTest : public QObject
{
    Q_OBJECT

public:
    GameStateTest();

private Q_SLOTS:
    void init();
    void testConstructor();

    // Player turn changes
    void testChangePlayerTurn();

    // GamePhase changes
    void testChangeGamePhase();

    // WinCondition changes
    void testEndGame();

    // winnerId is correct
    void testWinnerId();

private:
    std::shared_ptr<Student::GameState> state_;
};

GameStateTest::GameStateTest()
{
}

void GameStateTest::init()
{
    state_.reset();
    state_ = std::make_shared<Student::GameState>();
}

void GameStateTest::testConstructor()
{
    QVERIFY(state_->currentPlayer() == 1);
    QVERIFY(state_->currentGamePhase() == Common::GamePhase::MOVEMENT);
    QVERIFY(state_->checkWinCondition() == false);
}

void GameStateTest::testChangePlayerTurn()
{
    QVERIFY(state_->currentPlayer() == 1);
    int nextPlayer = 2;
    state_->changePlayerTurn(nextPlayer);
    QVERIFY(state_->currentPlayer() == nextPlayer);
}

void GameStateTest::testChangeGamePhase()
{
    QVERIFY(state_->currentGamePhase() == Common::GamePhase::MOVEMENT);
    state_->changeGamePhase(Common::GamePhase::SINKING);
    QVERIFY(state_->currentGamePhase() == Common::GamePhase::SINKING);
}

void GameStateTest::testEndGame()
{
    QVERIFY(state_->checkWinCondition() == false);
    state_->endGame(0);
    QVERIFY(state_->checkWinCondition() == true);
}

void GameStateTest::testWinnerId()
{
    int winnerId = 3;
    state_->endGame(winnerId);
    QVERIFY(state_->getWinnerId() == winnerId);
}

QTEST_APPLESS_MAIN(GameStateTest)

#include "tst_gamestatetest.moc"
