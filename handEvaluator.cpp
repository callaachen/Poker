/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* handEvaluator.cpp: Definitions for methods to determine       */
/*    winning hand types and select winning player(s)            */
/*****************************************************************/

#include "handEvaluator.h"


/*** Evaluate HandType methods ***/


/* PRIMARY EVALUATION METHOD: Determine a single Player's HandType */

void HandEvaluator::evalHandType(Hand& hand) {
    /* Verify Hand contains at least 2 Cards */
    if (hand.getCardCount() < 2) { return; }
    
    /* Set all Ace Ranks to AceHigh */
    for (int i = 0; i < hand.getCardCount(); ++i) {
        Card& card = const_cast<Card&>(hand.getCard(i));
        if (card.getRank() == Ace) {
            card.setRank(AceHigh);
        }
    }

    bool endAnalysis = false; // Terminator for advanced analysis

    evalNumOfAKind(hand); // Check for HandTypes with duplicates: Pairs, ThreeOfAKind, etc.

    /* Skip Straight and Flush checks for Hands with less than 5 distinct Rank Cards*/
    if (hand.getCardCount() < 5) { endAnalysis = true; } // Less than 5 total Cards
    else if (hand.getCardCount() == 5 && hand.getHandType() != HighCard) { endAnalysis = true; }    // 5-Card Hand, but less than 5 distinct Ranks
    else if (hand.getCardCount() == 6 && hand.getHandType() > OnePair) { endAnalysis = true; }      // 6-Card Hand
    else if (hand.getCardCount() == 7 && hand.getHandType() > ThreeOfAKind) { endAnalysis = true; } // 7-Card Hand

    if (!endAnalysis) {
        evalStraight(hand); // Straight
        evalFlush(hand);    // Flush, StraightFlush, RoyalFlush
    }

    //HandType handType = hand.getHandType();

    /* Sort Hand by Duplicates if HandType isn't already one of the following types that have no duplicate Cards*/
    if (hand.getHandType() != HighCard && hand.getHandType() != Straight && hand.getHandType() != Flush && hand.getHandType() != StraightFlush && hand.getHandType() != RoyalFlush) { 
        sortNumOfAKind(hand);
    }
}//end HandEvaluator::evalHandType()


/** Evaluate Hand Type HELPER METHODS **/

/* Evaluate if hand contains a flush */
void HandEvaluator::evalFlush(Hand& hand) {
    if (hand.getCardCount() < 5) {
        return; // Minimum of 5 Cards needed for a Flush
    }

    int count[4] = {0, 0, 0, 0}; // Suit counter
    Suit flushSuit = NoSuit; // Suit enum of Flush

    for (int i = 0; i < hand.getCardCount(); ++i) {
        count[hand.getCard(i).getSuit()]++; // Count Suits
    }

    for (int i = 0; i < 4; ++i) { // Flush if 5+ of one Suit
        if (count[i] >= 5) {
            flushSuit = static_cast<Suit>(i);
            break;
        }
    }

    if (flushSuit != NoSuit) { // Sort Cards and Check for higher HandTypes
        vector<Card> flushCards;
        for (const Card& card : hand.getCards()) {
            if (card.getSuit() == flushSuit) {
                flushCards.push_back(card);
            }
        }

        sort(flushCards.begin(), flushCards.end(), [](const Card& a, const Card& b) {
            return a.getRank() > b.getRank();
        });

        if (flushCards.size() >= 5) {
            bool isStraightFlush = true;
            for (int i = 0; i < 4; ++i) {
                if (flushCards[i].getRank() != flushCards[i + 1].getRank() + 1) {
                    isStraightFlush = false;
                    break;
                }
            }

            if (isStraightFlush) {
                if (flushCards[0].getRank() == AceHigh) {
                    hand.setHandType(RoyalFlush);
                } else {
                    hand.setHandType(StraightFlush);
                }
            } else {
                hand.setHandType(Flush);
            }
        } else {
            hand.setHandType(Flush);
        }
    }
}//end HandEvaluator::evalFlush()



/* Evaluate the number of each kind in the hand */

