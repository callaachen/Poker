/* network.cpp */

#include "network.h"
#include "poker_comm.h"

using namespace std;

// Function to read configuration file
unordered_map<string, string> readConfig(const string& filename) {
    unordered_map<string, string> config;
    ifstream file(filename);

    if (!file.is_open()) {
        throw runtime_error("Could not open configuration file.");
    }

    string line;

    // Read the file line by line
    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        auto delimiterPos = line.find('=');
        if (delimiterPos == string::npos) {
            continue;
        }
        string key = line.substr(0, delimiterPos);
        string value = line.substr(delimiterPos + 1);
        config[key] = value;
    }

    file.close();
    return config;
}

// Set a socket to non-blocking mode
void setNonBlocking(int socket) {

    // Get the current socket flags
    int flags = fcntl(socket, F_GETFL, 0);

    // Set the socket to non-blocking mode
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}

// Function to create and bind a server socket
int makeServerSocket(uint16_t portNo) {
    int serverSocketFD;
    struct sockaddr_in serverSocketName;

    // Create the socket
    serverSocketFD = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocketFD < 0) {
        throw runtime_error("Service socket creation failed");
    }

    // Configure the socket address
    serverSocketName.sin_family = AF_INET;
    serverSocketName.sin_port = htons(portNo);
    serverSocketName.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind the socket to the address
    if (bind(serverSocketFD, (struct sockaddr*)&serverSocketName, sizeof(serverSocketName)) < 0) {
        close(serverSocketFD);
        throw runtime_error("Binding the server to a socket failed");
    }

    // Start listening on the socket
    if (listen(serverSocketFD, 8) < 0) { // max 8 clients in backlog
        close(serverSocketFD);
        throw runtime_error("Listening on socket failed");
    }

    return serverSocketFD;
}

// Function to process a request by a client
void processRequest(int dataSocketFD, AllPlayers gameServer) {
    char recvBuf[256]; // message buffer for receiving a message
    char sendBuf[512]; // message buffer for sending a response
    string recvLine;   // converting the message to C++ string
    string sendLine;   // converting the message to C++ string
    /* variables to store data from user message (e.g. Brian1234! BET 100)*/
    string name;            
    string userID;     
    string action;
    int amount;
    vector<Player> currPlayers;
    /* miscellaneous */
    int index;

    // Read the client's request
    int n = read(dataSocketFD, recvBuf, sizeof(recvBuf) - 1);       // received message should look like: "Brian1234! BET 100"
                                                                    // if new client joined, message is: "Brian1234! Brian 0"
    if (n < 0) {
        throw runtime_error("Reading from data socket failed");
    }
    recvBuf[n] = 0;             // Null-terminate the received string
    recvLine = string(recvBuf); // convert C string to C++ string
    stringstream ss(recvLine);  // variable to use for splitting and storing C++ string

    ss >> userID;                 // stores user id information from client message to a variable
    ss >> action;                 // stores the action from client into a variable
    ss >> amount;                 // stores the amount bet from the client, or nothing if it's a quit or fold command

    currPlayers = gameServer.getPlayers();
    index = -1;
    for(unsigned int i = 0; i < currPlayers.size(); i++) {
        if(currPlayers[i].getUserID() == userID) {            // sets the index to the correct user that sent the message to server
            index = i;  
            break;
        }
    }
    if(action == "BET/RAISE" || action == "CHECK/CALL") {       // all these actions can change the amount of points of a player
        currPlayers[index].setScore(currPlayers[index].getScore() - amount);
    } else if(action == "FOLD" || action == "QUIT") {           // all these actions lead to the player unable to continue playing the round
        currPlayers[index].setStatus(PlayerLost);
    } else {                                                    // when a new name appears
        if(currPlayers.size() == 0) {
            Player newPlayer(action, currPlayers.size()+1, 100, userID, true, Human, Dealer);
            gameServer.addPlayer(newPlayer);
        } else if(currPlayers.size() == 1) {
            Player newPlayer(action, currPlayers.size()+1, 100, userID, true, Human, LittleBlind);
            gameServer.addPlayer(newPlayer);          
        } else if(currPlayers.size() == 2) {
            Player newPlayer(action, currPlayers.size()+1, 100, userID, true, Human, BigBlind);
            gameServer.addPlayer(newPlayer);
        } else {
            Player newPlayer(action, currPlayers.size()+1, 100, userID, true, Human, NoRole);
            gameServer.addPlayer(newPlayer);
        }    
    }   

    sendLine = messageToClient(userID, action, amount);             // writing a send message (e.g. Brian1234! bet 100 points!)
    strncpy(sendBuf, sendLine.c_str(), sizeof(sendBuf)-1);          // converting C++ string to C string
    sendBuf[sizeof(sendBuf)-1] = 0;                         

    // Send the response back to the client
    int l = strlen(sendBuf);
    n = write(dataSocketFD, sendBuf, l);
    if (n < 0) {
        throw runtime_error("Writing to data socket failed");
    }
}

