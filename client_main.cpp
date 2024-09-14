/*******************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                              */
/* Authors: The Lucky Sevens (Team 7)                              */
/* main.cpp: Client side Main Program File for Poker Game Software */
/*******************************************************************/

#include <cstring>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>
#include <stdlib.h>

//#include "ai.h"
#include "cards.h"
#include "constants.h"
#include "game.h"
#include "gui.h"
#include "handEvaluator.h"
#include "player.h"
#include "poker_comm.h"

using namespace std;

const char *Program; /* program name for debugging */

int main(int argc, char *argv[]) {
    // client connect to server starts here
    int PortNo;                         /* port number */
    struct sockaddr_in ServerAddress;   /* server address we connect with */
    struct hostent *Server;             /* server host */
    Program = argv[0];                  /* publish program name (for diagnostics) */

    string name;                        /* e.g. Brian */
    int randomNum;                      /* random number to make user ID */
    char special;                       /* special character to go to ID */
    char specials[8] = {'!', '@', '#', '$', '%', '^', '&', '*'};    /* possible special characters to make user ID */
    string userID;                      /* the user ID */
#ifdef DEBUG
    printf("%s: Starting...\n", argv[0]);
#endif
    if (argc < 3) {   
        fprintf(stderr, "Usage: %s hostname port\n", Program);
        exit(10);
    }
    Server = gethostbyname(argv[1]);
    if (Server == NULL) {   
        fprintf(stderr, "%s: No such host named '%s'\n", Program, argv[1]);
        exit(10);
    }
    PortNo = atoi(argv[2]);
    if (PortNo <= 2000) {   
        fprintf(stderr, "%s: Invalid port number %d, should be >2000\n", Program, PortNo);
        exit(10);
    }
    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(PortNo);
    ServerAddress.sin_addr = *(struct in_addr*)Server->h_addr_list[0]; /* why can we typecast in_addr to char */
    // ends here

    cout<<"Enter your name (ONLY WORDS) (e.g. Brian): "<<endl;
    getline(cin, name);
    srand(time(NULL));
    randomNum = 1000 + rand() % 9000;             // numbers go from 1000 to 9999
    int a = rand()%8;
    special = specials[a];
    userID = name + to_string(randomNum) + special;

    /* Initialize GTK */
    gtk_init(&argc, &argv);

    /* Initialize dummy data for Players */
    Player p1("Peter the Anteater", 1, 100, "P1", true, Human, Dealer);
    p1.addCardToHand(Card(Ace, Spades, true));
    p1.addCardToHand(Card(Queen, Hearts, true));

    Player p2("Rainer Döemer", 2, 100, "P2", true, Human, LittleBlind);
    p2.addCardToHand(Card(NoRank, NoSuit, false));
    p2.addCardToHand(Card(NoRank, NoSuit, false));

    Player p3("Kirby Burke", 3, 100, "P3", true, Human, BigBlind);
    p3.addCardToHand(Card(NoRank, NoSuit, false));
    p3.addCardToHand(Card(NoRank, NoSuit, false));

    Player p4("Calla Chen", 4, 100, "P4", true, Human, NoRole);
    p4.addCardToHand(Card(NoRank, NoSuit, false));
    p4.addCardToHand(Card(NoRank, NoSuit, false));
    p4.setIsCurrentPlayer(true);

    Player p5("Susung Kang", 5, 100, "P5", true, Human, NoRole);
    p5.addCardToHand(Card(NoRank, NoSuit, false));
    p5.addCardToHand(Card(NoRank, NoSuit, false));

    Player p6("Quang Phuc Le", 6, 100, "P6", true, Human, NoRole);
    p6.addCardToHand(Card(NoRank, NoSuit, false));
    p6.addCardToHand(Card(NoRank, NoSuit, false));

    Player p7("Taha Zeeshan", 7, 100, "P7", true, Human, NoRole);
    p7.addCardToHand(Card(NoRank, NoSuit, false));
    p7.addCardToHand(Card(NoRank, NoSuit, false));

    Player p8("Christian Zeidan", 8, 100, "P8", true, Human, NoRole);
    p8.addCardToHand(Card(NoRank, NoSuit, false));
    p8.addCardToHand(Card(NoRank, NoSuit, false));

    AllPlayers allPlayers;
    allPlayers.addPlayer(p1);
    allPlayers.addPlayer(p2);
    allPlayers.addPlayer(p3);
    allPlayers.addPlayer(p4);
    allPlayers.addPlayer(p5);
    allPlayers.addPlayer(p6);
    allPlayers.addPlayer(p7);
    allPlayers.addPlayer(p8);

    /*** Game module integration ***/
    /* Replace the preceding with this when the Game module is live */
    //Game game(allPlayers.getPlayers());
    vector <Player*> players;
    players.push_back(&p1);
    players.push_back(&p2);
    players.push_back(&p3);
    players.push_back(&p4);
    players.push_back(&p5);
    players.push_back(&p6);
    players.push_back(&p7);
    players.push_back(&p8);

    Network network;

    Game *game = new Game(players,network);

    /* Create the Game Window */
    GameScreen gameScreen(CANVAS_W, CANVAS_H, "Texas Hold 'Em", game);

    /* Initialize BoardCards */
    BoardCards boardCards;
    boardCards.setCard(0, Card(Jack, Diamonds, true));
    boardCards.setCard(1, Card(Two, Clubs, true));
    boardCards.setCard(2, Card(Six, Clubs, true));
    boardCards.setCard(3, Card(Six, Hearts, true));
    boardCards.setCard(4, Card(Six, Spades, true));

    /* Initialize Boxes */
    gameScreen.initializeBoxes(allPlayers, boardCards);

    /* Dummy text for ConsoleBox */
    gameScreen.getConsoleBox()->addLogMessage("Connected to server.");
    gameScreen.getConsoleBox()->addLogMessage("P1 raised the bet to 10.");
    gameScreen.getConsoleBox()->addLogMessage("P2 folded.");
    gameScreen.getConsoleBox()->addLogMessage("P3 called.");
    gameScreen.getConsoleBox()->addLogMessage("P4 checked.");
    gameScreen.getConsoleBox()->addLogMessage("P5 went all-in.");
    gameScreen.getConsoleBox()->addLogMessage("P6 won the hand.");
    gameScreen.getConsoleBox()->addLogMessage("P7 lost the hand.");
    gameScreen.getConsoleBox()->addLogMessage("P8 is the new dealer.");
    gameScreen.getConsoleBox()->addLogMessage("New round started.");
    gameScreen.getConsoleBox()->addLogMessage("Server disconnected.");
    gameScreen.getConsoleBox()->addLogMessage("Reconnecting to server...");
    gameScreen.getConsoleBox()->addLogMessage("P1 called.");
    gameScreen.getConsoleBox()->addLogMessage("P2 raised the bet to 20.");
    gameScreen.getConsoleBox()->addLogMessage("P3 folded.");


    /* Initialize and render the GameScreen */
    gameScreen.initialize();
    gameScreen.show();

#ifdef DEBUG
    /* Input testing functions here. */
#endif

    /* Main GTK Loop */
    gtk_main();

    return 0;
}
