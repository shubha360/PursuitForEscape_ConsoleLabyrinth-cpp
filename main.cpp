#include <iostream>
#include <conio.h>

#include "include/Level.h"
#include "include/Player.h"

using namespace std;

int main() {

	Level level("1", "files/level_1.txt");
	
	if (level.loadLevel()) {
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

	cout << "\n\nGame ended\n\n";

	return 0;
}