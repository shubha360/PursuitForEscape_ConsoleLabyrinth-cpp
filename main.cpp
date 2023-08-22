#include <conio.h>

#include "include/Player.h"

using namespace std;

int main() {

	// setting up the intro and main menu
	string name = "Pursuit For Escape: Console Labyrinth";
	string signs = string(name.size(), '-');
	string emptyLines = string(2, '\n');
	string spaces = string(5, ' ');

	string intro = emptyLines + '\n' + spaces + signs + '\n' + spaces + name + '\n' + spaces + signs + emptyLines;
	cout << intro;

	string menu = "1. New Game\n2. Load Game\n3. Exit Game\n\nYour input: ";
	char input = ' ';
	bool introLoop = true;
	
	Level level("files/level_1.txt");
	string levelFileLocation;
	bool levelLoaded = false;

	do {
		cout << menu;
		input = _getch();
		cout << input << "\n\n";

		switch (input) {
		case '1': // new game
			levelLoaded = level.loadLevel(level.getLevelFileLocation());

			if (levelLoaded) {
				introLoop = false;
			}
			else {
				cout << "Something went wrong!\n\n";
			}
			break;

		case '2': // load game
			levelLoaded = level.loadLevel(Level::SAVE_FILE_LOCATION);

			if (levelLoaded) {
				introLoop = false;
			}
			else {
				cout << "No load game exists!\n\n";
			}
			break;

		case '3': // quit
			introLoop = false;
			break;

		default:
			cout << "Unrecognized input! Please try again.\n\n";
			break;
		}

	} while (introLoop);

	
	if (levelLoaded) {
		Camera camera(&level);
		Player player(&level, &camera);

		camera.render();
		player.printPlayerInfo();

		do {
			char input = _getch();

			if (!player.movePlayer(input)) {
				break;
			}

		} while (true);
	}

	cout << "Game exited!\n\n";

	return 0;
}