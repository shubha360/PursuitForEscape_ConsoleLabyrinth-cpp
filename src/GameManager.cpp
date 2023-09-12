#include "../include/GameManager.h"

GameManager::GameManager(std::string levelFileLocation) {
	_state = GameStates::MAIN_MENU;
	_level.addLevelFile(levelFileLocation);
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

				case '3': // how to play
					_state = GameStates::HOW_TO_PLAY;
					break;

				case '4': // quit
					_state = GameStates::EXIT;
					std::cout << "Game exited!\n\n";
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

			_camera.render(_player.getPlayerInfo());

			do {
				char input = _getch();

				// returns false if player died or the level is completed
				if (!_player.movePlayer(input)) {
					break;
				}
			} while (true);

			_state = GameStates::MAIN_MENU;
			break;

		case GameStates::HOW_TO_PLAY:
			do {
				_printHowToPlay();
				std::cout << "Press any key to get back to the menu.\n\n";

				char input = _getch();
				break;

			} while (true);

			_state = GameStates::MAIN_MENU;
			break;
		}
	}
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
	static const std::string menu = "1. New Game\n2. Load Game\n3. How to play (Important if you are first timer)\n4. Exit Game\n\nYour input: ";
	std::cout << menu;
}

void GameManager::_printHowToPlay() {

	std::cout << std::string(40, '\n');
	_printIntro();

	static std::string htp =
        "Some system instructions -\n"
        "Set the CMD or the thing you're running this game on to full screen to have the proper view.\n"
        "If it is not possible set full screen by default, there should be some way around. You just have to google it.\n"
        "For example, on Windows 7 you can set the CMD to full screen by changing the layout measurements "
        "right clicking on the title bar -> properties -> layout\n"
        "You may experience clumsy frame loading if you're running it on a system that's too old.\n"
		"\n\n\n"
		"The main purpose of the player is to escape the labyrinth through the escape gate. "
		"But the escape gate remains locked until all the artifacts are collected. "
		"Also there are some ferocious enemies waiting to make a feast of the player! "
		"Player has to find all the artifacts killing or avoiding the enemies.\n\n"
		"Player and an enemy fight each other if they are in the same spot in the labyrinth or player moves to the adjacent spot to enemy. "
		"Because, enemy makes it's move after player. "
		"So if player is in the adjacent spot of enemy, enemy makes it's move towards the player's spot and fights player. "
		"It will be randomly determined who will attack whom. "
		"Any enemy will be killed with a single attack. "
		"Max health of player is 100 and game ends if the player dies from attack of an enemy.\n\n"
		"There are various power ups available to collect or to buy from the shop. "
		"Money is used to buy power ups. Money can be found around the labyrinth and by killing enemies. \n"
		"\n\n\n"
		"There are 4 different type of enemies. Each has it's own characteristics and powers.\n\n"

		"Snake - The weakest one. Can deal 15 - 20 health damage to the player. "
		"Can be eaten after killing for gaining 5 health back. "
		"Player earns 20 currency for killing a snake.\n\n"

		"Zombie - Can deal 10 - 15 health damage to the player at once and then 2 - 4 health damage for each next 5 moves. "
		"Total 35 damage at the worst case. "
		"Player earns 50 currency for killing a zombie.\n\n"

		"Witch - Can deal 40 - 50 health damage to the player and take away all the powerups for good. "
		"Player earns 120 currency for killing a witch.\n\n"

		"Monster - The strongest one. Can deal 75 - 95 health damage to the player. "
		"Player earns 200 currency for killing a monster.\n\n"

		"\n\n\n"
		"Symbols:\n\n"
		"@ - Player\n"
		"# - Wall\n"
		"= - Escape Gate Wall\n"
		"X - Escape Gate Locked\n"
		"~ - Escape Gate Unlocked\n"
		"! - Artifact\n"
		"S - Snake\n"
		"Z - Zombie\n"
		"W - Witch\n"
		"M - Monster\n"
		"\n\n\n"
		"Controls:\n\n"
		"W - Move Up\n"
		"S - Move Down\n"
		"A - Move Left\n"
		"D - Move Right\n"
		"N - New Game\n"
		"M - Load Game\n"
		"J - Save Game\n"
		"L - Delete Save Game\n"
		"Esc - Exit to main menu\n"
		"\n\n";

	std::cout << htp;
}
