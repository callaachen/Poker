#include "game.h"

using namespace std;

Game::Game(vector<Player *> players, Network &network)
    : players(players), dealerIndex(0), smallBlindIndex(1), bigBlindIndex(2), network(network)
{
    player_bet = new int[players.size()];
    player_money = new int[players.size()];

    initializeGame();
}

void Game::start()
{
    round = 1;
    hand = 1;
    setInitialDealerAndBlinds();
    setBlindBets();
    dealCards();
    broadcastGameState();

    // Start the game logic
    current_playerIndex = (bigBlindIndex + 1) % players.size();
    startingIndex = current_playerIndex;

    // PlayerChoice(1);
}

void Game::end()
{
    // End the game logic
}

void Game::PlayerChoice(int choice, int raise_amount)
{
    if (round == 1)
    {
        message = players[current_playerIndex]->getUserID() + " Turn. "; // Pass To Network
        Round1(choice, raise_amount);
    }
    else if (round == 2)
    {
        message = players[current_playerIndex]->getUserID() + " Turn. "; // Pass To Network
        Round2(choice, raise_amount);
    }
    else if (round == 3)
    {
        message = players[current_playerIndex]->getUserID() + " Turn. "; // Pass To Network
        Round3(choice, raise_amount);
    }
    else if (round == 4)
    {
        message = players[current_playerIndex]->getUserID() + " Turn. "; // Pass To Network
        Round4(choice, raise_amount);
    }
    else if (round == 5)
    {
        ShowDown();
    }
}

/*
    Choice 1: Match Previous Bet / Check
    Choice 2: Raise Bet
    Choice 3: Fold Cards
*/

void Game::Round1(int choice, int raise_amount)
{
    if (choice == 1)
    {
        int prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        while (players[prev_playerIndex]->getIsPlaying() == false)
            prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        int prev_bet = player_bet[prev_playerIndex];

        message = players[current_playerIndex]->getUserID() + " Calls. "; // Pass To Network
        placeBet(players[current_playerIndex], current_playerIndex, prev_bet);
    }
    else if (choice == 2)
    {
        message = players[current_playerIndex]->getUserID() + " Raises bet to $" + to_string(raise_amount); // Pass To Network
        placeBet(players[current_playerIndex], current_playerIndex, raise_amount);
    }
    else if (choice == 3)
    {
        message = players[current_playerIndex]->getUserID() + " Folds. "; // Pass To Network
        fold(players[current_playerIndex], current_playerIndex);
    }
}

void Game::Round2(int choice, int raise_amount)
{
    if (BoardCardIndex == 0) // Initialize Variables and Options that Suit the Round
    {
        for (int i = 0; i < 3; i++) // Display First 3 Community Cards AKA FLOP
        {
            CommunityCards.setCard(BoardCardIndex, deck.getRandomCard());
            BoardCardIndex++;
        }

        current_playerIndex = (dealerIndex + 1) % players.size();

        while (players[current_playerIndex]->getIsPlaying() == false)
        {
            current_playerIndex = (current_playerIndex + 1) % players.size();
        }

        startingIndex = current_playerIndex;
    }

    if (choice == 1)
    {
        int prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        while (players[prev_playerIndex]->getIsPlaying() == false)
            prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        int prev_bet = player_bet[prev_playerIndex];

        if (prev_bet == 0) // Check
        {
            startingIndex = current_playerIndex;
            message = players[current_playerIndex]->getUserID() + " Checks. "; // Pass To Network
            updateGameState();
        }
        else
        {
            message = players[current_playerIndex]->getUserID() + " Calls. "; // Pass To Network
            placeBet(players[current_playerIndex], current_playerIndex, prev_bet);
        }
    }
    else if (choice == 2)
    {
        message = players[current_playerIndex]->getUserID() + " Raises bet to $" + to_string(raise_amount); // Pass To Network
        placeBet(players[current_playerIndex], current_playerIndex, raise_amount);
    }
    else if (choice == 3)
    {
        message = players[current_playerIndex]->getUserID() + " Folds. "; // Pass To Network
        fold(players[current_playerIndex], current_playerIndex);
    }
}

