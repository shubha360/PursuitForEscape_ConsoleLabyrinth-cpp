#include <iostream>
#include <conio.h>

#include "include/GameManager.h"

using namespace std;

int main() {

	string name = "Pursuit For Escape: Console Labyrinth";
	string signs = string(name.size(), '-');
	string emptyLines = string(2, '\n');
	string spaces = string(5, ' ');

	string intro = emptyLines + '\n' + spaces + signs + '\n' + spaces + name + '\n' + spaces + signs + emptyLines;
	cout << intro;

	string menu = "1. New Game\n2. Load Game\n3. Quit\n\nYour input: ";
	char input = ' ';

	do {
		cout << menu;
		input = _getch();
		cout << input << "\n\n";

		switch (input) {
		case '1':
			break;

		case '2':
			break;

		case '3':
			break;

		default:
			cout << "Unrecognized input! Please try again.\n\n";
			break;
		}

	} while (input != '3');


	/*Level level;
	
	if (level.loadLevel("files/level_1.txt")) {
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
	}*/

	cout << "Game ended\n\n";

	return 0;
}