/*****************************************************************/
/* EECS 22L Project 2: Texas Hold 'Em                            */
/* Authors: The Lucky Sevens (Team 7)                            */
/* gui.cpp: Definitions for GUI containers, styles, and methods  */
/*****************************************************************/

#include "gui.h"

using namespace std;

/*** Constructors ***/

GameBox::GameBox(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height) {}

/** Constructors derived from GameBox **/

BoardBox::BoardBox(BoardCards* boardCards, int x, int y, int width, int height)
    : GameBox(x, y, width, height), boardCards(boardCards) {}

ConsoleBox::ConsoleBox(int x, int y, int width, int height)
    : GameBox(x, y, width, height) {}

ControlBox::ControlBox(int x, int y, int width, int height, GtkWidget* fixedContainer, ScoreBox* scoreBox, Game* game)
    : GameBox(x, y, width, height), playerBet(0), scoreBox(scoreBox), game(game) {
    increaseBetButton = new Button(x + 0.5 * GRID_SQ, y + 4.5 * GRID_SQ, 3 * GRID_SQ, 1 * GRID_SQ, "Bet", [this]() {
        /* Increase Bet method placeholder */
        int raise_amount = this->getPlayerBet();
        this->game->PlayerChoice(2,raise_amount);
        printf("Bet/Raise button clicked!\n");
    });

    matchBetButton = new Button(x + 4.5 * GRID_SQ, y + 0.5 * GRID_SQ, 3 * GRID_SQ, 1 * GRID_SQ, "Check", [this]() {
        /* Match Bet method placeholder */
          int raise_amount = this->getPlayerBet();
          this->game->PlayerChoice(1,raise_amount);
        printf("Call/Check button clicked!\n");
    });

    foldButton = new Button(x + 4.5 * GRID_SQ, y + 2.5 * GRID_SQ, 3 * GRID_SQ, 1 * GRID_SQ, "Fold", [this]() {
        /* Fold method placeholder */
        int raise_amount = this->getPlayerBet();
         this->game->PlayerChoice(3,raise_amount);
        printf("Fold button clicked!\n");
    });

    quitButton = new Button(x + 4.5 * GRID_SQ, y + 4.5 * GRID_SQ, 3 * GRID_SQ, 1 * GRID_SQ, "Quit", []() {
        gtk_main_quit(); // Quit game
    });

    /* Manually shift label positions to center them on Buttons */
    increaseBetButton->adjustTextPosition(18, 0); // Adjust for "Bet"
    matchBetButton->adjustTextPosition(12, 0); // Adjust for "Check"
    foldButton->adjustTextPosition(16, 0); // Adjust for "Fold"
    quitButton->adjustTextPosition(17, 0); // Adjust for "Quit"

    /* Widget that accepts player bet amount */
    playerBetEntry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(playerBetEntry), "0");
    gtk_widget_set_size_request(playerBetEntry, 3 * GRID_SQ, 2 * GRID_SQ); // Set size
    gtk_fixed_put(GTK_FIXED(fixedContainer), playerBetEntry, x + 0.5 * GRID_SQ, y + 1.5 * GRID_SQ);
    g_signal_connect(playerBetEntry, "changed", G_CALLBACK(onPlayerBetChanged), this);

    updateButtonLabels(); // Initial label update
}


Button* ControlBox::getIncreaseBetButton() const { return increaseBetButton; }
Button* ControlBox::getMatchBetButton() const { return matchBetButton; }
Button* ControlBox::getFoldButton() const { return foldButton; }
Button* ControlBox::getQuitButton() const { return quitButton; }

LogoBox::LogoBox(int x, int y, int width, int height, const char* logoPath)
    : GameBox(x, y, width, height), gameTitle("Texas Hold 'Em"), logoImage(nullptr) {
    setLogo(logoPath);
}

PlayerBox::PlayerBox(const Player* player, int x, int y, int width, int height)
    : GameBox(x, y, width, height), player(player) {}

