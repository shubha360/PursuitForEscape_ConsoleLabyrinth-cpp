#include "../include/GameManager.h"

GameManager::GameManager(std::string levelFileLocation) {
	_state = GameStates::MAIN_MENU;
	_level = Level(levelFileLocation);
	_levelLoaded = false;
}

void GameManager::playGame() {

	while (_state != GameStates::EXIT) {
		
		switch (_state) {

		case GameStates::MAIN_MENU:

			_printIntro();

			do {
				_printMainMenu();

				char input = ' ';
				input = _getch();
				printf("%c\n\n", input);

				switch (input) {
				case '1': // new game
					_levelLoaded = _level.loadLevel(_level.getLevelFileLocation());

					if (!_levelLoaded) {
						std::cout << "Something went wrong!\n\n";
					}
					else {
						_state = GameStates::PLAY_GAME;
					}
					break;

				case '2': // load game
					_levelLoaded = _level.loadLevel(Level::SAVE_FILE_LOCATION);

					if (!_levelLoaded) {
						std::cout << "No load game exists!\n\n";
					}
					else {
						_state = GameStates::PLAY_GAME;
					}
					break;

				case '3': // quit
					_state = GameStates::EXIT;
					break;

				default:
					std::cout << "Unrecognized input! Please try again.\n\n";
					break;
				}
			} while (_state == GameStates::MAIN_MENU);

			if (_levelLoaded) {
				_camera = Camera(&_level);
				_player = Player(&_level, &_camera);
			}

			break;

		case GameStates::PLAY_GAME:

			_camera.render();
			_player.printPlayerInfo();

			do {
				char input = _getch();

				if (!_player.movePlayer(input)) {
					break;
				}
			} while (true);

			_state = GameStates::EXIT;
			break;
		}
	}

	std::cout << "Game exited!\n\n";
}

void GameManager::_printIntro() {
	static const std::string name = "Pursuit For Escape: Console Labyrinth";
	static const std::string signs = std::string(name.size(), '-');
	static const std::string emptyLines = std::string(2, '\n');
	static const std::string spaces = std::string(5, ' ');

	static std::string intro = emptyLines + '\n' + spaces + signs + '\n' + spaces + name + '\n' + spaces + signs + emptyLines;
	std::cout << intro;
}

void GameManager::_printMainMenu() {
	static const std::string menu = "1. New Game\n2. Load Game\n3. Exit Game\n\nYour input: ";
	std::cout << menu;
}