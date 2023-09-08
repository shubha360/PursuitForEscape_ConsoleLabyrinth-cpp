#include "../include/Camera.h"

const int Camera::CAMERA_WIDTH = 40;
const int Camera::CAMERA_HEIGHT = 20;

const std::string Camera::_topAndBottomLine = std::string(CAMERA_WIDTH, '-');
const std::string Camera::_screenIndentTop = std::string(3, '\n');
const std::string Camera::_screenIndentBottom = std::string(5, '\n');
const std::string Camera::_screenIndentLeft = std::string(5, ' ');

const std::vector<std::string> Camera::_legend = {
	"#  Wall", 
	"=  Escape Gate Wall", 
	"X  Escape Gate Locked", 
	"~  Escape Gate Open", 
	"!  Artifacts", 
	"1  Shop", 
	"2  Refill Health", 
	"3  Map View", 
	"S  Snake", 
	"Z  Zombie", 
	"W  Witch", 
	"M  Monster", 
	"$  Random Money", 
	"+  10 Health", 
	"^  Shield For 5 Moves" 
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

// center the camera according to player's xy coordinate
void Camera::setCameraPosition(int x, int y) {
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

// render the view inside camera
void Camera::render() {
	std::cout << _screenIndentTop;

	int legendIterator = 0;
	std::string levelString = "";

	for (int y = _posY - 1; y <= _posY + CAMERA_HEIGHT; y++) {

		// print top and bottom borders
		if (y == _posY - 1 || y == _posY + CAMERA_HEIGHT) {
			levelString += _screenIndentLeft + " " + _topAndBottomLine;
		}

		// else print level lines
		else { 
			for (int x = _posX - 1; x <= _posX + CAMERA_WIDTH; x++) {
				
				// print left and right borders
				if (x == _posX - 1 || x == _posX + CAMERA_WIDTH) {

					if (x == _posX - 1) {
						levelString += _screenIndentLeft;
					}
					
					levelString += "|";

					// print legend at the right of the game view
					if (x == _posX + CAMERA_WIDTH && y >= 0 && legendIterator < _legend.size()) {
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
	std::cout << levelString;
}