ScoreBox::ScoreBox(int x, int y, int width, int height)
    : GameBox(x, y, width, height), mainPot(0), currentBet(0), controlBox(nullptr) {
    fill(begin(sideBets), end(sideBets), 0); // Initialize all sideBets to 0
}

Button::Button(int x, int y, int width, int height, const string& label, function<void()> onClick)
    : x(x), y(y), width(width), height(height), label(label), onClick(onClick), textX(x + 10), textY(y + (height / 2) + 4) {}

GameScreen::GameScreen(int width, int height, const char* title, Game* game)
    : window(nullptr), drawingArea(nullptr), fixedContainer(nullptr), width(width), height(height),
      consoleBox(nullptr), boardBox(nullptr), controlBox(nullptr), logoBox(nullptr), scoreBox(nullptr) {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), title);
    gtk_window_set_default_size(GTK_WINDOW(window), width, height);

    drawingArea = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawingArea, width, height);

    fixedContainer = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixedContainer);
    gtk_fixed_put(GTK_FIXED(fixedContainer), drawingArea, 0, 0);

    /* Connect the expose-event signal */
    g_signal_connect(G_OBJECT(drawingArea), "expose-event", G_CALLBACK(on_expose_event), this);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    /* Connect the button press event */
    gtk_widget_add_events(drawingArea, GDK_BUTTON_PRESS_MASK);
    g_signal_connect(G_OBJECT(drawingArea), "button-press-event", G_CALLBACK(on_button_press_event), this);
}

/*** Methods ***/

/** Button methods **/

void Button::draw(cairo_t* cr) {
    /* Background */
    cairo_set_source_rgb(cr, 0.9, 0.9, 0.9); // Light grey fill
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);

    /* Border */
    cairo_set_source_rgb(cr, 0.3, 0.3, 0.3); // Dark grey stroke
    cairo_set_line_width(cr, 2);
    cairo_rectangle(cr, x, y, width, height);
    cairo_stroke(cr);

    /* Button text */
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Black text
    cairo_move_to(cr, textX, textY);
    cairo_show_text(cr, label.c_str());
}

/* Adjust Button label position */
void Button::adjustTextPosition(int deltaX, int deltaY) {
    textX += deltaX;
    textY += deltaY;
}

void Button::click() { if (onClick) { onClick(); } } 

bool Button::isClicked(int clickX, int clickY) { return clickX >= x && clickX <= x + width && clickY >= y && clickY <= y + height; }

void Button::setLabel(const string& newLabel) { label = newLabel; }

/** GameBox methods **/

void GameBox::drawBackground(cairo_t* cr) {
    cairo_set_source_rgb(cr, 0.0, 0.5, 0.0); // 50% green background
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);
}

void GameBox::drawBorder(cairo_t* cr) {
    cairo_set_source_rgb(cr, 0.0, 0.4, 0.0); // 40% green border
    cairo_set_line_width(cr, 1);
    cairo_rectangle(cr, x, y, width, height);
    cairo_stroke(cr);
}

void GameBox::setDefaultTextColor(cairo_t* cr) { cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); }// White text

/** BoardBox methods **/

void BoardBox::draw(cairo_t* cr) {
    /* Colors inherited from GameBox */
    drawBackground(cr);
    drawBorder(cr);
    setDefaultTextColor(cr);

    /* Board Cards */
    for (int i = 0; i < BOARD_CT; ++i) {
        const Card& card = boardCards->getCard(i);
        drawCard(cr, card, x + GRID_SQ + 5 + (i * 3 * GRID_SQ), y + (GRID_SQ * 2)); // + 5 pixel offset on x axis
    }
}

/** ConsoleBox methods **/

