#pragma once

#include <conio.h>

#include "Player.h"

enum class GameStates { MAIN_MENU, PLAY_GAME, HOW_TO_PLAY, EXIT };

class GameManager {
public:
	GameManager(std::string levelFileLocation);
	void playGame();

private:
	GameStates _state;

	Level _level;
	Camera _camera;
	Player _player;

	bool _levelLoaded;

	void _printIntro();
	void _printMainMenu();
	void _printHowToPlay();
};

