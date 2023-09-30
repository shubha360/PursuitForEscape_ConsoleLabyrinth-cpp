#pragma once

#include "Level.h"

#include <conio.h>

class Camera {
public:
	static const int CAMERA_WIDTH;
	static const int CAMERA_HEIGHT;

	Camera();
	Camera(Level* level);
	void setCameraPosition(const int& x, const int& y); // center the camera according to player's xy coordinate
	void render(const std::string& playerInfo); // render the view inside camera

	void openMapView(const std::string& playerInfo); // opens map view

private:
	int _posX, _posY;
	Level* _currentLevel;

	static const std::vector<std::string> _legend;

	// takes the part of the level to print inside camera
	// mapViewMode is true if in map view
	// playerInfo is printed right under the level view
	void _captureAndPrint(const int& cameraPosX, const int& cameraPosY, const bool& mapViewMode, const std::string& playerInfo);
};