void ConsoleBox::draw(cairo_t* cr) {
    /* Background */
    cairo_set_source_rgb(cr, 0.0, 0.4, 0.0); // Darker 40% green
    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);

    /* Border */
    cairo_set_source_rgb(cr, 0.0, 0.3, 0.0); // Darker 30% green
    cairo_set_line_width(cr, 1);
    cairo_rectangle(cr, x, y, width, height);
    cairo_stroke(cr);

    /* ConsoleBox text */
    setDefaultTextColor(cr); // Text color from GameBox
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);  // Adjust the font size as needed
    cairo_move_to(cr, x + 10, y + GRID_SQ);
    cairo_show_text(cr, "Console");
    // Reset to default text size
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10);  // Adjust to your default font size

    /* Log messages */
    for (size_t i = 0; i < logMessages.size(); ++i) {
        cairo_move_to(cr, x + 10, y + (i + 2) * GRID_SQ); // Each message 1 GRID_SQ below the previous
        cairo_show_text(cr, logMessages[i].c_str());
    }
}

void ConsoleBox::addLogMessage(const string& message) {
    if (logMessages.size() >= 10) {
        logMessages.erase(logMessages.begin()); // Remove oldest message
    }
    logMessages.push_back(message); // Add new message
}

/** ControlBox methods **/

void ControlBox::draw(cairo_t* cr) {
    /* Colors inherited from GameBox */
    drawBackground(cr);
    drawBorder(cr);

    /* Draw Player Bet text */
    setDefaultTextColor(cr);
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 12);  // Adjust the font size as needed
    cairo_move_to(cr, x + 14, y + 28);
    cairo_show_text(cr, "Player Bet");
    // Reset the font size to default
    cairo_set_font_size(cr, 10);  // Adjust to your default font size

    /* Draw Buttons */
    increaseBetButton->draw(cr);
    matchBetButton->draw(cr);
    foldButton->draw(cr);
    quitButton->draw(cr);
}

int ControlBox::getPlayerBet() const { return playerBet; } // Access playerBet value from another module

int ControlBox::getPlayerBetEntryValue() const { return playerBet; }

void ControlBox::onPlayerBetChanged(GtkWidget* widget, gpointer data) {
    ControlBox* self = static_cast<ControlBox*>(data);
    const char* text = gtk_entry_get_text(GTK_ENTRY(widget));
    if (isdigit(text[0]) || (text[0] == '-' && isdigit(text[1]))) {
        self->playerBet = stoi(text);
    }
}

void ControlBox::setPlayerBet(int bet) { // Access playerBet value from another module
    playerBet += bet;
    gtk_entry_set_text(GTK_ENTRY(playerBetEntry), to_string(playerBet).c_str());
}

void ControlBox::setPlayerBetEntryValue(int bet) { 
    playerBet = bet;
    gtk_entry_set_text(GTK_ENTRY(playerBetEntry), to_string(bet).c_str());
}

void ControlBox::updateButtonLabels() {
    if (scoreBox->getCurrentBet() == 0) {
        increaseBetButton->setLabel("Bet");
        matchBetButton->setLabel("Check");
        increaseBetButton->adjustTextPosition(0, 0); // Reset for "Bet"
        matchBetButton->adjustTextPosition(0, 0);    // Reset for "Check"
    } else {
        increaseBetButton->setLabel("Raise");
        matchBetButton->setLabel("Call");
        increaseBetButton->adjustTextPosition(-5, 0); // Adjust for "Raise"
        matchBetButton->adjustTextPosition(6, 0);    // Adjust for "Call"
    }
}

/** LogoBox methods **/

