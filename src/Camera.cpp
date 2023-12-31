#include "../include/Camera.h"

const int Camera::CAMERA_WIDTH = 40;
const int Camera::CAMERA_HEIGHT = 20;

const std::vector<std::string> Camera::_legend = {
	"Legends:                  Controls:",
	"",
	"#  Wall                 | WASD - Player movement",
	"=  Escape Gate Wall     | ",
	"X  Escape Gate Locked   | R - Use Zombie Infection Healer",
	"~  Escape Gate Unlocked | T - Use Impaired Movement Healer",
	"!  Artifacts            | ",
	"0  Shop                 | N - New game",
	"S  Snake                | M - Load game",
	"Z  Zombie               | ",
	"W  Witch                | J - Save game",
	"M  Monster              | K - Delete Save Game",
	"$  Random Money         | ",
	"+  10 Health            | Esc - Exit to main menu",
	"*  Refill Health        | ",
	"^  Shield For an Attack | ",
	"&  Map View             | ",
};

Camera::Camera() {
	_posX = -1;
	_posY = -1;
	_currentLevel = nullptr;
}

Camera::Camera(Level* level) {
	_posX = 0;
	_posY = 0;
	_currentLevel = level;
}

// centers the camera according to player's xy coordinate
void Camera::setCameraPosition(const int& x, const int& y) {
	_posY = y - CAMERA_HEIGHT / 2;

	if (_posY < 0) {
		_posY = 0;
	}
	else if (_posY > _currentLevel->getRows() - CAMERA_HEIGHT) {
		_posY = _currentLevel->getRows() - CAMERA_HEIGHT;
	}

	_posX = x - CAMERA_WIDTH / 2;

	if (_posX < 0) {
		_posX = 0;
	}
	else if (_posX > _currentLevel->getColumns() - CAMERA_WIDTH) {
		_posX = _currentLevel->getColumns() - CAMERA_WIDTH;
	}
}

// renders the view inside camera
void Camera::render(const std::string& playerInfo) {
	_captureAndPrint(_posX, _posY, false, playerInfo);
}

// opens the map view mode
void Camera::openMapView(const std::string& playerInfo) {
	int viewMapPosX = 0;
	int viewMapPosY = 0;

	// for reducing print calls
	int oldViewMapPosX = viewMapPosX;
	int oldViewMapPosY = viewMapPosY;

	_captureAndPrint(viewMapPosX, viewMapPosY, true, playerInfo);

	char input = ' ';

	do {		

		// print if position changed
		if (oldViewMapPosX != viewMapPosX || oldViewMapPosY != viewMapPosY) {
			
			_captureAndPrint(viewMapPosX, viewMapPosY, true, playerInfo);
			oldViewMapPosX = viewMapPosX;
			oldViewMapPosY = viewMapPosY;
		}

		input = _getch();

		switch (input) {
		case 'w': // up
			if (viewMapPosY > 0) {
				viewMapPosY--;
			}
			break;

		case 's': // down
			if (viewMapPosY + CAMERA_HEIGHT < _currentLevel->getRows()) {
				viewMapPosY++;
			}
			break;

		case 'a': // left
			if (viewMapPosX > 0) {
				viewMapPosX--;
			}
			break;

		case 'd': // right
			if (viewMapPosX + CAMERA_WIDTH < _currentLevel->getColumns()) {
				viewMapPosX++;
			}
			break;
		}

	} while (input != 27);
}

void Camera::_captureAndPrint(const int& cameraPosX, const int& cameraPosY, const bool& mapViewMode, const std::string& playerInfo) {
	static const std::string _topAndBottomLine(CAMERA_WIDTH, '-');
	static const std::string _screenIndentTop(25, '\n');
	static const std::string _screenIndentBottom(10, '\n');
	static const std::string _screenIndentLeft(5, ' ');

	static const std::string mapViewHeading =
		"###        ###       ###       #########      ##             ## ########## ######### ##                     ##\n"
		"## ##    ## ##      ## ##      ##     ##       ##           ##      ##     ##         ##                   ## \n"
		"##  ##  ##  ##     ##   ##     ##     ##        ##         ##       ##     ##          ##       ###       ##  \n"
		"##    ##    ##    ##     ##    #########         ##       ##        ##     ########     ##     ## ##     ##   \n"
		"##          ##   ###########   ##                 ##     ##         ##     ##            ##   ##   ##   ##    \n"
		"##          ##  ##         ##  ##                   ##  ##          ##     ##             ## ##     ## ##     \n"
		"##          ## ##           ## ##                     ##        ########## #########       ###       ###      \n"
		"\n"
		"Use WASD to move around.\n"
		"Press Esc to exit map view.\n\n";

	int legendIterator = 0;
	std::string levelString = _screenIndentTop;

	// print map view heading if in map view mode
	if (mapViewMode) {
		levelString += mapViewHeading;
	}

	for (int y = cameraPosY - 1; y <= cameraPosY + CAMERA_HEIGHT; y++) {

		// print top and bottom borders
		if (y == cameraPosY - 1 || y == cameraPosY + CAMERA_HEIGHT) {
			levelString += _screenIndentLeft + " " + _topAndBottomLine;
		}

		// else print level lines
		else {
			for (int x = cameraPosX - 1; x <= cameraPosX + CAMERA_WIDTH; x++) {

				// print left and right borders
				if (x == cameraPosX - 1 || x == cameraPosX + CAMERA_WIDTH) {

					if (x == cameraPosX - 1) {
						levelString += _screenIndentLeft;
					}

					levelString += "|";

					// print legend at the right of the game view
					if (x == cameraPosX + CAMERA_WIDTH && y >= 0 && legendIterator < _legend.size()) {
						levelString += "  " + _legend[legendIterator++];
					}
				}

				// else print level characters
				else {
					levelString += _currentLevel->getTileAtGrid(x, y);
				}
			}
		}
		levelString += "\n";
	}

	levelString += "\n";
	levelString += playerInfo;
	levelString += _screenIndentBottom;

	std::cout << levelString;
}