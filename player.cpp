/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* player.cpp: Definitions for Player profiles and containers    */
/*****************************************************************/

#include "cards.h"
#include "player.h"

using namespace std;

/** Constructors **/

Player::Player(string name, int number, int score, string userID, bool isPlaying, PlayerType type, DealerRole role, PlayerStatus status)
    : name(name), number(number), score(score), userID(userID), isPlaying(isPlaying), type(type), role(role), status(status), isCurrentPlayer(false) {}

/** Setters **/

void Player::addCardToHand(const Card& c) { hand.addCard(c); }
void Player::setDealerRole(DealerRole d) { this->role = d; }
void Player::setIsPlaying(bool p) { this->isPlaying = p; }
void Player::setName(string name) { this->name = name; }
void Player::setNumber(int num) { this->number = num; }
void Player::setPlayerType(PlayerType t) { this->type = t; }
void Player::setScore(int s) { this->score = s; }
void Player::setUserID(string u) { this->userID = u; }
void Player::setStatus(PlayerStatus status) { this->status = status; }
void Player::setIsCurrentPlayer(bool current) { this->isCurrentPlayer = current; }

/** Getters **/
DealerRole Player::getDealerRole() const { return role; }
bool Player::getIsPlaying() const { return isPlaying; }
string Player::getName() const { return name; }
int Player::getNumber() const { return number; }
PlayerType Player::getPlayerType() const { return type; }
int Player::getScore() const { return score; }
PlayerStatus Player::getStatus() const { return status; }
string Player::getUserID() const { return userID; }
bool Player::getIsCurrentPlayer() const { return isCurrentPlayer; }
/* Player getters that return print-friendly values for objects */
string Player::getDealerRoleName() const {
    switch (role) {
        case NoRole: return "None";
        case Dealer: return "Dealer";
        case LittleBlind: return "Little Blind";
        case BigBlind: return "Big Blind";
        case Dealer2P: return "Dealer and Little Blind";
        default: return "None";
    }
} // end Player::getDealerRoleName()

int Player::getDealerRoleNum() const { return static_cast<int>(role); }

const Hand& Player::getHand() const { return hand; }

/** Methods **/

/* AllPlayers methods */

bool AllPlayers::addPlayer(const Player& player) {
    if (players.size() < MAX_PLAYERS) {
        players.push_back(player);
        return true;
    } else {
        return false;
    }
} // end addPlayer()

vector<Player>& AllPlayers::getPlayers() { return players; }

const vector<Player>& AllPlayers::getPlayers() const { return players; }

/* Remove Player by userID */
bool AllPlayers::removePlayer(const string& userID) {
    for (auto it = players.begin(); it != players.end(); ++it) {
        if (it->getUserID() == userID) {
            players.erase(it);
            return true; // userID found
        }
    }
    return false; // userID not found
}

/* Player methods */

void Player::printPlayer() const {
    cout << "P" << number << ": " << name << std::endl; // number and name
    cout << "UserID: " << userID << endl;
    cout << "Score: " << score << endl << endl;
    cout << "Role: " << getDealerRoleName() << endl;

    cout << "Hole Cards: " << endl;
    for (int i = 0; i < hand.getCardCount(); ++i) {
        const Card& card = hand.getCard(i);
        cout << "Card " << i + 1 << ": " << card.getRankName() << " of " << card.getSuitName() << endl;
    }
}// end printPlayer()