void LogoBox::draw(cairo_t* cr) {
     /* Colors inherited from GameBox */
    drawBackground(cr);
    drawBorder(cr);
    setDefaultTextColor(cr);

    /* Calculate logo image scaling */
    double logoWidth = cairo_image_surface_get_width(logoImage);   // Original image width
    double logoHeight = cairo_image_surface_get_height(logoImage); // Original image height
    double scaleFactor = 100.0 / logoHeight; // Scale height to 100 pixels
    double scaledWidth = logoWidth * scaleFactor;

    /* Center logo */
    double offsetX = (width - scaledWidth) / 2.0;
    double offsetY = 10; // 10 pixels from the top

    /* Draw scaled logo */
    cairo_save(cr);
    cairo_translate(cr, x + offsetX, y + offsetY);
    cairo_scale(cr, scaleFactor, scaleFactor);
    cairo_set_source_surface(cr, logoImage, 0, 0);
    cairo_paint(cr);
    cairo_restore(cr);

    /* Set the font to bolded western-like font */
    cairo_select_font_face(cr, "Serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 18); // Set font size to 18 for better visibility

    /* Measuring text width */
    cairo_text_extents_t extents;
    cairo_text_extents(cr, gameTitle.c_str(), &extents);
    double textWidth = extents.width;
    
    /* Calculate centered position */
    double textX = x + (width - textWidth) / 2.0;
    double textY = y + height - 15;

    /* Set subdued Red */
    cairo_set_source_rgb(cr, 1.0, 0.0, 0.0); 

    /* Logo text */
    cairo_move_to(cr, textX, textY);
    cairo_show_text(cr, gameTitle.c_str());
}

void LogoBox::setLogo(const char* filepath) {
    if (logoImage) {
        cairo_surface_destroy(logoImage);
    }
    logoImage = cairo_image_surface_create_from_png(filepath);
}

/** PlayerBox methods **/

void PlayerBox::draw(cairo_t* cr) {
    /* Colors inherited from GameBox */
    drawBackground(cr);
    drawBorder(cr);
    setDefaultTextColor(cr);

    /* Player information */
    cairo_move_to(cr, x + 10, y + 20);
    cairo_show_text(cr, ("P" + to_string(player->getNumber()) + ": " + player->getName()).c_str());

    cairo_move_to(cr, x + 10, y + 40);
    cairo_show_text(cr, ("Score: " + to_string(player->getScore())).c_str());

    const Hand& hand = player->getHand();

    if (hand.getCardCount() > 0) {
        const Card& card1 = hand.getCard(0);
        drawCard(cr, card1, x + 10, y + 80);
    }

    if (hand.getCardCount() > 1) {
        const Card& card2 = hand.getCard(1);
        drawCard(cr, card2, x + 60, y + 80);
    }

    // Draw the chip indicator if the player has a dealer role
    DealerRole role = player->getDealerRole();
    if (role != NoRole) {
        drawChip(cr, role, x + (6 * GRID_SQ), y + (4.25 * GRID_SQ), (0.65 * GRID_SQ));
    } else if (player->getIsCurrentPlayer()) {
        // Drawing cowboy emoji for players with no role
        PangoLayout* layout = pango_cairo_create_layout(cr);

        // Set the font description
        PangoFontDescription* desc = pango_font_description_new();
        pango_font_description_set_family(desc, "Sans");
        pango_font_description_set_weight(desc, PANGO_WEIGHT_BOLD);
        pango_font_description_set_size(desc, 23 * PANGO_SCALE); // 12 point font size for better visibility
        pango_layout_set_font_description(layout, desc);
        pango_font_description_free(desc);

        // Setting cowboy emoji text
        pango_layout_set_text(layout, COWBOY_EMOJI, -1);

        // Get text dimensions
        int text_width, text_height;
        pango_layout_get_size(layout, &text_width, &text_height);
        text_width /= PANGO_SCALE;
        text_height /= PANGO_SCALE; 

        // Move to the desired position, centered within the area
        cairo_move_to(cr, x + (7 * GRID_SQ) - text_width / 2, y + (1 * GRID_SQ) - text_height / 2);

        // Render the layout
        pango_cairo_show_layout(cr, layout);

        // Clean up
        g_object_unref(layout);
    }
}

/** ScoreBox methods **/

void ScoreBox::draw(cairo_t* cr) {
    /* Colors inherited from GameBox */
    drawBackground(cr);
    drawBorder(cr);
    setDefaultTextColor(cr);

    // Main Pot text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);  // Adjust the font size as needed
    cairo_move_to(cr, x + 23, y + GRID_SQ);
    cairo_show_text(cr, "Main Pot");
    // Reset the font size to default
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);  // Adjust to your default font size
    cairo_set_source_rgb(cr, 0, 0, 0); // black color for text

    // Draw rectangle around main pot value
    double mainPotRectX = x + 10;
    double mainPotRectY = y + (GRID_SQ * 2) - 20;
    double rectWidth = 100; // adjust width
    double rectHeight = 30; // adjust height
    cairo_rectangle(cr, mainPotRectX, mainPotRectY, rectWidth, rectHeight);
    cairo_stroke(cr);

    // Set white background for the box
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White color
    cairo_rectangle(cr, mainPotRectX, mainPotRectY, rectWidth, rectHeight);
    cairo_fill(cr);
    // Draw the rectangle border
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Black color
    cairo_rectangle(cr, mainPotRectX, mainPotRectY, rectWidth, rectHeight);
    cairo_stroke(cr);

    // draw main pot value centered in rectangle
    cairo_text_extents_t extents;
    string mainPotStr = to_string(mainPot);
    cairo_text_extents(cr, mainPotStr.c_str(), &extents);
    double mainPotTextX = mainPotRectX + (rectWidth - extents.width) / 3 - extents.x_bearing;
    double mainPotTextY = mainPotRectY + (rectHeight - extents.height) / 2 + extents.height;
    cairo_move_to(cr, mainPotTextX, mainPotTextY);
    cairo_show_text(cr, mainPotStr.c_str());

    // Current Bet text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);  // Adjust the font size as needed
    cairo_set_source_rgb(cr, 1, 1, 1); // white color for text
    cairo_move_to(cr, x + 13, y + (GRID_SQ * 4));
    cairo_show_text(cr, "Current Bet");
    // Reset the font size to default
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 12);  // Adjust to your default font size
    cairo_set_source_rgb(cr, 0, 0, 0); // Black color for text

    // Draw the rectangle around the current bet value
    double currentBetRectX = x + 10;
    double currentBetRectY = y + (GRID_SQ * 5) - 20;
    cairo_rectangle(cr, currentBetRectX, currentBetRectY, rectWidth, rectHeight);
    cairo_stroke(cr);

    // Set white background for the box
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White color
    cairo_rectangle(cr, currentBetRectX, currentBetRectY, rectWidth, rectHeight);
    cairo_fill(cr);
    // Draw the rectangle border
    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Black color
    cairo_rectangle(cr, currentBetRectX, currentBetRectY, rectWidth, rectHeight);
    cairo_stroke(cr);

    // Draw the current bet value centered in the rectangle
    string currentBetStr = to_string(currentBet);
    cairo_text_extents(cr, currentBetStr.c_str(), &extents);
    double currentBetTextX = currentBetRectX + (rectWidth - extents.width) / 3 - extents.x_bearing;
    double currentBetTextY = currentBetRectY + (rectHeight - extents.height) / 2 + extents.height;
    cairo_move_to(cr, currentBetTextX, currentBetTextY);
    cairo_show_text(cr, currentBetStr.c_str());

    // Side Bets text
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size(cr, 14);  // Adjust the font size as needed
    cairo_set_source_rgb(cr, 1, 1, 1); // white text
    cairo_move_to(cr, x + 210, y + GRID_SQ);
    cairo_show_text(cr, "Side Bets");
    // Reset the font size to default
    cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, 10);  // Adjust to your default font size
    cairo_set_source_rgb(cr, 1, 1, 1); // white color for text


    // Print sideBets values
    for (int i = 1; i <= 4; ++i) {
        cairo_move_to(cr, x + 10 + (GRID_SQ * 8), y + (GRID_SQ * (1 + i)));
        if (sideBets[i] == 0) {
            cairo_show_text(cr, ("P" + to_string(i) + ": --").c_str());
        } else {
            cairo_show_text(cr, ("P" + to_string(i) + ": " + to_string(sideBets[i])).c_str());
        }
    }

    for (int i = 5; i <= 8; ++i) {
        cairo_move_to(cr, x + 10 + (GRID_SQ * 11), y + (GRID_SQ * (i - 3)));
        if (sideBets[i] == 0) {
            cairo_show_text(cr, ("P" + to_string(i) + ": --").c_str());
        } else {
            cairo_show_text(cr, ("P" + to_string(i) + ": " + to_string(sideBets[i])).c_str());
        }
    }
}

