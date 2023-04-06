#include <iostream>
#include <vector>
#include <utility>
#include <random>
#include <queue>
#include <SFML/Graphics.hpp>
#include "Tile.cpp"
#include "AudioManager.cpp"
using namespace std;

class Grid {
	int clickedCount;
	int flagCount;
	bool displayMines;
	Tile grid[ROWS][COLS];
	pair<int, int> prevHoverTile;
public:
	// No. of flags placed.
	int getFlagCount() {
		return flagCount;
	}

	// No. of tiles visible or clicked.
	int visibleTiles() {
		return clickedCount;
	}

	// Constructor for Initializing and setting Tile Position.
	Grid() {
		ResetGrid();
	}

	// Reset Grid whenever you want to play Again.
	void ResetGrid() {
		flagCount = MINE_COUNT;
		displayMines = false;
		clickedCount = 0;
		prevHoverTile = { -1, -1 };
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				grid[i][j].SetTilePosition(OFFSET + CELL_SIZE * i, OFFSET + CELL_SIZE * j);
				grid[i][j].isClicked = false;
				grid[i][j].isHover = false;
				grid[i][j].isFlagVisible = false;
				grid[i][j].SetTileStatus(TileStatus::Safe);
				grid[i][j].SetAdjacentMineCount(0);
			}
		}
	}
	// Check if Row & Col are valid in Grid.
	bool isValidTileGrid(int row, int col) {
		if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
			return true;
		}
		return false;
	}

	// Randomize Mines & Set Adjacent Mines Values based on Initial Mouse Click.
	void RandomizeMineAndSetGridValues(sf::Vector2i mouse_pos) {
		float pos_x = mouse_pos.x;
		float pos_y = mouse_pos.y;
		int row, col;
		if (pos_x < OFFSET || pos_y < OFFSET || pos_x >= OFFSET + CELL_SIZE * ROWS || pos_y >= OFFSET + CELL_SIZE * COLS) {
			row = -1;
			col = -1;
		}
		else {
			row = (int)((pos_x - OFFSET) / CELL_SIZE) % ROWS;
			col = (int)((pos_y - OFFSET) / CELL_SIZE) % COLS;
		}

		const int range_from = 0;
		const int range_to = ROWS;
		std::random_device rand_dev;
		std::mt19937 generator(rand_dev());
		std::uniform_int_distribution<int> distr(range_from, range_to);

		for (int i = 0; i < MINE_COUNT; i++) {
			int rand_row, rand_col;
			do {
				rand_row = distr(generator) % ROWS;
				rand_col = distr(generator) % COLS;
			} while (grid[rand_row][rand_col].isTileMine() || (rand_row == row && rand_col == col));
			grid[rand_row][rand_col].SetTileStatus(TileStatus::Mine);
		}

		int dx[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
		int dy[8] = { -1, 0, 1, 0, 1, -1, -1, 1 };

		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				for (int k = 0; k < 8; k++) {
					int neighbour_r = i + dy[k];
					int neighbour_c = j + dx[k];
					if (isValidTileGrid(neighbour_r, neighbour_c) && grid[neighbour_r][neighbour_c].isTileMine()) {
						int adjCount = grid[i][j].GetAdjacentMineCount();
						grid[i][j].SetAdjacentMineCount(adjCount + 1);
					}
				}
			}
		}
	}


	// Change Grid Color Value on Hover.
	void UpdateGridOnHover(sf::Vector2i mouse_pos, GameStatus gameStatus, AudioManager *audioManager) {
		if (gameStatus != GameStatus::InProgress)
			return;
		float pos_x = mouse_pos.x;
		float pos_y = mouse_pos.y;
		if (prevHoverTile.first != -1) {
			grid[prevHoverTile.first][prevHoverTile.second].isHover = false;
		}
		if (pos_x < OFFSET || pos_y < OFFSET || pos_x >= OFFSET + CELL_SIZE * ROWS || pos_y >= OFFSET + CELL_SIZE * COLS)
			return;
		int row = (int)((pos_x - OFFSET) / CELL_SIZE) % ROWS;
		int col = (int)((pos_y - OFFSET) / CELL_SIZE) % COLS;
		bool playAudio = (prevHoverTile.first == -1 && prevHoverTile.second == -1) || !(prevHoverTile.first == row && prevHoverTile.second == col);
		if (playAudio && !grid[row][col].isClicked)
			audioManager->PlayGridHover();
		prevHoverTile = { row, col };
		grid[row][col].isHover = true;

	}


	// Draw Grid on window.
	void DrawGrid(sf::RenderWindow& window, GameStatus gameStatus) {
		for (int i = 0; i < ROWS; i++) {
			for (int j = 0; j < COLS; j++) {
				grid[i][j].DrawTile(window, gameStatus);
			}
		}
	}


	// Update Grid Value after Left Click - Handles Grid Operations.
	void UpdateGridOnLeftClick(sf::Vector2i mouse_pos, GameStatus& gameStatus, AudioManager* audioManager) {
		if (gameStatus != GameStatus::InProgress)
			return;
		float pos_x = mouse_pos.x;
		float pos_y = mouse_pos.y;
		if (pos_x < OFFSET || pos_y < OFFSET || pos_x >= OFFSET + CELL_SIZE * ROWS || pos_y >= OFFSET + CELL_SIZE * COLS)
			return;
		// VALID INPUT - CHECK FOR CLICKCOUNT
		if (clickedCount == 0)
			RandomizeMineAndSetGridValues(mouse_pos);
		int row = (int)((pos_x - OFFSET) / CELL_SIZE) % ROWS;
		int col = (int)((pos_y - OFFSET) / CELL_SIZE) % COLS;
		if (grid[row][col].isFlagVisible) {
			grid[row][col].isFlagVisible = false;
			audioManager->FlagAudio();
			flagCount += 1;
			return;
		}
		if (grid[row][col].isTileMine()) {
			gameStatus = GameStatus::Failed;
			audioManager->PlayGameOver();
			return;
		}
		if (grid[row][col].isClicked)
			return;
		if (grid[row][col].GetAdjacentMineCount() > 0) {
			clickedCount += 1;
			grid[row][col].isClicked = true;
			return;
		}
		clickedCount += 1;
		grid[row][col].isClicked = true;
		queue<pair<int, int>> q;
		int dx[8] = { 0, 1, 0, -1, 1, -1, 1, -1 };
		int dy[8] = { -1, 0, 1, 0, 1, -1, -1, 1 };
		q.push({ row, col });
		while (!q.empty()) {
			pair<int, int> row_col_pair = q.front();
			int r = row_col_pair.first;
			int c = row_col_pair.second;
			if (grid[r][c].isFlagVisible) {
				grid[r][c].isFlagVisible = false;
				flagCount += 1;
			}
			q.pop();
			for (int i = 0; i < 8; i++) {
				int new_r = r + dy[i];
				int new_c = c + dx[i];
				if (isValidTileGrid(new_r, new_c)) {
					// check for multiple conditions
					int adjacentMines = grid[new_r][new_c].GetAdjacentMineCount();
					bool isAlreadyNotClicked = !grid[new_r][new_c].isClicked && !grid[new_r][new_c].isTileMine();
					if (isAlreadyNotClicked) {
						clickedCount += 1;
						grid[new_r][new_c].isClicked = true;
						if (grid[new_r][new_c].isFlagVisible) {
							grid[new_r][new_c].isFlagVisible = false;
							flagCount += 1;
						}
					}
					if (isAlreadyNotClicked && adjacentMines == 0)
						q.push({ new_r, new_c });
				}
			}
		}
	}

	// Adds Flag at the Mouse Position.
	void UpdateGridOnRightClick(sf::Vector2i mouse_pos, AudioManager* audioManager) {
		float pos_x = mouse_pos.x;
		float pos_y = mouse_pos.y;
		if (flagCount == 0 || pos_x < OFFSET || pos_y < OFFSET || pos_x >= OFFSET + CELL_SIZE * ROWS || pos_y >= OFFSET + CELL_SIZE * COLS)
			return;
		int row = (int)((pos_x - OFFSET) / CELL_SIZE) % ROWS;
		int col = (int)((pos_y - OFFSET) / CELL_SIZE) % COLS;
		if (isValidTileGrid(row, col) && !grid[row][col].isClicked) {
			grid[row][col].isFlagVisible = true;
			audioManager->FlagAudio();
			flagCount -= 1;
		}
	}
};

