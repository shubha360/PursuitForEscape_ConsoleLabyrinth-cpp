#include "../include/Camera.h"

#include <iostream>

const int Camera::CAMERA_WIDTH = 40;
const int Camera::CAMERA_HEIGHT = 20;

Camera::Camera(Level* level) {
	_posX = 0;
	_posY = 0;
	_currentLevel = level;

	_topAndBottomLine = string(CAMERA_WIDTH, '-');
	_screenIndentTop = string(5, '\n');
	_screenIndentBottom = string(5, '\n');
	_screenIndentLeft = string(5, ' ');
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