void Game::Round3(int choice, int raise_amount)
{
    if (BoardCardIndex == 3) // Initialize Variables and Options that Suit the Round
    {
        // Display Another Community Card AKA THE TURN
        CommunityCards.setCard(BoardCardIndex, deck.getRandomCard());
        BoardCardIndex++;

        current_playerIndex = (dealerIndex + 1) % players.size();

        while (players[current_playerIndex]->getIsPlaying() == false)
        {
            current_playerIndex = (current_playerIndex + 1) % players.size();
        }

        startingIndex = current_playerIndex;
    }

    if (choice == 1)
    {
        int prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        while (players[prev_playerIndex]->getIsPlaying() == false)
            prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        int prev_bet = player_bet[prev_playerIndex];

        if (prev_bet == 0) // Check
        {
            startingIndex = current_playerIndex;
            message = players[current_playerIndex]->getUserID() + " Checks. "; // Pass To Network
            updateGameState();
        }
        else
        {
            message = players[current_playerIndex]->getUserID() + " Calls. "; // Pass To Network
            placeBet(players[current_playerIndex], current_playerIndex, prev_bet);
        }
    }
    else if (choice == 2)
    {
        message = players[current_playerIndex]->getUserID() + " Raises bet to $" + to_string(raise_amount); // Pass To Network
        placeBet(players[current_playerIndex], current_playerIndex, raise_amount);
    }
    else if (choice == 3)
    {
        message = players[current_playerIndex]->getUserID() + " Folds. "; // Pass To Network
        fold(players[current_playerIndex], current_playerIndex);
    }
}

void Game::Round4(int choice, int raise_amount)
{
    if (BoardCardIndex == 4) // Initialize Variables and Options that Suit the Round
    {
        // Display Another Community Card AKA THE RIVER
        CommunityCards.setCard(BoardCardIndex, deck.getRandomCard());
        BoardCardIndex++;

        current_playerIndex = (dealerIndex + 1) % players.size();

        while (players[current_playerIndex]->getIsPlaying() == false)
        {
            current_playerIndex = (current_playerIndex + 1) % players.size();
        }

        startingIndex = current_playerIndex;
    }

    if (choice == 1)
    {
        int prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        while (players[prev_playerIndex]->getIsPlaying() == false)
            prev_playerIndex = (current_playerIndex - 1 + players.size()) % players.size();

        int prev_bet = player_bet[prev_playerIndex];

        if (prev_bet == 0) // Check
        {
            startingIndex = current_playerIndex;
            message = players[current_playerIndex]->getUserID() + " Checks. "; // Pass To Network
            updateGameState();
        }
        else
        {
            message = players[current_playerIndex]->getUserID() + " Calls. "; // Pass To Network
            placeBet(players[current_playerIndex], current_playerIndex, prev_bet);
        }
    }
    else if (choice == 2)
    {
        message = players[current_playerIndex]->getUserID() + " Raises bet to $" + to_string(raise_amount); // Pass To Network
        placeBet(players[current_playerIndex], current_playerIndex, raise_amount);
    }
    else if (choice == 3)
    {
        message = players[current_playerIndex]->getUserID() + " Folds. "; // Pass To Network
        fold(players[current_playerIndex], current_playerIndex);
    }
}

void Game::ShowDown()
{
    Player *winner = NULL;
    int winner_id = 0;

    evaluateBestHand();

    for (size_t i = 0; i < players.size(); i++)
    {
        if (players[i]->getStatus() == PlayerWon)
        {
            winner = players[i];
            winner_id = i;
        }
    }

    distributeWinnings(winner, winner_id);

    nextHand();
}

void Game::distributeWinnings(Player *winner, int winner_id)
{
    message = winner->getUserID() + " Won $" + to_string(pot); // Pass To Network
    player_money[winner_id] += pot;
    pot = 0;
}

void Game::setBlindBets()
{
    int small_blind = 5;
    int bigBlind = 10;

    player_bet[smallBlindIndex] = small_blind;
    message = players[smallBlindIndex]->getUserID() + " has set a small blind of " + to_string(small_blind); // Pass To Network

    player_bet[bigBlindIndex] = bigBlind;
    message = players[bigBlindIndex]->getUserID() + " has set a big blind of " + to_string(bigBlind); // Pass To Network
}

