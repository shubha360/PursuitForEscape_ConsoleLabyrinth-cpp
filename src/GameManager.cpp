#include "../include/GameManager.h"

GameManager::GameManager(Level& level, Camera& camera, Player& player) {
	_level = level;
	_camera = camera;
	_player = player;
}
