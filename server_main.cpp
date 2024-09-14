/**************************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                                     */
/* Authors: The Lucky Sevens (Team 7)                                     */
/* server_main.cpp: Server side Main Program File for Poker Game Software */
/**************************************************************************/
#include <iostream>
#include <sstream>
#include <string>

//#include "ai.h"
#include "cards.h"
#include "player.h"
#include "poker_comm.h"

using namespace std;

AllPlayers playerlist;

const char *program; /* program name for FatalError */

void fatalError(const char *errorMsg) {
    fputs(program, stderr);
    fputs(": ", stderr);
    perror(errorMsg);
    fputs(program, stderr);
    fputs(": Exiting!\n", stderr);
    exit(20);
} /* end of FatalError */

int main(int argc, char *argv[])
{
    int servSocketFD;       /* socket file descriptor for service */
    int portNo;		        /* port number */
    AllPlayers gameServer;  /* contains information about the current game for all players*/  

    program = argv[0]; /* publish program name (for diagnostics) */
#ifdef DEBUG
    printf("%s: Starting...\n", program);
#endif
    if (argc < 2)
    {   fprintf(stderr, "Usage: %s port\n", program);
	    exit(10);
    }
    portNo = atoi(argv[1]);	/* get the port number */
    if (portNo <= 2000)
    {   fprintf(stderr, "%s: invalid port number %d, should be >2000\n",
	    program, portNo);
        exit(10);
    }
#ifdef DEBUG
    printf("%s: Creating the server socket...\n", program);
#endif
    servSocketFD = makeServerSocket(portNo);
    serverMainLoop(servSocketFD, 250000, program, gameServer);
    printf("\n%s: Shutting down.\n", program);
    close(servSocketFD);
    return 0;
}
