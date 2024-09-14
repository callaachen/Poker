/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* gui.h: Declarations for GUI containers, styles, and methods   */
/*****************************************************************/

#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include <cairo.h>
#include <pango/pangocairo.h>
#include <vector>
#include <string>
#include <functional>

#include "constants.h"
#include "game.h" /*** TO-DO: Set games.h reference after it's viable ***/
#include "player.h"

using namespace std;

/** Game module integration **/
// extern AllPlayers allPlayers;
// extern Game game;
// int Game::getCurrentPlayerIndex() const {
//     return current_playerIndex;
// }
// int Game::getCurrentBet() const {
//     // Implement logic to return the current bet
//     return currentBet;
// }

void drawCard(cairo_t* cr, const Card& card, int x, int y); // Defining drawCard
void drawChip(cairo_t* cr, const DealerRole& chip, int x, int y, int radius); // Defining drawChip

/** Class and structure declarations **/
class Game;
class GameBox;
class BoardBox;
class ConsoleBox;
class ControlBox;
class GameScreen;
class LogoBox;
class PlayerBox;
class ScoreBox;
class Button;

/** Class definitions **/

/* GameBox is the parent of all other *Box classes */
class GameBox {
public:
    /* Constructor */
    GameBox(int x, int y, int width, int height);

    /* Virtual draw method to be overridden by derived classes */
    virtual void draw(cairo_t* cr) = 0;

protected:
    int x;
    int y;
    int width;
    int height;

    void drawBackground(cairo_t* cr);
    void drawBorder(cairo_t* cr);
    void setDefaultTextColor(cairo_t* cr);
};

class Button {
public:
    /* Constructor */
    Button(int x, int y, int width, int height, const string& label, function<void()> onClick);

    /* Methods */
    void draw(cairo_t* cr);
    bool isClicked(int clickX, int clickY);
    void click();
    void setLabel(const string& label);
    void adjustTextPosition(int deltaX, int deltaY); // Adjust label position

private:
    int x;
    int y;
    int width;
    int height;
    string label;
    function<void()> onClick;
    int textX; // offset value from adjustTextPostion()
    int textY; // offset value from adjustTextPostion()
};

class BoardBox : public GameBox {
public:
    /* Constructor */
    BoardBox(BoardCards* boardCards, int x, int y, int width, int height);

    /* Methods */
    void draw(cairo_t* cr) override;

private:
    BoardCards* boardCards;
};

class ConsoleBox : public GameBox {
public:
    /* Constructor */
    ConsoleBox(int x, int y, int width, int height);

    /* Methods */
    void draw(cairo_t* cr) override;
    void addLogMessage(const string& message);

private:
    vector<string> logMessages;
};

class ControlBox : public GameBox {
public:
    /* Constructor */
    ControlBox(int x, int y, int width, int height, GtkWidget* fixedContainer, ScoreBox* scoreBox, Game* game);

    /* Setters */
    Button* increaseBetButton;
    Button* matchBetButton;
    Button* foldButton;
    Button* quitButton;

    /* Getters */
    Button* getIncreaseBetButton() const;
    Button* getMatchBetButton() const;
    Button* getFoldButton() const;
    Button* getQuitButton() const;

    /* Methods */
    void draw(cairo_t* cr) override;
    void updateButtonLabels();

    /* Player Bet Entry Value Setters and Getters */
    void setPlayerBetEntryValue(int bet); // Helper method
    int getPlayerBetEntryValue() const;   // Helper method
    void setPlayerBet(int bet); 
    int getPlayerBet() const;

private:
    int playerBet;
    GtkWidget* playerBetEntry;
    ScoreBox* scoreBox; // Reference to the ScoreBox to access currentBet
    Game* game;
    static void onPlayerBetChanged(GtkWidget* widget, gpointer data);
};

class LogoBox : public GameBox {
public:
    /* Constructor */
    LogoBox(int x, int y, int width, int height, const char* logoPath);

    /* Methods */
    void draw(cairo_t* cr) override;
    void setLogo(const char* filepath);

private:
    string gameTitle;
    cairo_surface_t* logoImage;
};

class PlayerBox : public GameBox {
public:
    /* Constructor */
    PlayerBox(const Player* player, int x, int y, int width, int height);

    /* Methods */
    void draw(cairo_t* cr) override;

private:
    const Player* player;
};

class ScoreBox : public GameBox {
public:
    /* Constructor */
    ScoreBox(int x, int y, int width, int height);

    /* Methods */
    void draw(cairo_t* cr) override;
    void setMainPot(int mainPotCounter);
    void setCurrentBet(int currentBetCounter);
    void setScoreBox(int mainPotCounter, int currentBetCounter);
    void setSideBet(int index, int value); // index = Player number; value is points

    int getCurrentBet() const;
    int getMainPot() const;
    int getSideBet(int index) const;

    void setControlBox(ControlBox* ctrlBox);

private:
    int mainPot;
    int currentBet;
    int sideBets[9]; // Array to store side bets
    ControlBox* controlBox;
};

class GameScreen {
public:
    /* Constructor */
    GameScreen(int width, int height, const char* title, Game* game);

    /* Setters */
    void addPlayerBox(PlayerBox* pBox); // Dynamically sets variable amount of PlayerBox instances
    void setConsoleBox(ConsoleBox* cBox);
    void setBoardBox(BoardBox* bBox);
    void setControlBox(ControlBox* ctrlBox);
    void setLogoBox(LogoBox* lgBox);
    void setScoreBox(ScoreBox* sBox);

    /* Method for accessing ConsoleBox log */
    BoardBox* getBoardBox() const;     // Access BoardBox for printing log messages 
    ConsoleBox* getConsoleBox() const; // For access to set ConsoleBox log
    ControlBox* getControlBox() const; // For access to get/set input widget in ControlBox
    ScoreBox* getScoreBox() const;     // For access to set pots and currentBet in ScoreBox

    /* Methods */
    void initialize();
    void show();
    void initializeBoxes(AllPlayers& allPlayers, BoardCards& boardCards);
    void initializeButtons();

    /* New methods for redrawing PlayerBoxes */
    void redrawPlayerBox(int playerIndex);
    void redrawAllPlayerBoxes();

    /* Testing methods */
    GtkWidget* getWindow() const { return window; }
    GtkWidget* getDrawingArea() const { return drawingArea; }

private:
    GtkWidget *window;
    GtkWidget *drawingArea;
    GtkWidget *fixedContainer;
    int width;
    int height;
    vector<PlayerBox*> playerBoxes; // Array of Players to render each PlayerBox

    ConsoleBox* consoleBox;
    BoardBox* boardBox;
    ControlBox* controlBox;
    LogoBox* logoBox;
    ScoreBox* scoreBox;

    Game* game;

    vector<Button*> buttons;

    /* GTK Events */
    static gboolean on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);
    static gboolean on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data);
};

#endif// GUI_H