void Game::placeBet(Player *player, int player_id, int amount)
{
    if (player_money[player_id] + player_bet[player_id] - amount >= 0)
    {
        player_money[player_id] += player_bet[player_id]; // If Bet Already Placed
        player_bet[player_id] = amount;
        player_money[player_id] -= amount;

        message = player->getUserID() + " placed a bet of $ " + to_string(amount); // Pass To Network
    }
    else
    {
        message = player->getUserID() + " has not enough Money to bet $ " + to_string(amount); // Pass To Network
    }

    updateGameState();
}

void Game::fold(Player *player, int player_id)
{
    // Handle fold logic
    players[player_id]->setIsPlaying(false);
    players[player_id]->setStatus(PlayerLost);

    updateGameState();
}

void Game::updateGameState()
{
    // Update the game state after a player's action
    nextTurn();
    broadcastGameState();
    // PlayerChoice(1);
}

void Game::nextTurn()
{
    if (!EnoughPlayersAvailable())
    {
        end();
        return;
    }
    
    int prev_Index = current_playerIndex;
    current_playerIndex = (current_playerIndex + 1) % players.size();

    while (players[current_playerIndex]->getIsPlaying() == false)
    {
        current_playerIndex = (current_playerIndex + 1) % players.size();
    }

    if (current_playerIndex == prev_Index)
    {
        cout << "Every Other Player Folded!";
        round++;

        for (size_t i = 0; i < players.size(); i++) // Use size_t
        {
            pot += player_bet[i];
            player_bet[i] = 0;

            if (round < 5 && player_money[i] > 0)
            {
                players[i]->setIsPlaying(true);
                players[i]->setStatus(Playing);
            }
        }
    }
    else if (current_playerIndex == startingIndex)
    {
        cout << "Round Over!" << endl;
        round++;

        for (size_t i = 0; i < players.size(); i++) // Use size_t
        {
            pot += player_bet[i];
            player_bet[i] = 0;

            if (round < 5)
            {
                players[i]->setIsPlaying(true);
                players[i]->setStatus(Playing);
            }
        }
    }
}

/*
    In Texas Hold Em Poker, this is how the game works.

    You can think of Hand as a stage. When the game starts, its stage 1. On stage 1, dealer is 0, small blind is 1 and big blind is 2.
    Each Stage has 5 Rounds
    Round 1 to Round 4 are the Initial, the Flop, The Turn, and the River respectively.
    Round 5 is the ShowDown that checks who won and then calls NextHand

    NextHand now changes the hand i.e the dealer is now 1, small blind is 2, big blind is 3. This is to ensure that every player gets the chance to deal and there is fairness in the game.
    Then the game starts from over again.

    The Next Hand also checks if everyone has dealt cards, then end the game.
*/

bool Game::EnoughPlayersAvailable()         // Returns True if Enough Players are there to continue the game.
{
    int players_playing = 0;

    for (size_t  i = 0; i < players.size(); i++)
    {
        if (players[i]->getIsPlaying())
            players_playing++;
    }

    if (players_playing < 2)
        return false;

    else
        return true;
}

void Game::nextHand()
{
    message = "Next Hand Starting!"; // Pass to Network
    round = 1;
    hand++;
    BoardCardIndex = 0;

    if (!EnoughPlayersAvailable())
    {
        end();
        return;
    }

    rotateBlindsAndDealer();
    setBlindBets();
    dealCards();
    broadcastGameState();
    // Logic for starting the next round
    current_playerIndex = (bigBlindIndex + 1) % players.size();
    startingIndex = current_playerIndex;

    // PlayerChoice(1);
}

void Game::initializeGame()
{
    pot = 0;
    round = 0;
    current_playerIndex = 0;
    startingIndex = 0;
    hand = 0;
    BoardCardIndex = 0;

    for (size_t  i = 0; i < players.size(); i++)
    {
        player_bet[i] = 0;
        player_money[i] = players[i]->getScore(); // Initialize Starting Money of Players
    }
}

