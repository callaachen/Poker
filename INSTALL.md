# Instructions for Installation
ONLY RUN ON LAGUNA

## System Requirements
Storage:
- Minimum: 500kB
- Recommended: 1mB

Memory:
- Minimum: 200kB
- Recommendedd: 500kB

Supported OS:
- Rocky Linux 8.9 (Green Obsidian)
- CentOS Linus 7 (Core)

Software Requirements:
- GTK 2.2 graphics interface library
- (For Windows machines signing into ssh into Linux) Any version of Xming, preferably the newest version

## Setup and Configuration
This poker game is made specifically for the aforementioned OS, and possibly other Linux distributions, if you try to run the following processes on a Windows machine, it isn’t guaranteed to work.
1. (For Windows users ssh-ing into Linux): Launch Xming in the background, and enable X11 forwarding
    - By using the command ssh -x in Powershell
    - Or going to connection->ssh->x11->check the box that says “enable x11 forwarding” if using PuTTy
2. Unzip the Poker_V1.0.tar.gz with the command: gtar xzvf Poker_V1.0.tar.gz
3. When the Poker_Beta.tar.gz tar file is successfully extracted, switch to the directory that you just extracted with the command: ```cd Poker_V1.0.tar.gz```.
4. Type ```make all``` to build the executables
5. Type ```cd bin``` to go to the directory with executables
6. Run the command ```./PokerServer 10070``` to start the server
7. On a different server(bondi or laguna), redo Steps 1-5 and type ```./Poker "Server Name" 10070``` to launch our game.


## Compiling and running the game
1. Compile the game with this command: make all
2. If there are errors showing up, contact our team for additional help.
3. If there are no error messages, run the command ```./Poker_V1.0``` to launch the poker game application.