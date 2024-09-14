/******************************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                                         */
/* Authors: The Lucky Sevens (Team 7)                                         */
/* poker_comm.cpp: The code for the communications functions                  */
/******************************************************************************/

#include "poker_comm.h"

std::string serializePlayer(const Player& player) {
    std::stringstream ss;
    ss << player.getName() << " "
       << player.getNumber() << " "
       << player.getScore() << " "
       << player.getUserID() << " "
       << player.getIsPlaying() << " "
       << player.getPlayerType() << " "
       << player.getDealerRole() << " "
       << player.getStatus() << " "
       << player.getIsCurrentPlayer();

    // Serialize cards in hand
    const auto& hand = player.getHand();
    auto& cards = const_cast<Hand&>(hand).getCards();

    for (const auto& card : cards) {
        ss << " " << card.getRank() << " " << card.getSuit() << " " << card.getIsVisible();
    }

    return ss.str();
}

void sendPlayerUpdates(int clientSocketFD, const AllPlayers& allPlayers) {
    char sendBuf[2048]; // message buffer for sending a response

    for (const auto& player : allPlayers.getPlayers()) {
        std::string serializedPlayer = serializePlayer(player);
        strncpy(sendBuf, serializedPlayer.c_str(), sizeof(sendBuf) - 1);
        sendBuf[sizeof(sendBuf) - 1] = 0; // Null-terminate the string

        // Send the response back to the client
        int l = strlen(sendBuf);
        int n = write(clientSocketFD, sendBuf, l);
        if (n < 0) {
            throw std::runtime_error("Writing to client socket failed");
        }
    }
}


Player deserializePlayer(const std::string& serializedPlayer) {
    std::stringstream ss(serializedPlayer);
    std::string name, userID;
    int number, score, type, role, status;
    bool isPlaying, isCurrentPlayer;
    std::vector<Card> hand;

    ss >> name >> number >> score >> userID >> isPlaying >> type >> role >> status >> isCurrentPlayer;

    // Deserialize cards in hand
    while (!ss.eof()) {
        int rank, suit;
        bool faceDown;
        ss >> rank >> suit >> faceDown;
        hand.emplace_back(static_cast<Rank>(rank), static_cast<Suit>(suit), faceDown);
    }

    Player player(name, number, score, userID, isPlaying, static_cast<PlayerType>(type), static_cast<DealerRole>(role), static_cast<PlayerStatus>(status));
    for (const auto& card : hand) {
        player.addCardToHand(card);
    }
    player.setIsCurrentPlayer(isCurrentPlayer);

    return player;
}

void receivePlayerUpdates(int serverSocketFD, AllPlayers& allPlayers) {
    char buffer[2048] = {0};
    int bytesReceived = recv(serverSocketFD, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        perror("Error receiving data from server");
        return;
    } else if (bytesReceived == 0) {
        std::cout << "Server disconnected\n";
        return;
    }

    std::stringstream ss(std::string(buffer, bytesReceived));
    std::string serializedPlayer;
    
    // Clear the current players
    allPlayers.getPlayers().clear();

    // Process each serialized player
    while (std::getline(ss, serializedPlayer)) {
        Player player = deserializePlayer(serializedPlayer);
        allPlayers.addPlayer(player);
    }

    //gameScreen.updatePlayers(allPlayers);
}



/* Client class functions */
string bet_raiseToServer(string userID, int amount) {
    string message;
    
    return message;
}

string foldToServer(string userID, int amount) {
    string message;

    return message;
}

string check_callToServer(string userID, int amount) {
    string message;

    return message;
}

string quitToServer(string userID, int amount) {
    string message;

    return message;    
}

/* Server class functions */
void updateGameStatus() {

}

void sendToPlayerNo(int index) {
    
}

std::string messageToClient(std::string userID, std::string action, int amount) {
    string message;
    message = userID + action + to_string(amount) + " points!";
    return message;
}

// Check info coming to the client from the server
void infoFromServer(int serverSocket) {
    char buffer[1024] = {0};
    int bytesReceived = recv(serverSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        perror("Error receiving data from server");
        return;
    } else if (bytesReceived == 0) {
        printf("Server disconnected\n");
        return;
    }

    // Assuming the server sends responses in a specific format
    char* token = strtok(buffer, " ");
    while (token != NULL) {
        // Check each token to identify the response type
        if (strcmp(token, "GAME_START") == 0) {
            printf("Received GAME_START response from server\n");
            // Handle GAME_START response
        } else if (strcmp(token, "YOUR_TURN") == 0) {
            printf("Received YOUR_TURN response from server\n");
            // Handle YOUR_TURN response
        } else if (strcmp(token, "WIN") == 0) {
            printf("Received WIN response from server\n");
            // Handle WIN response
        } else {
            printf("Unknown response received from server: %s\n", token);
        }
        token = strtok(NULL, " ");
    }
}


// Check information coming from clients to the server
void infoFromCLient(int clientSocket) {
    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        perror("Error receiving data from client");
        return;
    } else if (bytesReceived == 0) {
        printf("Client disconnected\n");
        return;
    }

    // Assuming the client sends requests in a specific format
    // For example, request types are separated by space
    char* token = strtok(buffer, " ");
    while (token != NULL) {
        // Check each token to identify the request type
        if (strcmp(token, "JOIN_GAME") == 0) {
            printf("Received JOIN_GAME request from client\n");
            // Handle JOIN_GAME request
        } else if (strcmp(token, "BET") == 0) {
            printf("Received BET request from client\n");
            // Handle BET request
        } else if (strcmp(token, "FOLD") == 0) {
            printf("Received FOLD request from client\n");
            // Handle FOLD request
        } else {
            printf("Unknown request received from client: %s\n", token);
        }
        token = strtok(NULL, " ");
    }
}