void ScoreBox::setControlBox(ControlBox* ctrlBox) {
    controlBox = ctrlBox;
}

void ScoreBox::setCurrentBet(int currentBetCounter) { 
    currentBet = currentBetCounter; 
    if (controlBox) {
        controlBox->updateButtonLabels();
    }
}

void ScoreBox::setMainPot(int mainPotCounter) { mainPot = mainPotCounter; }

void ScoreBox::setScoreBox(int mainPotCounter, int currentBetCounter) {
    mainPot = mainPotCounter;
    currentBet = currentBetCounter;
    // Update button labels whenever the current bet changes
    // Assuming there is a reference to controlBox, call updateButtonLabels() here
}

void ScoreBox::setSideBet(int index, int value) {
    if (index >= 0 && index < 9) {
        sideBets[index] = value;
    }
}

int ScoreBox::getCurrentBet() const { return currentBet; }

int ScoreBox::getMainPot() const { return mainPot; }

int ScoreBox::getSideBet(int index) const {
    if (index >= 0 && index < 9) {
        return sideBets[index];
    }
    return -1; // Return error code for invalid index
}

/** GameScreen methods **/

void GameScreen::initialize() {}

void GameScreen::show() { gtk_widget_show_all(window); }

/* Wipe and refresh a single Player */
void GameScreen::redrawPlayerBox(int playerIndex) {
    if (playerIndex >= 0 && playerIndex < static_cast<int>(playerBoxes.size())) {
        cairo_t *cr = gdk_cairo_create(drawingArea->window);
        playerBoxes[playerIndex]->draw(cr);
        cairo_destroy(cr);
    }
}