void Game::setInitialDealerAndBlinds()
{
    if (players.size() < 2)
    {
        // Not enough players to start the game
        return;
    }

    players[dealerIndex]->setDealerRole(Dealer);
    players[smallBlindIndex]->setDealerRole(LittleBlind);
    players[bigBlindIndex]->setDealerRole(BigBlind);
}

void Game::rotateBlindsAndDealer()
{
    players[dealerIndex]->setDealerRole(NoRole);
    players[smallBlindIndex]->setDealerRole(NoRole);
    players[bigBlindIndex]->setDealerRole(NoRole);

    dealerIndex = (dealerIndex + 1) % players.size();
    if (players.size() == 2)
    {
        smallBlindIndex = dealerIndex;
        bigBlindIndex = (dealerIndex + 1) % players.size();
    }
    else
    {
        smallBlindIndex = (dealerIndex + 1) % players.size();
        bigBlindIndex = (dealerIndex + 2) % players.size();
    }

    if (players.size() == 2)
    {
        players[dealerIndex]->setDealerRole(Dealer2P);
        players[bigBlindIndex]->setDealerRole(BigBlind);
    }
    else
    {
        players[dealerIndex]->setDealerRole(Dealer);
        players[smallBlindIndex]->setDealerRole(LittleBlind);
        players[bigBlindIndex]->setDealerRole(BigBlind);
    }
}

void Game::dealCards()
{
    int numPlayers = players.size();

    if (numPlayers == 0)
        return; // No players to deal to

    int currentPlayerIndex = (dealerIndex + 1) % numPlayers;

    for (int i = 0; i < numPlayers; i++)
    {
        players[currentPlayerIndex]->addCardToHand(deck.getRandomCard());
        players[currentPlayerIndex]->addCardToHand(deck.getRandomCard());

        currentPlayerIndex = (currentPlayerIndex + 1) % numPlayers;
    }

    message = players[dealerIndex]->getUserID() + " deals cards. "; // Pass To Network

    currentPlayerIndex = (dealerIndex + 1) % numPlayers;
}

void Game::broadcastGameState()
{
    for (size_t i = 0; i < players.size(); i++) // Use size_t
    {
        network.sendMessageToClient(players[i]->getUserID(), "GameStateUpdate"); // Placeholder message
    }
}

void Game::evaluateBestHand()
{
    AllPlayers allplayers;

    for(size_t i = 0; i<players.size(); i++)
    {
        allplayers.addPlayer(*players[i]);
    }
    handEval.compareHandTypes(allplayers);
}

// Getter methods
int Game::getPlayerBet(int index) const
{
    return player_bet[index];
}

int Game::getPlayerMoney(int index) const
{
    return player_money[index];
}

int Game::getRound() const
{
    return round;
}

int Game::getHand() const
{
    return hand;
}

int Game::getPot() const
{
    return pot;
}

int Game::getCurrentPlayerIndex() const
{
    return current_playerIndex;
}

const BoardCards& Game::getCommunityCards() const
{
    return CommunityCards;
}

// GameState class implementation
Game::GameState::GameState(Game& game) : game(game) {}

void Game::GameState::initialize()
{
    game.initializeGame();
}

void Game::GameState::updatePlayerBet(int index, int bet)
{
    game.player_bet[index] = bet;
}

void Game::GameState::updatePlayerMoney(int index, int money)
{
    game.player_money[index] = money;
}

void Game::GameState::updateRound(int round)
{
    game.round = round;
}

void Game::GameState::updatePot(int pot)
{
    game.pot = pot;
}

void Game::GameState::updateHand(int hand)
{
    game.hand = hand;
}

void Game::GameState::updateCurrentPlayerIndex(int index)
{
    game.current_playerIndex = index;
}

int Game::GameState::getPlayerBet(int index) const
{
    return game.getPlayerBet(index);
}

int Game::GameState::getPlayerMoney(int index) const
{
    return game.getPlayerMoney(index);
}

int Game::GameState::getRound() const
{
    return game.getRound();
}

int Game::GameState::getPot() const
{
    return game.getPot();
}

int Game::GameState::getHand() const
{
    return game.getHand();
}

int Game::GameState::getCurrentPlayerIndex() const
{
    return game.getCurrentPlayerIndex();
}
