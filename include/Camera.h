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

	static const string _topAndBottomLine;
	static const string _screenIndentTop;
	static const string _screenIndentBottom;
	static const string _screenIndentLeft;
};