/* Wipe and refresh ALL Players */
void GameScreen::redrawAllPlayerBoxes() {
    for (size_t i = 0; i < playerBoxes.size(); ++i) {
        redrawPlayerBox(i);
    }
}

/* Box Setters */

void GameScreen::addPlayerBox(PlayerBox* pBox) { playerBoxes.push_back(pBox); }

void GameScreen::setConsoleBox(ConsoleBox* cBox) { consoleBox = cBox; }

void GameScreen::setBoardBox(BoardBox* bBox) { boardBox = bBox; }

void GameScreen::setControlBox(ControlBox* ctrlBox) { controlBox = ctrlBox; }

void GameScreen::setLogoBox(LogoBox* lgBox) { logoBox = lgBox; }

void GameScreen::setScoreBox(ScoreBox* sBox) { scoreBox = sBox; }

/* Box Getters */

BoardBox* GameScreen::getBoardBox() const { return boardBox; }       // Access BoardBox for printing log messages

ConsoleBox* GameScreen::getConsoleBox() const { return consoleBox; } // Access ConsoleBox for printing log messages

ControlBox* GameScreen::getControlBox() const { return controlBox; } // Access ControlBox to set/get playerBetEntry value

ScoreBox* GameScreen::getScoreBox() const { return scoreBox; }       // For access to set pots and currentBet in ScoreBox

