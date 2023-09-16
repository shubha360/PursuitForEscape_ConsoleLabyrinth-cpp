#pragma once

#include "Level.h"
#include "Camera.h"

class Player {
public:
	static const int PLAYER_HEALTH;
	static const int PLAYER_STARTING_MONEY;

	Player();
	Player(Level* level, Camera* camera);
	bool movePlayer(char input);
	std::string getPlayerInfo();
	void updatePlayerAfterGameStateChange();

	int getPositionX() { return _posX; };
	int getPositionY() { return _posY; };
	int getHealth() { return _currentHealth; };
	int getMoney() { return _money; };
	int getArtifactsCollected() { return _artifactsCollected; };

private:
	int _posX, _posY;
	int _oldX, _oldY; // for storing coordinates of previous spot after a move
	Level* _currentLevel;
	Camera* _camera;

	int _currentHealth;
	int _money;
	int _artifactsCollected;
	int _shields;

	int _zombieInfectedMoves; // remaining zombie infected moves after got bitten by zombie
	bool _zombieAttackedNow; // to determine if zombie attacked in the previous move

	std::string _playerLog[5] {"", "", "", ""}; // to store the logs

	void _combatEnemy(Enemy* enemy);
	bool _moveEnemies(); // moves enemies after player moves, returns false if player died from an enemy attack
	std::string _processAttackFromEnemy(Enemy* enemy, int damage); // processes attack from enemy and returns log text
	std::string _processEnemyKill(Enemy* enemy); // processes enemy kill and returns log text

	void _addLog(std::string logText); // adds new log text to log array
	void _increaseHealth(int amountToAdd); // increase health by specified amount
};

