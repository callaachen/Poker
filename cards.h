/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* cards.h: Declarations for Card methods and containers         */
/*****************************************************************/

#ifndef CARDS_H
#define CARDS_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "constants.h"

using namespace std;

/** Class and structure declarations **/

class Deck;
struct BoardCards;
struct Card;
struct Hand;

/*** Enumerators ***/

enum Rank {
    NoRank = -1,
    Ace = 1,
    Two = 2,
    Three = 3,
    Four = 4,
    Five = 5,
    Six = 6,
    Seven = 7,
    Eight = 8,
    Nine = 9,
    Ten = 10,
    Jack = 11,
    Queen = 12, // Unicode = 13
    King = 13,   // Unicode = 14
    AceHigh = 14 // for evalHand()
};

enum Suit {
    NoSuit = -1,
    Spades = 0,
    Hearts = 1,
    Diamonds = 2,
    Clubs = 3
};

enum HandType {
    HighCard = 0,
    OnePair = 1,
    TwoPairs = 2,
    ThreeOfAKind = 3,
    Straight = 4,
    Flush = 5,
    FullHouse = 6,
    FourOfAKind = 7,
    StraightFlush = 8,
    RoyalFlush = 9
};

/** Card definition prioritized due to dependencies **/
struct Card {
private:
    Rank rank;
    Suit suit;
    bool isVisible = false; // Changed default visibility to false
    int unicode;

public:
    /* Constructors */
    Card();
    Card(Rank r, Suit s, bool isVisible = false, int unicode = 0);

    /* Setters */
    void setIsVisible(bool v);
    void setRank(Rank r);
    void setSuit(Suit s);
    void setUnicode();

    /* Getters */
    bool getIsVisible() const;
    Rank getRank() const;
    Suit getSuit() const;
    int getUnicode() const;

    /* Getters that return print-friendly values for objects */
    string getRankName() const;
    string getSuitName() const;

    string printCard() const;
};


class Deck {
private:
    Card card[DECK_CT];

public:
    /* Constructor */
    Deck();

    /* Setters */
    void setDeck();

    /* Getters */
    Card getRandomCard();

    /* Methods */
    void printDeck() const;
};


struct BoardCards {
private:
    Card card[BOARD_CT];

public:
    /* Constructor */
    BoardCards();

    /* Setters */
    void setBoardCards(const Card* cards);
    void setCard(int index, const Card& c);
    void addRandomCards(Deck& deck);

    /* Getters */
    const Card& getCard(int index) const;

    /* Methods */
    void printBoardCards() const;
};

struct Hand {
private:
    vector<Card> cards;
    HandType handType;

public:
    /* Constructor */
    Hand();

    /* Setters */
    void addCard(const Card& card);
    void addHandCards(Deck& deck, BoardCards& boardCards);
    void setHandType(HandType ht);

    /* Getters */
    const Card& getCard(int index) const;
    vector<Card>& getCards();
    int getCardCount() const;
    HandType getHandType() const;

    /* Methods */
    void printHand() const;
};

#endif // CARDS_H