void GameScreen::initializeBoxes(AllPlayers& allPlayers, BoardCards& boardCards) {
    /* Initialize PlayerBoxes */
    for (size_t i = 0; i < allPlayers.getPlayers().size(); ++i) {
        PlayerBox* pBox = new PlayerBox(&allPlayers.getPlayers()[i], (i % 4) * 8 * GRID_SQ, (i / 4) * 6 * GRID_SQ + 12 * GRID_SQ, 8 * GRID_SQ, 6 * GRID_SQ);
        addPlayerBox(pBox);
    }

    /* Initialize BoardBox */
    BoardBox* bBox = new BoardBox(&boardCards, 8 * GRID_SQ, 6 * GRID_SQ, 16 * GRID_SQ, 6 * GRID_SQ);
    setBoardBox(bBox);

    /* Initialize ConsoleBox */
    ConsoleBox* cBox = new ConsoleBox(24 * GRID_SQ, 0 * GRID_SQ, 8 * GRID_SQ, 12 * GRID_SQ);
    setConsoleBox(cBox);

    /* Initialize ScoreBox */
    ScoreBox* sBox = new ScoreBox(8 * GRID_SQ, 0 * GRID_SQ, 16 * GRID_SQ, 6 * GRID_SQ);
    sBox->setMainPot(0);
    sBox->setCurrentBet(0);
    setScoreBox(sBox);

    /* Initialize ControlBox */
    ControlBox* ctrlBox = new ControlBox(0 * GRID_SQ, 6 * GRID_SQ, 8 * GRID_SQ, 6 * GRID_SQ, fixedContainer, sBox, game);
    setControlBox(ctrlBox);

    /* Link ControlBox to ScoreBox */
    sBox->setControlBox(ctrlBox);

    /* Initialize LogoBox */
    LogoBox* lgBox = new LogoBox(0 * GRID_SQ, 0 * GRID_SQ, 8 * GRID_SQ, 6 * GRID_SQ, LOGO_PATH);
    setLogoBox(lgBox);
}

void GameScreen::initializeButtons() {
    Button* exampleButton = new Button(10, 10, 100, 40, "Click Me", []() {
        printf("Button clicked!\n");
    });
    buttons.push_back(exampleButton);
}

/* DrawCard Definition */

void drawCard(cairo_t* cr, const Card& card, int x, int y) {
    // Set background color to white
    cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
    cairo_rectangle(cr, x, y, 40, 50); // Adjust size as needed
    cairo_fill(cr);

    // Set text color based on visibility
    if (card.getIsVisible()) {
        if (card.getSuit() == Hearts || card.getSuit() == Diamonds) {
            cairo_set_source_rgb(cr, 0.9, 0.0, 0.0); // red
        } else if (card.getSuit() == Spades || card.getSuit() == Clubs) {
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // black
        } else {
            cairo_set_source_rgb(cr, 0.0, 0.0, 0.5); // navy blue
        }
    } else {
        cairo_set_source_rgb(cr, 0.0, 0.0, 0.5); // navy blue for hidden cards
    }

    // Create a Pango layout
    PangoLayout* layout = pango_cairo_create_layout(cr);

    // Set the font description
    PangoFontDescription* desc = pango_font_description_new();
    pango_font_description_set_family(desc, "Sans");
    pango_font_description_set_weight(desc, PANGO_WEIGHT_BOLD);
    pango_font_description_set_size(desc, CARD_SZ * PANGO_SCALE); // 20 point font size for better visibility
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    // Convert Unicode integer to string
    char unicodeStr[5];
    if (card.getIsVisible()) {
        int unicode = card.getUnicode();
        g_snprintf(unicodeStr, sizeof(unicodeStr), "%C", unicode);
    } else {
        g_snprintf(unicodeStr, sizeof(unicodeStr), "%C", CARD_BACK);
    }

    // Set the text to the layout
    pango_layout_set_text(layout, unicodeStr, -1);

    // Move to the desired position
    cairo_move_to(cr, x - 1, y + 1); // Adjust the position as needed

    // Render the layout
    pango_cairo_show_layout(cr, layout);

    // Clean up
    g_object_unref(layout);
}

