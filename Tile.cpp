#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.cpp"
using namespace std;

class Tile {
	int adjMineCount;
	sf::Font font;
	sf::Text text;
	sf::Texture flagTexture;
	sf::Texture mineTexture;
	sf::Sprite Mine_Sprite;
	sf::Sprite Flag_Sprite;
	TileStatus tileStatus;
	sf::RectangleShape sqr;

	
public:
	bool isFlagVisible;
	bool isClicked;
	bool isHover;
	
	// Constructor to Initialize
	Tile() {
		// VARIABLES
		isFlagVisible = false;
		isClicked = false;
		isHover = false;
		adjMineCount = 0;
		tileStatus = TileStatus::Safe;

		// sqr
		sqr = sf::RectangleShape(sf::Vector2f(CELL_SIZE, CELL_SIZE));
		sqr.setFillColor(sf::Color::Green);
		sqr.setOutlineThickness(OUTLINE_THICKNESS);
		sqr.setOutlineColor(sf::Color::Black);

		// text + font
		font.loadFromFile(TILE_FONT_PATH);
		text.setFont(font);
		text.setFillColor(sf::Color::Black);
		text.setCharacterSize(FONT_SIZE);
		text.setString(DEFAULT);

		// Textures & Sprites
		flagTexture.loadFromFile(FLAG_SPRITE_PATH);
		mineTexture.loadFromFile(MINE_SPRITE_PATH);
		Flag_Sprite.setTexture(flagTexture);
		Mine_Sprite.setTexture(mineTexture);
		// Resize
		sf::Vector2f targetSize(SPRITE_SIZE, SPRITE_SIZE);
		Flag_Sprite.setScale(targetSize.x / Flag_Sprite.getLocalBounds().width, targetSize.y / Flag_Sprite.getLocalBounds().height);
		Mine_Sprite.setScale(targetSize.x / Mine_Sprite.getLocalBounds().width, targetSize.y / Mine_Sprite.getLocalBounds().height);

	}

	// Sets Text Value if Tile is Clicked.
	void SetTileText () {
		if (!isClicked || adjMineCount == 0 || isTileMine()) {
			text.setString(DEFAULT);
		}
		else {
			text.setString(to_string(adjMineCount));
		}
	}

	// Sets Tile Color based on Click, Hover
	void SetTileColor() {
		if (isClicked) {
			sqr.setFillColor(CLICKED_COLOR);
		}
		else if (isHover) {
			sqr.setFillColor(HOVER_COLOR);
		}
		else {
			sqr.setFillColor(TILE_COLOR);
		}
	}

	// DrawTile
	void DrawTile(sf::RenderWindow& window, GameStatus gameStatus) {
		SetTileText();
		SetTileColor();
		window.draw(sqr);
		window.draw(text);
		DrawSprites(window, gameStatus);
	}

	// Draw Mine or Flag
	void DrawSprites(sf::RenderWindow& window, GameStatus gameStatus) {
		if (gameStatus == GameStatus::InProgress && isFlagVisible) {
			window.draw(Flag_Sprite);
		}
		else if (gameStatus != GameStatus::InProgress && isTileMine()) {
			window.draw(Mine_Sprite);
		}
	}

	// Sets Tile, Flag, Mine Sprite Positions.
	void SetTilePosition(float tilePos_x, float tilePos_y) {
		text.setPosition(sf::Vector2f(tilePos_x + 30, tilePos_y + 15));
		Flag_Sprite.setPosition(sf::Vector2f(tilePos_x + 20, tilePos_y + 15));
		Mine_Sprite.setPosition(sf::Vector2f(tilePos_x + 25, tilePos_y + 15));
		sqr.setPosition(sf::Vector2f(tilePos_x, tilePos_y));
	}

	// Sets count after traversing the neighbours.
	void SetAdjacentMineCount(int count) {
		this->adjMineCount = count;
	}

	// Returns no. of adjacent mines.
	int GetAdjacentMineCount() {
		return adjMineCount;
	}

	// Sets Tile status as mine or safe.
	void SetTileStatus(TileStatus tileStatus) {
		this->tileStatus = tileStatus;
	}

	// Return if Tile is a mine.
	bool isTileMine() {
		return tileStatus == TileStatus::Mine;
	}

};