void HandEvaluator::evalNumOfAKind(Hand& hand) {
    /* Verify Hand contains at least 2 Cards */
    if (hand.getCardCount() < 2) { return; }

    int rankCount[15] = {0};  // 15 entries to cover Ace (1) through AceHigh(14)

    for (const Card& card : hand.getCards()) { rankCount[card.getRank()]++; } // Count each Rank occurence

    /* Counters for duplicate Ranks */
    int fourOfAKind = 0;
    int threeOfAKind = 0;
    int pairs = 0;

    /* Populate duplicate counters */
    for (int count : rankCount) { 
        if (count == 4) { fourOfAKind++; }
        else if (count == 3) { threeOfAKind++; }
        else if (count == 2) { pairs++; }
    }

    /* Assign HandType based on duplicate counts, from highest to lowest HandType */
    if (fourOfAKind > 0) { hand.setHandType(FourOfAKind); }
    else if (threeOfAKind > 0 && pairs > 0) { hand.setHandType(FullHouse); }
    else if (threeOfAKind > 0) { hand.setHandType(ThreeOfAKind); }
    else if (pairs > 1) { hand.setHandType(TwoPairs); } 
    else if (pairs == 1) { hand.setHandType(OnePair); }
    else { hand.setHandType(HighCard); } // Default HandType; no duplicate Cards
    
}//end HandEvaluator::evalNumOfAKind()



/* Evaluate if hand contains a Straight */
void HandEvaluator::evalStraight(Hand& hand) {
    if (hand.getCardCount() < 5) { return; } // Straight requires at least 5 Cards

    sortDescending(hand); // Sort the cards in descending order

    // Check for exception HandTypes: Low Straight and Low StraightFlush
    bool isFlush = true;         // Set to false if Hand fails checks
    bool isLowStraight = true;   // Set to false if Hand fails checks
    int lowRankCount[6] = { 0 }; // Lowest 5 Card Ranks (+1 to match Rank)
    int suitCount[4] = { 0 };    // Suit enum 

    for (const Card& card : hand.getCards()) {
        if (card.getRank() < 6 || card.getRank() == 14) { // Count only Ace to Five
            int rankIndex = (card.getRank() == 14) ? 1 : card.getRank(); // Count AceHighs as Aces
            lowRankCount[rankIndex]++;
            suitCount[card.getSuit()]++;
        }
    }

    for (int i = 1; i <= 5; ++i) { // Confirm every Low Rank present (1 to 5)
        if (lowRankCount[i] == 0) {
            isLowStraight = false; // Not a Low Straight if any low Rank missing
            break;
        }
    }

    if (isLowStraight) { // Low Straight confirmed
        hand.setHandType(Straight);

        // Check for Flush
        int flushSuit = -1;
        for (int suit = 0; suit < 4; ++suit) {
            if (suitCount[suit] == 5) { // Set flushSuit same for all 5 Cards
                flushSuit = suit;
                break;
            }
        }

        if (flushSuit == -1) { isFlush = false; } // No Suit set

        if (isFlush) { // Straight + Flush = StraightFlush
            hand.setHandType(StraightFlush);

            for (Card& card : hand.getCards()) { // Set HighAce of flushSuit to Ace
                if (card.getRank() == 14 && card.getSuit() == flushSuit) { 
                    card.setRank(Ace);
                }
            }
        } else { // Low Straight but not a StraightFlush
            hand.setHandType(Straight);
            // Set any HighAce to Ace
            for (Card& card : hand.getCards()) {
                if (card.getRank() == 14) {
                    card.setRank(Ace);
                    break; // Only set the first HighAce found
                }
            }
            sortStraight(hand);
        }
        return; // Return after handling Low Straight/Flush
    }// END Exception Checks

    // General Straight Check
    int consecutiveCount = 1; // Count of consecutive cards
    for (int i = 1; i < hand.getCardCount(); ++i) {
        if (hand.getCard(i - 1).getRank() == hand.getCard(i).getRank() + 1) {
            consecutiveCount++;
            if (consecutiveCount == 5) {
                hand.setHandType(Straight);
                return;
            }
        } else if (hand.getCard(i - 1).getRank() != hand.getCard(i).getRank()) {
            consecutiveCount = 1; // Reset count if ranks are not consecutive
        }
    }
}//end HandEvaluator::evalStraight



/* Sort Cards from low to high Rank */

void HandEvaluator::sortAscending(Hand& hand) { 
    /* Use pre-defined C++ command to evaluate and sort Cards */
    sort(hand.getCards().begin(), hand.getCards().end(), [](const Card& a, const Card& b) {
        return a.getRank() < b.getRank();
    });
}//end HandEvaluator::sortAscending()



/* Sort Cards from high to low Rank */

