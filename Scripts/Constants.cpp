#include <SFML/Graphics.hpp>

#define CELL_SIZE 75
#define SPRITE_SIZE 50
#define FONT_SIZE 40
#define OUTLINE_THICKNESS -5.0f
#define OFFSET 75.0f
#define ROWS 18
#define COLS 12
#define MINE_COUNT 25
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define TITLE "MineSweeper C++ & SFML"
#define RESTART_BUTTON_POSX 1525
#define RESTART_BUTTON_POSY 500
#define QUIT_BUTTON_POSX 1525
#define QUIT_BUTTON_POSY 700
#define FLAG_BUTTON_POSX 1525
#define FLAG_BUTTON_POSY 300
#define UI_BUTTON_WIDTH 300
#define UI_BUTTON_HEIGHT 100
#define TILE_FONT_PATH "Assets\\Fonts\\Antonio-Regular.ttf"
#define FLAG_SPRITE_PATH "Assets\\Sprites\\flag.png"
#define MINE_SPRITE_PATH "Assets\\Sprites\\mine.png"
#define DEFAULT " "
#define CLICKED_COLOR sf::Color{ 180, 180, 255, 255 }
#define HOVER_COLOR sf::Color{ 120, 255, 120, 255 }
#define TILE_COLOR sf::Color::Green
#define BUTTON_FONT "Assets\\Fonts\\Antonio-Regular.ttf"
#define BUTTON_TEXT_COLOR sf::Color::Black
#define BUTTON_TEXT_SIZE 50
#define BUTTON_COLOR sf::Color{200, 200, 255}
#define BUTTON_HOVER_COLOR sf::Color{150, 150, 255}
#define BUTTON_OUTLINE_COLOR sf::Color{175, 175, 255, 255}
#define RESTART_BUTTON_TEXT "RESTART"
#define QUIT_BUTTON_TEXT "QUIT"
#define RESTART_TEXT_POS sf::Vector2f(1600, 525)
#define QUIT_TEXT_POS sf::Vector2f(1625, 725)
#define FLAG_TEXT_STRING " : 25 / 25"
#define FLAG_TEXT_COLOR sf::Color::White
#define FLAG_SPRITE_POSX 1575
#define FLAG_SPRITE_POSY 325
#define FLAG_TEXT_POS sf::Vector2f(1625, 325)
#define STATUS_FONT_PATH "Assets\\Fonts\\Mitr-Regular.ttf"
#define STATUS_FONT_SIZE 75
#define STATUS_TEXT_POS sf::Vector2f(1475, 125)
#define COMPLETE_STATUS "YOU WIN !!"
#define FAILED_STATUS "GAME OVER"
#define STATUS_PROGRESS_COLOR sf::Color::Black
#define STATUS_COMPLETE_COLOR sf::Color::Green
#define STATUS_FAILED_COLOR sf::Color::Red


enum TileStatus {
	Mine,
	Safe
};

// Game Status Based on Progress
enum GameStatus {
	Complete,
	Failed,
	InProgress
};