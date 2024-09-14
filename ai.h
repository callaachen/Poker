/******************************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                                         */
/* Authors: The Lucky Sevens (Team 7)                                         */
/* ai.h: Function declarations and data structures for server Bots            */
/******************************************************************************/

#ifndef AI_H
#define AI_H

#include "player.h"
#include "game.h"
#include "cards.h"

struct AI {
    int BetMeter;
    int FoldMeter;
    int CheckMeter;
};
 
class Bots{
    private:
        Bots();
        AI ai;
        static int botCount;
    public:
        static int getBotCount();

        static void incrementBotCount();

        static bool createBotPlayer(AllPlayers& allPlayers);

        void CheckMoney(const Player& player, const Game& game); 

        void CheckCards(const Player& player, const Game& game);

        int MakeBetAmount(const Player& player, const Game& game);

        void makeDecision(Player& player, Game& game);
};

#endif 