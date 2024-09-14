/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* handEvaluator.h: Declarations for methods to determine        */
/*    winning hand types and select winning player(s)            */
/*****************************************************************/

#ifndef HANDEVALUATOR_H
#define HANDEVALUATOR_H

#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include "constants.h"
#include "cards.h"
#include "player.h"


/* Methods to evaluate and compare Player Hands */

struct HandEvaluator {
private:
    /* Helper methods for evalHandType() */
    
    void evalFlush(Hand& hand);
    void evalNumOfAKind(Hand& hand); // Identify Pairs, ThreeOfAKind, etc.
    void evalStraight(Hand& hand);
    void sortAscending(Hand& hand);  // Sort Cards from low to high Rank
    void sortDescending(Hand& hand); // Sort Cards from high to low Rank

    /* META HELPER methods for evalHandType() (helper methods FOR other helper methods) */

    void sortFlush(Hand& hand, Suit flushSuit);      // Push Flush outlier Cards to back of Hand
    void sortNumOfAKind(Hand& hand); // Push Straight outlier Cards to back of Hand
    void sortStraight(Hand& hand);   // Frontload strongest matches and push outliers to back

public:
    void compareHandTypes(AllPlayers& allPlayers);         // Compare AllPlayers' Hands
    void evalHandType(Hand& hand);   // Evaluate one Player's Hand
};

#endif // HANDEVALUATOR_H
