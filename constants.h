/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* constrants.h: Global macros for Poker game modules            */
/*****************************************************************/

#ifndef CONSTANTS_H
#define CONSTANTS_H

/** Poker (Client-side Main) **/


/** Main (Server-side) **/


/** Cards **/
#define BOARD_CT 5 // Number of Cards in Board
#define DECK_CT 52 // Number of Cards in Deck
#define HAND_CT 7  // Number of Cards in Hand: Player Cards plus Board Cards
#define RANK_CT 4  // Number of possible Card Ranks
#define SUIT_CT 4  // Number of possible Card Suits


/** Game **/


/** GUI **/
#define CANVAS_W 800 // Canvas width in pixels
#define CANVAS_H 600 // Canvas height in pixels
#define CARD_SZ 30   // Card font size
#define CARD_BG_W 40 // Card background width
#define CARD_BG_H 50 // Card background height; GRID_SQ * 2
#define CARD_BACK 127136 // Unicode value for backside of card
#define GRID_SQ 25   // Grid square width/height in pixels
#define LOGO_PATH "logo.png" // LogoBox image


/** Hand Evaluator */

/** Network **/


/** Player **/
#define MAX_PLAYERS 8 // Maximum number of Players
#define COWBOY_EMOJI  "\xF0\x9F\xA4\xA0" // Unicode value for cowboy emoji

#endif //CONSTANTS_H