void HandEvaluator::sortDescending(Hand& hand) {
    /* Use pre-defined C++ command to evaluate and sort Cards */
    sort(hand.getCards().begin(), hand.getCards().end(), [](const Card& a, const Card& b) {
        return a.getRank() > b.getRank();
    });
}//end HandEvaluator::sortDescending()


/** Evaluate Hand Type META HELPER METHODS (helper methods FOR other helper methds) **/



/* META-HELPER METHOD for evalFlush(): Push Flush outlier Cards to back of Hand */

void HandEvaluator::sortFlush(Hand& hand, Suit flushSuit) {
    /* Verify Hand contains MORE THAN five Cards */
    if (hand.getCardCount() <= 5) { return; } // Hand with a 5-Card Flush has no outliers; Hand with less than 5 isn't a Flush

    /* Sort Cards into descending order for analysis -- fallback in case something went weird in evalFlush() */
    sortDescending(hand);

    /* Create a partition of the Flush Cards and shift it to the front of Hand */
    stable_partition(hand.getCards().begin(), hand.getCards().end(), [flushSuit](const Card& card) {
        return card.getSuit() == flushSuit;
    });
}//end HandEvaluator::sortFlush()



/* META-HELPER METHOD for evalNumOfAKind(): Frontload strongest matches and push outliers to back */

void HandEvaluator::sortNumOfAKind(Hand& hand) {
    /* Verify Hand contains more than 2 Cards (2 matching Cards is OnePair; doesn't need sorting) */
    if (hand.getCardCount() < 2) { return; }

    /* Get Hand Cards and sort into descending order */
    vector<Card> sortedHand = hand.getCards();
    sortDescending(hand);

    /* Array to store Rank counts and sets of Cards with matching Ranks */
    int rankCount[14] = {0};  // Based on enum Rank (1 to 13)
    vector<Card> fourOfAKind, threeOfAKind, pairs, singles;

    /* Count instances of each Rank */
    for (const Card& card : sortedHand) { rankCount[card.getRank()]++; }

    /* Copy Cards into sets based on rankCount */
    for (const Card& card : sortedHand) {
        int rank = card.getRank();

        if (rankCount[rank] == 4) { fourOfAKind.push_back(card); } // 4 Cards with this Rank

        else if (rankCount[rank] == 3) { threeOfAKind.push_back(card); } // 3 Cards with this Rank
        
        else if (rankCount[rank] == 2) { pairs.push_back(card); } // 2 Cards with this Rank
        
        else { singles.push_back(card); } // Zero or one of this Rank
    }

    /* Clear Hand and repopulate with matching sets of Cards -- in order of HandType -- followed by outliers */
    hand.getCards().clear(); // Clear Hand
    hand.getCards().insert(hand.getCards().end(), fourOfAKind.begin(), fourOfAKind.end());
    hand.getCards().insert(hand.getCards().end(), threeOfAKind.begin(), threeOfAKind.end());
    hand.getCards().insert(hand.getCards().end(), pairs.begin(), pairs.end()); // Highest of TwoPairs frontloaded from sortDescending() method
    hand.getCards().insert(hand.getCards().end(), singles.begin(), singles.end()); // Outliers also in descending order
}//end HandEvaluator::sortNumOfAKind()



/* META-HELPER METHOD: evalStraight(): Push Straight outlier Cards to back of Hand */

