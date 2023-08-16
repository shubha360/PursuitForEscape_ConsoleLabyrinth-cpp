#pragma once

#include "Level.h"
#include "Camera.h"
#include "Player.h"

class GameManager {
public:
	GameManager(Level& level, Camera& camera, Player& player);

private:
	Level _level;
	Camera _camera;
	Player _player;
};

