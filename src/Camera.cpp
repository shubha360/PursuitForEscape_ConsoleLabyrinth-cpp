#include "../include/Camera.h"

#include <iostream>

const int Camera::CAMERA_WIDTH = 40;
const int Camera::CAMERA_HEIGHT = 20;

const string Camera::_topAndBottomLine = string(CAMERA_WIDTH, '-');
const string Camera::_screenIndentTop = string(3, '\n');
const string Camera::_screenIndentBottom = string(5, '\n');
const string Camera::_screenIndentLeft = string(5, ' ');

const vector<string> Camera::_legend = { 
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

Camera::Camera(Level* level) {
	_posX = 0;
	_posY = 0;
	_currentLevel = level;
}

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

void Camera::render() {
	cout << _screenIndentTop;

	int legendIterator = 0;

	for (int y = _posY - 1; y <= _posY + CAMERA_HEIGHT; y++) {

		if (y == _posY - 1 || y == _posY + CAMERA_HEIGHT) {
			cout << _screenIndentLeft << ' ' << _topAndBottomLine;
		}
		else {
			for (int x = _posX - 1; x <= _posX + CAMERA_WIDTH; x++) {
				
				if (x == _posX - 1 || x == _posX + CAMERA_WIDTH) {

					if (x == _posX - 1) {
						cout << _screenIndentLeft;
					}
					
					cout << '|';

					if (x == _posX + CAMERA_WIDTH && y >= 0 && legendIterator < _legend.size()) {
						cout << "  " << _legend[legendIterator++];
					}
				}
				else {
					cout << _currentLevel->getPositionAtGrid(x, y);
				}
			}
		}
		cout << endl;
	}
	//cout << _screenIndentBottom;
	cout << endl;
}