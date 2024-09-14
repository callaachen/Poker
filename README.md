# Poker by Lucky Sevens CO
Version 1.0 - Created May 9, 2024, revised 

# Overview
Welcome to Poker by Lucky Sevens CO! Our Poker simulator offers an immersive digital experience where players can engage in a game of Poker at their convenience. Our simulator provides players of all skill levels with an authentic and challenging experience. Whether you are a seasoned grandmaster looking for a competitive match or a beginner of Poker fundamentals, our simulator offers customizable features and tutorials to cater your needs. Dive into the world of Poker like never before, where strategy, tactics, and foresight can come together in an engaging virtual environment.  

######

## Table of Contents
1. [Introduction](#introduction)
2. [Features](#features)
3. [Installation](#installation)
4. [Running the Game](#running-the-game)
5. [Uninstallation](#uninstallation)
6. [Game Rules](#game-rules)
7. [Contribution](#contribution)
8. [License](#License)
9. [About Lucky Sevens](#about-lucky-sevens)

## Introduction
Our Texas Hold 'Em game supports 2 to 8 players, with 1 to 7 of those players being bots controlled by the application. Each human player interacts with the game from their own client version, which connects to a shared server. The in-game currency is points and the game adheres to all the standard Texas Hold 'Em rules and player actions. 

## Features
- **Multiplayer Support:** Play with 2 to 8 Players
- **Bot Integration:** 1 to 7 players can be bots controlled by the app
- **Client-Server Architecture:** Each human player uses a client that connects to a shared server.
- **Points-Based Currency:** In-game currency is points, not real money
- **Standard Rules:** Supports all standard Texas Hold 'Em Rules and Actions

## Installation
Instructions for Installation are located in [INSTALL.md](INSTALL.md)

## Prerequisites
- Ensure you have `gcc`, `make`, and other required development tools installed on your system
- Ensure you have GNU compiler g++ capable of compiling C++ 11.
- Ensure you have GTK+ 2.24.31 and Cairo 1.15.12 libraries installed.
- Ensure you have a stable internet connection. 

### Steps
1. **Download the Game Packages**
    - Download `Poker_V1.0.tar.gz` file from the release section 
2. **Unpack the Archive**
    ```sh
    tar -xyzf Poker_V1.0.tar.gz
    ```
    This will create the following folders:
    - `src`: All the C++ resources
    - `doc`: User Manuals
    - `bin`: Where the application will be compiled
    - `root`: Contains the Makefile
3. **Navigate to the Root Directory**
    ```sh
    make
    ```
    This will compile the game and place the executible in the `bin` directory

## Running the game
1. In PuTTy, log into a local server. ```laguna``` is the preferred server to launch the game.
2. Once you are already logged in, change the directory to ```src```.
3. In the terminal type ```make```.
4. After building the game, type ```./Poker [server] 10070```. You can use any of the hostnames EXCEPT for the server you are currently logged on to. 

## Uninstallation
To uninstall, simply type ```make delete``` and press enter to remove all lthe game components from your machine, including the installer file. 

## Game Rules
The game follows all standard Texas Hold 'Em rules, including:
- **Blindes and Antes:** Small and big blinds are posted before the deal
- **Dealing:** Each player is dealt two cards face down (hole cards).
- **Betting Rounds:** Four betting rounds (pre-flop, flop, turn, river).
- **Community Cards:** Five community cards are dealt face up.
- **Winning Hand:** The best five-card hand using any combination of hole cards and community cards wins the pot.

For detailed rules, please refer to the user manuals located in the `doc` directory.


## License
By using the Lucky Sevens Poker software ("the Software") provided by Lucky Sevens UCI, you agree to abide by this License Agreement and Disclaimer of Warranty. If you disagree with these terms, refrain from using the Software.

# About Lucky Sevens
Lucky Sevens is an innovative software engineering company proudly affiliated with the Electrical Engineering and Computer Science (EECS) department at the esteemed University of California, Irvine. Committed to pushing the boundaries of technological advancement, we specialize in developing cutting-edge software solutions that cater to diverse industries and societal needs. With a foundation deeply rooted in academic excellence and research-driven methodologies, our team of passionate engineers and developers strives to deliver transformative solutions that empower businesses, institutions, and individuals alike. At Lucky Sevens Co., we envision, innovate, and implement, driving forward the digital landscape with ingenuity and integrity.

Representatives of Lucky Sevens: Kirby Burke, Calla Chen, Susung Kang, Quang Phuc Le, Taha Zeeshan, and Christian Zeidan

######
