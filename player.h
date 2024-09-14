/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* player.h: Declarations for Player profiles and containers     */
/*****************************************************************/

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include "constants.h"
#include "cards.h"

using namespace std;

/** Class and structure declarations **/

class AllPlayers;
class Player;

/** Enumerators **/

/* Designate dealer and blinds */
enum DealerRole {
    NoRole = -1,
    Dealer = 0,
    LittleBlind = 1,
    BigBlind = 2,
    Dealer2P = 3 // Dealer is also Little Blind in 2-player mode
};

/* For WinningHand analysis */
enum PlayerStatus {
    PlayerLost = -1,  // Folded or losing hand
    Playing = 0,      // Win/lose status pending
    PlayerWon = 1     // Has WinningHand
};//end PlayerStatus

enum PlayerType { Human = 0, Computer = 1 };

/** Classes **/

class AllPlayers {
private:
    vector<Player> players;

public:
    /* Methods */
    bool addPlayer(const Player& player);
    bool removePlayer(const string& userID); // For Server team to pull Players by userID
    vector<Player>& getPlayers();
    const vector<Player>& getPlayers() const; // Keep const version for read-only access
}; // end AllPlayers

class Player {
private:
    string name;
    int number; // table position: 1 to 8
    int score;
    string userID;
    bool isPlaying; // 0 = fold; 1 = active
    PlayerType type;
    DealerRole role;
    PlayerStatus status;
    Hand hand;
    bool isCurrentPlayer = false; // Initialize to false by default

public:
    /* Constructor */
    Player(string name, int number, int score, string userID, bool isPlaying, PlayerType type, DealerRole role, PlayerStatus status = Playing);

    /* Setters */
    void addCardToHand(const Card& c);
    void setDealerRole(DealerRole d);
    void setIsPlaying(bool p);
    void setName(string name);
    void setNumber(int num);
    void setPlayerType(PlayerType t);
    void setScore(int s);
    void setStatus(PlayerStatus status);
    void setUserID(string u);
    void setIsCurrentPlayer(bool current);

    /* Getters */
    DealerRole getDealerRole() const;
    const Hand& getHand() const;
    bool getIsPlaying() const;
    string getName() const;
    int getNumber() const;
    PlayerType getPlayerType() const;
    int getScore() const;
    PlayerStatus getStatus() const;
    string getUserID() const;
    bool getIsCurrentPlayer() const;

    /* Getters that return print-friendly values for objects */
    string getDealerRoleName() const;
    int getDealerRoleNum() const;

    /* Methods */
    void printPlayer() const;
}; // end Player
#endif // PLAYER_H