void drawChip(cairo_t* cr, const DealerRole& chip, int x, int y, int radius) {
    // Set the chip color based on the role
    if (chip == LittleBlind || chip == BigBlind) {
        cairo_set_source_rgb(cr, 0.0, 0.75, 1.0); // Blue fill
    } else {
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0); // White fill
    }
    cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
    cairo_fill(cr);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0); // Black outline
    cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
    cairo_stroke(cr);

    // Create a Pango layout
    PangoLayout* layout = pango_cairo_create_layout(cr);

    // Set the font description
    PangoFontDescription* desc = pango_font_description_new();
    pango_font_description_set_family(desc, "Sans");
    pango_font_description_set_weight(desc, PANGO_WEIGHT_BOLD);
    pango_font_description_set_size(desc, 9 * PANGO_SCALE); // 9 point font size
    pango_layout_set_font_description(layout, desc);
    pango_font_description_free(desc);

    // Set the role text on the chip
    const char* roleStr = " ";
    if (chip == Dealer) {
        roleStr = "D";
    } else if (chip == LittleBlind) {
        roleStr = "LB";
    } else if (chip == BigBlind) {
        roleStr = "BB";
    }

    // Set the text to the layout
    pango_layout_set_text(layout, roleStr, -1);

    // Get text dimensions
    int text_width, text_height;
    pango_layout_get_size(layout, &text_width, &text_height);
    text_width /= PANGO_SCALE;
    text_height /= PANGO_SCALE;

    // Move to the desired position, centered within the chip
    cairo_move_to(cr, x - text_width / 2, y - text_height / 2);

    // Render the layout
    pango_cairo_show_layout(cr, layout);

    // Clean up
    g_object_unref(layout);
}

gboolean GameScreen::on_button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer user_data) {
    GameScreen* gameScreen = static_cast<GameScreen*>(user_data);
    for (Button* button : gameScreen->buttons) {
        if (button->isClicked(event->x, event->y)) {
            button->click();
            return TRUE; // Event handled
        }
    }

    // Handle ControlBox buttons
    if (gameScreen->controlBox) {
        if (gameScreen->controlBox->getIncreaseBetButton()->isClicked(event->x, event->y)) {
            gameScreen->controlBox->getIncreaseBetButton()->click();
            return TRUE; // Event handled
        }
        if (gameScreen->controlBox->getMatchBetButton()->isClicked(event->x, event->y)) {
            gameScreen->controlBox->getMatchBetButton()->click();
            return TRUE; // Event handled
        }
        if (gameScreen->controlBox->getFoldButton()->isClicked(event->x, event->y)) {
            gameScreen->controlBox->getFoldButton()->click();
            return TRUE; // Event handled
        }
        if (gameScreen->controlBox->getQuitButton()->isClicked(event->x, event->y)) {
            gameScreen->controlBox->getQuitButton()->click();
            return TRUE; // Event handled
        }
    }

    return FALSE; // Event not handled
}

/** Draw GTK objects **/
gboolean GameScreen::on_expose_event(GtkWidget *widget, GdkEventExpose *event, gpointer user_data) {
    cairo_t *cr = gdk_cairo_create(widget->window);

    /* GameScreen background */
    cairo_set_source_rgb(cr, 0.0, 0.5, 0.0); // 50% green
    cairo_paint(cr);

    /* Draw PlayerBox instances */
    GameScreen* gameScreen = static_cast<GameScreen*>(user_data);
    for (PlayerBox* pBox : gameScreen->playerBoxes) { pBox->draw(cr); }

    /* Draw ConsoleBox */
    if (gameScreen->consoleBox) {
        gameScreen->consoleBox->draw(cr);
    }

    /* Draw BoardBox */
    if (gameScreen->boardBox) { gameScreen->boardBox->draw(cr); }

    /* Draw ControlBox */
    if (gameScreen->controlBox) { gameScreen->controlBox->draw(cr); }

    /* Draw ScoreBox */
    if (gameScreen->scoreBox) { gameScreen->scoreBox->draw(cr); }

    /* Draw LogoBox */
    if (gameScreen->logoBox) { gameScreen->logoBox->draw(cr); }

    /* Draw Buttons */
    for (Button* button : gameScreen->buttons) { button->draw(cr); }

    cairo_destroy(cr);
    return FALSE;
}