void HandEvaluator::sortStraight(Hand& hand) {
    /* Verify Hand contains MORE THAN five Cards */
    if (hand.getCardCount() <= 5) { return; } // Hand with a 5-Card Straight is already in order; Hand with less than 5 isn't a Straight

    vector<Card> straightCards; // Cards in sequential order
    vector<Card> outlierCards;  // Cards not in sequential order
    bool firstCardSet = false;  // Control switch for evaluation of first Card

    /* Compare each Card Rank with previous Card's Rank */
    for (int i = 1; i < hand.getCardCount(); ++i) { // Start with second Card

        /* If current Card Rank is one less than previous Card Rank, copy to straightCards vector */
        if (!straightCards.empty() && hand.getCard(i).getRank() == straightCards.back().getRank() - 1) {
            straightCards.push_back(hand.getCard(i));
        } 
        
        /* If current Card is out of sequence, copy to outlierCards vector */
        else { outlierCards.push_back(hand.getCard(i)); }

        /* End after 5 highest value Straight Cards are identified */
        if (straightCards.size() == 5) { break; }

        /* Evaluate first Card after straightCards has two entries */
        if (!firstCardSet && straightCards.size() == 2) {

            /* If first Card is one Rank higher than first Card in straightCards, copy it to front of straightCards */
            if (hand.getCard(0).getRank() == straightCards.front().getRank() + 1) {
                straightCards.insert(straightCards.begin(), hand.getCard(0));
            }
            
            /* If not, copy first Card to front of outlierCards */
            else { outlierCards.insert(outlierCards.begin(), hand.getCard(0)); }

            firstCardSet = true; // Set to prevent this evaluation from firing again
        }//endif
    }//endfor

    /* For 7-Card Hand, sort the two outliers in descending order */
    if (hand.getCardCount() == 7) {
        sort(outlierCards.begin(), outlierCards.end(), [](const Card& a, const Card& b) {
            return a.getRank() > b.getRank();
        });
    }

    /* Replace Hand Cards with straightCards, followed by outlierCards */
    hand.getCards().clear();
    hand.getCards().insert(hand.getCards().end(), straightCards.begin(), straightCards.end());
    hand.getCards().insert(hand.getCards().end(), outlierCards.begin(), outlierCards.end());
}//end HandEvaluator::sortStraight()



/*** Compare Hands Method ***/



/* Determine best Hand(s) from AllPlayers after showdown round */

void HandEvaluator::compareHandTypes(AllPlayers& allPlayers) {
    HandType highestHandType = HighCard; // Value of highest HandType in playingPlayers

    /* Collect all active Players and determine the highest HandType */
    for (Player& player : allPlayers.getPlayers()) {
        if (player.getIsPlaying()) {
            HandType handType = player.getHand().getHandType(); // Add active Player's HandType
            if (handType > highestHandType) {
                highestHandType = handType; // Set highestHandType, if Player has highest HandType
            }
        }
    }

    /* Set PlayerLost for Players whose HandType is inferior to highestHandType */
    for (Player& player : allPlayers.getPlayers()) { 
        if (player.getIsPlaying() && player.getHand().getHandType() < highestHandType) {
            player.setStatus(PlayerLost); // Set status to PlayerLost
        }
    }

    /* The lone Player with the highest HandType wins */
    int activePlayersCount = 0;
    Player* potentialWinner = nullptr;

    for (Player& player : allPlayers.getPlayers()) {
        if (player.getIsPlaying() && player.getStatus() != PlayerLost) {
            activePlayersCount++;
            potentialWinner = &player;
        }
    }

    if (activePlayersCount == 1) {
        potentialWinner->setStatus(PlayerWon);
        return;
    }

    /* For multiple Players with highest HandType, compare Card Ranks */

    for (int i = 0; i < 5; ++i) { // First five cards only

        Rank highestRank = NoRank; // Reset high Card for each iteration

        /* Determine the highest rank for the ith card */
        for (Player& player : allPlayers.getPlayers()) {
            if (player.getIsPlaying() && player.getStatus() != PlayerLost) {
                Rank rank = player.getHand().getCard(i).getRank();
                if (rank > highestRank) {
                    highestRank = rank;
                }
            }
        }

        /* Set PlayerLost for those who do not have the highest rank for the ith card */
        for (Player& player : allPlayers.getPlayers()) {
            if (player.getIsPlaying() && player.getStatus() != PlayerLost && player.getHand().getCard(i).getRank() < highestRank) {
                player.setStatus(PlayerLost);
            }
        }

        /* The lone Player with the highest Card Rank wins */
        activePlayersCount = 0;
        potentialWinner = nullptr;

        for (Player& player : allPlayers.getPlayers()) { 
            if (player.getIsPlaying() && player.getStatus() != PlayerLost) {
                activePlayersCount++;
                potentialWinner = &player;
            }
        }

        if (activePlayersCount == 1) { // One Player left
            potentialWinner->setStatus(PlayerWon); // Set status
            for (Player& player : allPlayers.getPlayers()) { 
                if (player.getStatus() != PlayerWon) {
                    player.setStatus(PlayerLost);
                }
            }
            return;
        }
    }

    /* If all five Cards match, all remaining Players win */
    for (Player& player : allPlayers.getPlayers()) { 
        if (player.getIsPlaying() && player.getStatus() != PlayerLost) {
            player.setStatus(PlayerWon);
        }
    }
}//end HandEvaluator::compareHandTypes()
