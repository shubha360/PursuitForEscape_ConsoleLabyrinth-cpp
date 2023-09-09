#pragma once

#include "Level.h"

class Camera {
public:
	static const int CAMERA_WIDTH;
	static const int CAMERA_HEIGHT;

	Camera();
	Camera(Level* level);
	void setCameraPosition(int x, int y); // center the camera according to player's xy coordinate
	void render(); // render the view inside camera

private:
	int _posX, _posY;
	Level* _currentLevel;

	static const std::vector<std::string> _legend;
};

