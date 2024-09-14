/******************************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                                         */
/* Authors: The Lucky Sevens (Team 7)                                         */
/* ai.cpp: Function defintions for server Bots                                */
/******************************************************************************/

#include "ai.h"
#include "handEvaluator.h"

int Bots::botCount = 0;

Bots::Bots() {
    ai.BetMeter = 0;
    ai.FoldMeter = 0;
    ai.CheckMeter = 0;
}

int Bots::getBotCount() {
    return botCount;
}

void Bots::incrementBotCount() {
    botCount++;
}

bool Bots::createBotPlayer(AllPlayers& allPlayers) {
    if (botCount >= 7) {
        return false; // Maximum number of bots reached
    }
    
    int botNumber = botCount + 1;
    std::string botName = "Bot" + std::to_string(botNumber);
    std::string botUserID = "bot_" + std::to_string(botNumber);
    
    Player botPlayer(botName, botNumber, 1000, botUserID, true, PlayerType::Computer, DealerRole::NoRole);
    allPlayers.addPlayer(botPlayer);
    
    incrementBotCount();
    return true;
}

void Bots::CheckMoney(const Player& player, const Game& game) { //checks current money left
    int currentMoney = player.getScore();
    int currentBet = game.getPot();
    if (currentMoney < 0.75 * currentBet) { //if current money of bot is less than 75% of current bet, more likely to fold
        ai.FoldMeter += 10;
    }

    else{ //else(bot has more than 75% of current bet), more likely to check/call
        ai.CheckMeter += 10;
    }

}

void Bots::CheckCards(const Player& player, const Game& game) {
    std::vector<Card> communityCards;
    for (int i = 0; i < BOARD_CT; ++i) {
        communityCards.push_back(game.getCommunityCards().getCard(i)); // getting cards on board
    }

    // Combine player's hand and community cards
    Hand combinedHand = player.getHand();
    for (const Card& card : communityCards) {
        combinedHand.addCard(card);
    }

    //Evaluating combined hand
    HandEvaluator evaluator;
    evaluator.evalHandType(combinedHand);
    HandType handType = combinedHand.getHandType();

    //decision meter changes depending on the value of hand ai has
    if (handType >= Straight) {
        ai.BetMeter += 10;
    } else if (handType <= OnePair) {
        ai.FoldMeter += 10;
    } else {
        ai.CheckMeter += 10;
    }
}



int Bots::MakeBetAmount(const Player& player, const Game& game) {
    //set basebet = half of current pot
    int baseBet = game.getPot() / 2;
    return baseBet;
}


void Bots::makeDecision(Player& player, Game& game) {
    // Check current status
    CheckMoney(player, game);
    CheckCards(player, game);

    // Make decision based on meters
    // Fold if foldmeter is highest
    if (ai.FoldMeter > ai.BetMeter && ai.FoldMeter > ai.CheckMeter) {
        game.fold(&player, game.getCurrentPlayerIndex());
    }
    // Bet if betmeter is highest
    else if (ai.BetMeter > ai.FoldMeter && ai.BetMeter > ai.CheckMeter) {
        int betAmount = MakeBetAmount(player, game);
        game.placeBet(&player, game.getCurrentPlayerIndex(), betAmount);
    }
    // Match bet if checkmeter is highest
    else if (ai.CheckMeter > ai.FoldMeter && ai.CheckMeter > ai.BetMeter) {
        game.placeBet(&player, game.getCurrentPlayerIndex(), game.getPot()); // Call to match the current bet
    }
    // Fold if not sure
    else {
        game.fold(&player, game.getCurrentPlayerIndex());
    }
    
    // Reset meters
    ai.BetMeter = 0;
    ai.FoldMeter = 0;
    ai.CheckMeter = 0;
}