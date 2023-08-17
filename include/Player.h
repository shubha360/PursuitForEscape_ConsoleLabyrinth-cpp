#pragma once

#include "Level.h"
#include "Camera.h"

#include <sstream>

class Player {
public:
	static const int PLAYER_HEALTH;
	static const int PLAYER_STARTING_MONEY;

	Player(Level* level, Camera* camera);
	bool movePlayer(char input);
	void printPlayerInfo();

	int getPositionX() { return _posX; };
	int getPositionY() { return _posY; };
	int getHealth() { return _currentHealth; };
	int getMoney() { return _money; };
	int getArtifactsCollected() { return _artifactsCollected; };

private:
	int _posX, _posY;
	Level* _currentLevel;
	Camera* _camera;

	int _currentHealth;
	int _money;
	int _artifactsCollected;
};