class UIManager {
	sf::Text statusText;
	sf::Font statusFont;
	sf::Texture flagTexture;
	sf::Sprite Flag_Sprite;
	sf::RectangleShape FlagButton;
	sf::Text FlagText;
	sf::RectangleShape RestartButton;
	sf::Text RestartText;
	sf::RectangleShape QuitButton;
	sf::Text QuitText;
	sf::Font font;
public:
	// Constructor To Initialize All Elements.
	UIManager() {
		// font + Text + Button
		font.loadFromFile(BUTTON_FONT);
		RestartText.setFont(font);
		RestartText.setFillColor(BUTTON_TEXT_COLOR);
		RestartText.setCharacterSize(BUTTON_TEXT_SIZE);
		RestartText.setString(RESTART_BUTTON_TEXT);
		RestartText.setPosition(RESTART_TEXT_POS);
		// RESTART BUTTON
		RestartButton = sf::RectangleShape(sf::Vector2f(UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT));
		RestartButton.setPosition(RESTART_BUTTON_POSX, RESTART_BUTTON_POSY);
		RestartButton.setFillColor(BUTTON_COLOR);
		RestartButton.setOutlineThickness(OUTLINE_THICKNESS);
		RestartButton.setOutlineColor(BUTTON_OUTLINE_COLOR);

		// QUIT TEXT
		QuitText.setFont(font);
		QuitText.setFillColor(BUTTON_TEXT_COLOR);
		QuitText.setCharacterSize(BUTTON_TEXT_SIZE);
		QuitText.setString(QUIT_BUTTON_TEXT);
		QuitText.setPosition(QUIT_TEXT_POS);
		// QUIT BUTTON
		QuitButton = sf::RectangleShape(sf::Vector2f(UI_BUTTON_WIDTH, UI_BUTTON_HEIGHT));
		QuitButton.setPosition(QUIT_BUTTON_POSX, QUIT_BUTTON_POSY);
		QuitButton.setFillColor(BUTTON_COLOR);
		QuitButton.setOutlineThickness(OUTLINE_THICKNESS);
		QuitButton.setOutlineColor(BUTTON_OUTLINE_COLOR);

		// FLAG TEXT
		FlagText.setFont(font);
		FlagText.setFillColor(FLAG_TEXT_COLOR);
		FlagText.setCharacterSize(BUTTON_TEXT_SIZE);
		FlagText.setString(FLAG_TEXT_STRING);
		FlagText.setPosition(FLAG_TEXT_POS);

		// Textures & Sprites
		flagTexture.loadFromFile(FLAG_SPRITE_PATH);
		Flag_Sprite.setTexture(flagTexture);
		sf::Vector2f targetSize(SPRITE_SIZE, SPRITE_SIZE);
		Flag_Sprite.setScale(targetSize.x / Flag_Sprite.getLocalBounds().width, targetSize.y / Flag_Sprite.getLocalBounds().height);
		Flag_Sprite.setPosition(FLAG_SPRITE_POSX, FLAG_SPRITE_POSY);

		// GAME STATUS TEXT
		statusFont.loadFromFile(STATUS_FONT_PATH);
		statusText.setFont(statusFont);
		statusText.setFillColor(STATUS_PROGRESS_COLOR);
		statusText.setCharacterSize(STATUS_FONT_SIZE);
		statusText.setString(DEFAULT);
		statusText.setPosition(STATUS_TEXT_POS);
	}

