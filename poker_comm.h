/******************************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                                         */
/* Authors: The Lucky Sevens (Team 7)                                         */
/* poker_comm.h: The header file for the communications functions             */
/******************************************************************************/

#ifndef POKER_COMM_H
#define POKER_COMM_H

#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "player.h" 
#include "cards.h"
#include "game.h"
#include "network.h"

class Game;

std::string serializePlayer(const Player& player);

void sendPlayerUpdates(int clientSocketFD, const AllPlayers& allPlayers);

Player deserializePlayer(const std::string& serializedPlayer);

void receivePlayerUpdates(int serverSocketFD, AllPlayers& allPlayers);

/* client actions*/
string bet_raiseToServer(string userID, int amount);       // sends betting information to server

string foldToServer(string userID, int amount);            // sends folding information to server

string check_callToServer(string userID, int amount);      // sends checking information to server

string quitToServer(string userID, int amount);            // sends quitting information to server

/* server actions */
void updateGameStatus();                // commits global changes to all players

void sendToPlayerNo(int index);         // sends specific info to the designated player

std::string messageToClient(std::string userID, std::string action, int amount);

void infoFromServer(int serverSocket);

void infoFromCLient(int clientSocket);

#endif