// Main server loop to handle client connections
void serverMainLoop(int serverSocketFD, int timeout, const char *program, AllPlayers gameServer) {
    int dataSocketFD;       // socket for new client
    socklen_t clientLen;
    struct sockaddr_in 
        clientAddress;      // address of the client to connect with
    fd_set 
        activeFDs,          // socket file descriptors to select from
        readFDs;            // socket file descriptors ready to be read from
    AllPlayers playerlist;  // list of all players that the server should keep track of
    int settedFDs, i;       // number of bits set by select() function, and iteration tracker i
    struct timeval timeVal; // timeout argument in the select() function
    int shutdown;           // signal to see if the server is told to shut down

    FD_ZERO(&activeFDs);
    FD_SET(serverSocketFD, &activeFDs);
    shutdown = 0;

    // Time tracking for sending player updates
    time_t lastUpdate = time(nullptr);

    while(!shutdown) {
        readFDs = activeFDs;    // since the server never actively sends out data to clients, all the FDs are for reading
        timeVal.tv_sec = timeout/1000000;   // seconds
        timeVal.tv_usec = timeout%1000000;  // microseconds
        settedFDs = select(FD_SETSIZE, &readFDs, NULL, NULL, &timeVal);
        /* behaviors depending on select() function output */
        if(settedFDs < 0) {
            throw runtime_error("Waiting for input or timeout (select) failed!\n");
        } else if (settedFDs == 0) {
            /* timeout occurred */
        } else {
            /* some FDs have data to be read */
            for(i = 0; i < FD_SETSIZE; i++) {
                if(FD_ISSET(i, &readFDs)) {
                    if(i == serverSocketFD) {
                        /* connection request on server socket */
#ifdef DEBUG
                        printf("%s: Accepting new client...\n", program);
#endif
                        clientLen = sizeof(clientAddress);
                        dataSocketFD = accept(serverSocketFD, (struct sockaddr *)&clientAddress, &clientLen);
                        if(dataSocketFD < 0) {
                            throw runtime_error("Data socket creation (accept) failed!\n");
                        }
#ifdef DEBUG
                        printf("%s: New client connected from %s:%hu.\n", program, inet_ntoa(clientAddress.sin_addr, ntohs(clientAddress.sin_port)));
#endif
                        FD_SET(dataSocketFD, &activeFDs);
                    } else {
                        /* active communication with a client */
#ifdef DEBUG 
                        printf("%s: Dealing with client on FD%d...\n", program, i);
#endif
                        processRequest(i, gameServer);
#ifdef DEBUG
                        printf("%s: Closing client connection FD%d.\n", program, i);
#endif
                        close(i);
                        FD_CLR(i, &activeFDs);
                    }
                }
            }
        }

        /* server sees connection request */

        /* server accepts connection */

        /* server adds the player to the end of the playerlist */
                        // Periodically send player updates
        time_t now = time(nullptr);
        if (difftime(now, lastUpdate) >= 1) {  // Adjust the interval as needed
            sendPlayerUpdates(dataSocketFD, gameServer);
            lastUpdate = now;
        }
    }
}

// Function to let client connect to a server and send a message
void sendMessageToServer(const string& ipAddress, int port, const char* message) {
    // Create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        throw runtime_error("Error creating socket");
    }

    // Configure server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        close(clientSocket);
        throw runtime_error("Error connecting to server");
    }

    // Send data to server
    if (send(clientSocket, message, strlen(message), 0) < 0) {
        close(clientSocket);
        throw runtime_error("Error sending data to server");
    }

    // Receive data from server
    char buffer[1024] = {0};
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        close(clientSocket);
        throw runtime_error("Error receiving data from server");
    }

    // Print received data
    cout << "Received from server: " << buffer << endl;

    // Close socket
    close(clientSocket);
}

// Implementation of sendMessageToClient method
void Network::sendMessageToClient(const std::string& userID, const std::string& message) {
    cout << "Sending message to " << userID << ": " << message << endl;
}

/*
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " port" << endl;
        return 1;
    }
    int portNo = atoi(argv[1]);
    if (portNo <= 2000) {
        cerr << "Invalid port number " << portNo << ", should be >2000" << endl;
        return 1;
    }

    try {
        int serverSocketFD = makeServerSocket(portNo);
        serverMainLoop(serverSocketFD, 250000); // 250000 microseconds = 0.25 seconds
        close(serverSocketFD);
    } catch (const runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
*/
