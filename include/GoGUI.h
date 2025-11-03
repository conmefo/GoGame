#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "Game.h"


enum class ScreenState {
    MAIN_MENU,
    GAME_SCREEN,
    SETTINGS
};


struct Button {
    sf::RectangleShape shape;
    sf::Text text;

    bool isMouseOver( sf::RenderWindow& window) ;
};


class GameUI {
public:

    GameUI();

    void run();

private:

    void handleEvents();
    void update();
    void render();


    void handleMenuEvents( sf::Event& event);

    void handleGameEvents( sf::Event& event);
    
    void processBoardClick( sf::Vector2i& mousePosition);

    void drawCurrentScreen();

    void drawMainMenu();

    void drawGameScreen();
    
    void drawBoard();
    void drawStones();
    void drawTurnIndicator();
    void drawCaptureCounts();
    void drawGameButtons(); 
    void drawEndGameMessage();

    void loadAssets();

    void setupMainMenu();

    void setupGameButtons();

    bool mapPixelToBoardCoords( sf::Vector2i& mousePosition, int& boardX, int& boardY);


    sf::RenderWindow window;    
    Game game;                
    ScreenState currentScreen;   


    sf::Font mainFont;           
    
    sf::Text titleText;
    std::map<std::string, Button> menuButtons;

    std::map<std::string, Button> gameButtons;

    float boardPadding;
    float boardSizePixels;
    float cellWidth;
    sf::Vector2f boardTopLeft;
};