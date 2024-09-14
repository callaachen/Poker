/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* cards.cpp: Definitions for Card methods and containers        */
/*****************************************************************/

#include "cards.h"

/*** Constructors ***/

BoardCards::BoardCards() {
    // Initialize with no cards by default
    for (int i = 0; i < BOARD_CT; ++i) {
        card[i] = Card(NoRank, NoSuit, false);
    }
}

Card::Card() : rank(NoRank), suit(NoSuit), isVisible(false), unicode(0) {} // empty Card
Card::Card(Rank r, Suit s, bool v, int u) : rank(r), suit(s), isVisible(v), unicode(0) { this->setUnicode(); } // Sets Unicode value for Card

Deck::Deck() {
    setDeck();
    srand(static_cast<unsigned>(time(0))); // Initialize random seed
}

Hand::Hand() : cards(), handType(HighCard) {} // Set default handType to HighCard

/*** Setters ***/

/** BoardCards setters **/

void BoardCards::setBoardCards(const Card* cards) {
    for (int i = 0; i < BOARD_CT; ++i) {
        card[i] = cards[i];
    }
}

void BoardCards::setCard(int index, const Card& c) {
    if (index >= 0 && index < BOARD_CT) {
        card[index] = c;
    }
}

void BoardCards::addRandomCards(Deck& deck) {
    int cardsToAdd = (card[0].getRank() == NoRank && card[0].getSuit() == NoSuit) ? 3 : 1;
    for (int i = 0; i < BOARD_CT && cardsToAdd > 0; ++i) {
        if (card[i].getRank() == NoRank && card[i].getSuit() == NoSuit) {
            card[i] = deck.getRandomCard();
            cardsToAdd--;
        }
    }
}

/** Card setters **/

void Card::setIsVisible(bool v) { isVisible = v; }

void Card::setRank(Rank r) { rank = r; }

void Card::setSuit(Suit s) { suit = s; }

void Card::setUnicode() { // Set Unicode character, based on index generated from Rank and Suit
    if (rank == NoRank || suit == NoSuit) {
        unicode = 0; // Default value when Rank/Suit not set
        return;
    }
    /* Base and adjustment values */
    int baseValue = 0x1F0A0;      // First card in Unicode matrix
    int suitOffset = suit * 0x10; // enum Suit iterates by 16^1
    int rankOffset = rank;        // enum Rank iterates by 16^0

    /* Add 1 for Queen and King to match their positions in the Unicode sequence */
    if (rank == Queen || rank == King) { rankOffset += 1; }
    else if (rank == AceHigh) { rankOffset -=13; } // Unicode is based on Ace = 1; AceHigh = 14

    /* Sum of base, Suit scalar/iterator, and Rank iterator */
    unicode = baseValue + suitOffset + rankOffset;
}

/** Deck setters **/

void Deck::setDeck() {
    int index = 0;
    for (int suit = Spades; suit <= Clubs; ++suit) { // One row per Suit
        for (int rank = Ace; rank <= King; ++rank) { // All Rank values per Suit
            card[index] = Card(static_cast<Rank>(rank), static_cast<Suit>(suit), false);
            ++index;
        }
    }
}

/** Hand setters **/

void Hand::addCard(const Card& card) {
    if (cards.size() < HAND_CT) {
        cards.push_back(card);
    }
}

void Hand::addHandCards(Deck& deck, BoardCards& boardCards) {
    if (cards.size() == 0) {
        addCard(deck.getRandomCard());
        addCard(deck.getRandomCard());
    } else if (cards.size() >= 2 && cards.size() < HAND_CT) {
        for (int i = BOARD_CT - 1; i >= 0; --i) {
            const Card& boardCard = boardCards.getCard(i);
            if (boardCard.getRank() != NoRank && boardCard.getSuit() != NoSuit) {
                addCard(boardCard);
                break;
            }
        }
    }
}

void Hand::setHandType(HandType ht) { handType = ht; }

/*** Getters ***/

/** BoardCards getters **/

const Card& BoardCards::getCard(int index) const {
    if (index < 0 || index >= BOARD_CT) {
        static Card invalidCard;
        return invalidCard;
    }
    return card[index];
}

/** Card getters **/

bool Card::getIsVisible() const { return isVisible; }
Rank Card::getRank() const { return rank; }
Suit Card::getSuit() const { return suit; }
int Card::getUnicode() const { return unicode; }

string Card::getRankName() const { // Returns print-friendly values
    switch (rank) {
        case NoRank: return "-";
        case Ace: return "Ace";
        case Two: return "Two";
        case Three: return "Three";
        case Four: return "Four";
        case Five: return "Five";
        case Six: return "Six";
        case Seven: return "Seven";
        case Eight: return "Eight";
        case Nine: return "Nine";
        case Ten: return "Ten";
        case Jack: return "Jack";
        case Queen: return "Queen";
        case King: return "King";
        default: return "-";
    }
}

string Card::getSuitName() const { // Returns print-friendly values
    switch (suit) {
        case NoSuit: return "-";
        case Spades: return "Spades";
        case Hearts: return "Hearts";
        case Diamonds: return "Diamonds";
        case Clubs: return "Clubs";
        default: return "Unknown";
    }
}

/** Deck getters **/

Card Deck::getRandomCard() {
    while (true) {
        int index = rand() % DECK_CT; // Generate a random index

        if (card[index].getRank() != NoRank && card[index].getSuit() != NoSuit) { // If selected Card index isn't empty
            Card temp = card[index]; // selected Card
            card[index] = Card(); // Replace selected Card with empty one
            return temp; // Return selected Card
        }
    }
}

/** Hand getters **/

const Card& Hand::getCard(int index) const {
    if (index < 0 || index >= static_cast<int>(cards.size())) {
        static Card invalidCard;
        return invalidCard;
    }
    return cards[index];
}

vector<Card>& Hand::getCards() {
    return cards;
}

int Hand::getCardCount() const { return cards.size(); }

HandType Hand::getHandType() const { return handType; }

/*** Methods ***/

/** BoardCards methods **/

void BoardCards::printBoardCards() const {
    for (int i = 0; i < BOARD_CT; ++i) {
        card[i].printCard();
    }
}

/** Card methods **/

/* Return Unicode value */
string Card::printCard() const {
    ostringstream oss;
    if (unicode != 0) {
        oss << static_cast<char32_t>(unicode);
    }
    return oss.str();
}

/** Deck methods **/

void Deck::printDeck() const { // Prints all the Cards by Suit
    vector<string> spades, hearts, diamonds, clubs;

    /* Sorts cards into vectors based on Suit */
    for (int i = 0; i < DECK_CT; ++i) {
        if (card[i].getSuit() == Spades) {
            spades.push_back(card[i].printCard());
        } else if (card[i].getSuit() == Hearts) {
            hearts.push_back(card[i].printCard());
        } else if (card[i].getSuit() == Diamonds) {
            diamonds.push_back(card[i].printCard());
        } else if (card[i].getSuit() == Clubs) {
            clubs.push_back(card[i].printCard());
        }
    }

    /* Print each Suit vector */
    cout << "Spades: ";
    for (const auto& c : spades) { cout << c << " "; }
    cout << endl;

    cout << "Hearts: ";
    for (const auto& c : hearts) { cout << c << " "; }
    cout << endl;

    cout << "Diamonds: ";
    for (const auto& c : diamonds) { cout << c << " "; }
    cout << endl;

    cout << "Clubs: ";
    for (const auto& c : clubs) { cout << c << " "; }
    cout << endl;
}

/** Hand methods **/

void Hand::printHand() const {
    for (const Card& card : cards) {
        card.printCard();
    }
}