	// Checks & Updates Restart & Quit Buttons on Hover. Also returns true if hovering.
	pair<bool, bool> UpdateButtonOnHover(sf::Vector2i mouse_pos) {
		float pos_x = mouse_pos.x;
		float pos_y = mouse_pos.y;
		if (pos_x > RESTART_BUTTON_POSX && pos_x < RESTART_BUTTON_POSX + UI_BUTTON_WIDTH && pos_y > RESTART_BUTTON_POSY && pos_y < RESTART_BUTTON_POSY + UI_BUTTON_HEIGHT) {
			RestartButton.setFillColor(BUTTON_HOVER_COLOR);
			return { true, false };
		}
		else if (pos_x > QUIT_BUTTON_POSX && pos_x < QUIT_BUTTON_POSX + UI_BUTTON_WIDTH && pos_y > QUIT_BUTTON_POSY && pos_y < QUIT_BUTTON_POSY + UI_BUTTON_HEIGHT) {
			QuitButton.setFillColor(BUTTON_HOVER_COLOR);
			return { false, true };
		}
		else {
			RestartButton.setFillColor(BUTTON_COLOR);
			QuitButton.setFillColor(BUTTON_COLOR);
			return { false, false };
		}
	}

	// Draw all Buttons & Texts.
	void DrawUI(sf::RenderWindow& window) {
		window.draw(statusText);
		window.draw(RestartButton);
		window.draw(QuitButton);
		window.draw(Flag_Sprite);
		window.draw(FlagText);
		window.draw(RestartText);
		window.draw(QuitText);
	}

	// Update Flag Count & Status Text Based On GameStatus.
	void UpdateUI(int flagCount, GameStatus gameStatus) {
		string s = " : " + to_string(flagCount) + " / " + to_string(MINE_COUNT);
		FlagText.setString(s);
		if (gameStatus == GameStatus::InProgress) {
			statusText.setString(DEFAULT);
			statusText.setFillColor(STATUS_PROGRESS_COLOR);
		}
		else if (gameStatus == GameStatus::Complete) {
			statusText.setString(COMPLETE_STATUS);
			statusText.setFillColor(STATUS_COMPLETE_COLOR);
		}
		else {
			statusText.setString(FAILED_STATUS);
			statusText.setFillColor(STATUS_FAILED_COLOR);
		}
	}
};