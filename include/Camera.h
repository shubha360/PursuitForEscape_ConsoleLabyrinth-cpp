#pragma once

#include "Level.h"

class Camera {
public:
	static const int CAMERA_WIDTH;
	static const int CAMERA_HEIGHT;

	Camera(Level* level);
	void setCameraPosition(int x, int y);
	void render();

private:
	int _posX, _posY;
	Level* _currentLevel;

	string _topAndBottomLine;
	string _screenIndentTop;
	string _screenIndentBottom;
	string _screenIndentLeft;
};

