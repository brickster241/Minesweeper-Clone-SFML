#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid-UI-Manager.cpp"
using namespace std;

class GameManager {
public:
	GameStatus gameStatus;
	sf::RenderWindow window;
	AudioManager audioManager;
	// Constructor to Initialize
	GameManager() {
		gameStatus = GameStatus::InProgress;
		window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), TITLE, sf::Style::Close | sf::Style::Titlebar | sf::Style::Fullscreen);
	}

	// Play Game - Main Loop
	void PlayGame() {
		Grid gameGrid;
		UIManager gameUI;
		while (window.isOpen()) {
			sf::Event evnt;
			while (window.pollEvent(evnt))
			{
				if (evnt.type == sf::Event::Closed)
					window.close();
				if (evnt.type == sf::Event::MouseButtonReleased) {
					if (gameStatus == GameStatus::InProgress && evnt.mouseButton.button == sf::Mouse::Left) {
						gameGrid.UpdateGridOnLeftClick(sf::Mouse::getPosition(window), gameStatus, &audioManager);
					}
					if (gameStatus == GameStatus::InProgress && evnt.mouseButton.button == sf::Mouse::Right) {
						gameGrid.UpdateGridOnRightClick(sf::Mouse::getPosition(window), &audioManager);
					}
					pair<bool, bool> isButtonsClicked = gameUI.UpdateButtonOnHover(sf::Mouse::getPosition(window));
					if (isButtonsClicked.first) {
						audioManager.PlayButtonClick();
						gameGrid.ResetGrid();
						gameUI.UpdateUI(MINE_COUNT, gameStatus);
						gameStatus = GameStatus::InProgress;
					}
					else if (isButtonsClicked.second) {
						audioManager.PlayButtonClick();
						window.close();
					}
				}
			}
			gameGrid.UpdateGridOnHover(sf::Mouse::getPosition(window), gameStatus, &audioManager);
			gameUI.UpdateButtonOnHover(sf::Mouse::getPosition(window));
			gameUI.UpdateUI(gameGrid.getFlagCount(), gameStatus);
			if (gameGrid.visibleTiles() == ROWS * COLS - MINE_COUNT && gameStatus != GameStatus::Complete) {
				gameStatus = GameStatus::Complete;
				audioManager.PlayGameComplete();
			}
			window.clear();
			gameGrid.DrawGrid(window, gameStatus);
			gameUI.DrawUI(window);
			window.display();
		}
	}
};