#pragma once

#include "Level.h"

#include <conio.h>

class Camera {
public:
	static const int CAMERA_WIDTH;
	static const int CAMERA_HEIGHT;

	Camera();
	Camera(Level* level);
	void setCameraPosition(int x, int y); // center the camera according to player's xy coordinate
	void render(std::string playerInfo); // render the view inside camera

	void openMapView(std::string playerInfo);

private:
	int _posX, _posY;
	Level* _currentLevel;

	static const std::vector<std::string> _legend;

	static const std::string _topAndBottomLine;
	static const std::string _screenIndentTop;
	static const std::string _screenIndentBottom;
	static const std::string _screenIndentLeft;

	void _captureAndPrint(int cameraPosX, int cameraPosY, bool viewMapMode, std::string playerInfo);
};

