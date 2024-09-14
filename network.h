#ifndef NETWORK_H
#define NETWORK_H

#include <iostream>
#include <cstring>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

#include "player.h"
#include "poker_comm.h"

using namespace std;
class Game;
unordered_map<string, string> readConfig(const string& filename);
void setNonBlocking(int socket);
int makeServerSocket(uint16_t portNo);
void processRequest(int dataSocketFD, AllPlayers gameServer);
void serverMainLoop(int serverSocketFD, int timeout, const char *program, AllPlayers gameServer);
bool newClientJoined(const string& ipAddress, int port, int maxPlayers);
void sendMessageToServer(const string& ipAddress, int port, const char* message);

class Network {
public:
    void sendMessageToClient(const std::string& userID, const std::string& message);
};

#endif // NETWORK_H
