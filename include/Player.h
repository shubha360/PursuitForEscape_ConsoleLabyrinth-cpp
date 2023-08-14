#pragma once

#include "Level.h"
#include "Camera.h"

class Player {
public:
	static const int PLAYER_HEALTH;
	static const int PLAYER_STARTING_MONEY;

	Player(Level* level, Camera* camera);
	bool movePlayer(char input);
	void printPlayerInfo();

private:
	int _posX, _posY;
	Level* _currentLevel;
	Camera* _camera;

	int _currentHealth;
	int _money;
	int _artifactsCollected;
};

