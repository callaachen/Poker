#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "cards.h"
#include "handEvaluator.h"
#include "network.h"
#include <vector>
#include <algorithm>

class Network;

class Game
{
public:
    // Constructor to initialize the game with a list of players
    Game(std::vector<Player *> players, Network& network);

    // Method to start the game
    void start();

    // Method to end the game
    void end();

    // Method to place a bet for a player
    void placeBet(Player *player, int player_id, int amount);

    // Method to fold a player's hand
    void fold(Player *player, int player_id);

    // Method to proceed to the next hand
    void nextHand();

    // Method to initialize the game (shuffle deck, etc.)
    void initializeGame();

    // Method to set the initial dealer and blinds
    void setInitialDealerAndBlinds();

    // Method to rotate the dealer and blinds for the next hand
    void rotateBlindsAndDealer();

    // Method to deal cards to players
    void dealCards();

    // Method to broadcast the game state to all players
    void broadcastGameState();

    // Method to update the game state after a player's action
    void updateGameState();

    // Method to proceed to the next player's turn
    void nextTurn();

    // Method to set the initial blind bets
    void setBlindBets();

    // Method to handle player choices during their turn
    void PlayerChoice(int choice, int raise_amount);

    // Method to handle the first betting round
    void Round1(int choice, int raise_amount);

    // Method to handle the second betting round (flop)
    void Round2(int choice, int raise_amount);

    // Method to handle the third betting round (turn)
    void Round3(int choice, int raise_amount);

    // Method to handle the fourth betting round (river)
    void Round4(int choice, int raise_amount);

    // Method to handle the showdown phase
    void ShowDown();

    // Method to distribute winnings to the winner
    void distributeWinnings(Player *winner, int winner_id);

    // Method to evaluate and determine the best hand
    void evaluateBestHand();

    // Method to check if enough players are available to continue the game
    bool EnoughPlayersAvailable();

    // Getter methods
    int getPlayerBet(int index) const;
    int getPlayerMoney(int index) const;
    int getRound() const;
    int getHand() const;
    int getPot() const;
    int getCurrentPlayerIndex() const;
    const BoardCards& getCommunityCards() const;

    // New class for managing game state
    class GameState
    {
    public:
        GameState(Game& game);
        void initialize();
        void updatePlayerBet(int index, int bet);
        void updatePlayerMoney(int index, int money);
        void updateRound(int round);
        void updatePot(int pot);
        void updateHand(int hand);
        void updateCurrentPlayerIndex(int index);

        int getPlayerBet(int index) const;
        int getPlayerMoney(int index) const;
        int getRound() const;
        int getPot() const;
        int getHand() const;
        int getCurrentPlayerIndex() const;

    private:
        Game& game;
    };

private:
    std::vector<Player *> players; // List of players in the game
    int dealerIndex; // Index of the dealer
    int smallBlindIndex; // Index of the small blind player
    int bigBlindIndex; // Index of the big blind player
    Deck deck; // Deck of cards used in the game
    int pot; // Total amount of money in the pot
    int round; // Current round of betting
    int current_playerIndex; // Index of the current player
    int startingIndex; // Index of the starting player for the current round
    int hand; // Current hand number
    Network& network; // Network instance for sending messages to clients
    BoardCards CommunityCards; // Community cards on the board
    int BoardCardIndex; // Index of the current community card
    HandEvaluator handEval; // HandEvaluator instance for evaluating hands
    std::string message; // Message to be passed to the network
    int *player_money; // Array to store money for each player
    int *player_bet; // Array to store bets for each player
};

#endif
