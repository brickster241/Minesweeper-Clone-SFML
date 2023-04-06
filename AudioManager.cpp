#include <iostream>
#include <SFML/Audio.hpp>
using namespace std;

class AudioManager {
	sf::SoundBuffer buffer;
	sf::Sound sound;
public:
	// Plays Audio When Mouse is Hovering over Tile.
	void PlayGridHover() {
		buffer.loadFromFile("Audio\\hover-sound.wav");
		sound.setBuffer(buffer);
		sound.setVolume(10);
		sound.play();
	}

	// Plays Audio When Restart / Quit is clicked.
	void PlayButtonClick() {
		buffer.loadFromFile("Audio\\button-click.wav");
		sound.setBuffer(buffer);
		sound.setVolume(100);
		sound.play();
	}

	// Plays Audio when Game is Complete.
	void PlayGameComplete() {
		buffer.loadFromFile("Audio\\game-win.wav");
		sound.setBuffer(buffer);
		sound.setVolume(75);
		sound.play();
	}

	// Plays Audio when Game is Over.
	void PlayGameOver() {
		buffer.loadFromFile("Audio\\game-over.wav");
		sound.setBuffer(buffer);
		sound.setVolume(75);
		sound.play();
	}

	// Plays Audio when setting or removing Flag.
	void FlagAudio() {
		buffer.loadFromFile("Audio\\flag-place.wav");
		sound.setBuffer(buffer);
		sound.setVolume(75);
		sound.play();
	}

	
};