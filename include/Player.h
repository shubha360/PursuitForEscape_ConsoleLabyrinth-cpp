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
	std::string getPlayerInfo(); // return player info in string, used for rendering
	void updatePlayerAfterGameStateChange(); // updates player according to level file at new game, load game

	int getPositionX() { return _posX; };
	int getPositionY() { return _posY; };
	int getHealth() { return _currentHealth; };
	int getMoney() { return _money; };
	int getArtifactsCollected() { return _artifactsCollected; };

private:
	int _posX, _posY;
	int _oldX, _oldY; // for storing coordinates of previous position after a move
	Level* _currentLevel;
	Camera* _camera;

	int _currentHealth;
	int _money;
	int _artifactsCollected;

	int _shields;
	int _zombieInfectionHealers;
	int _impairedMovementHealers;

	int _zombieInfectedMoves; // remaining zombie infected moves after got bitten by zombie
	int _impairedMoves; // remaining impaired moves after got attacked by witch
	int _artifactsOfMonster; // artifacts hold by the monster

	std::string _playerLog[11] {"", "", "", "", "", "", "", "", "", "", ""}; // to store the logs

	void _combatEnemy(Enemy* enemy); // combat with an enemy
	bool _moveEnemies(); // moves enemies after player moves, returns false if player died from an enemy attack
	std::string _processAttackFromEnemy(Enemy* enemy, int damage); // processes an attack from enemy and returns log text, pass the enemy and damage done by the enemy
	std::string _processEnemyKill(Enemy* enemy); // processes enemy kill and returns log text

	void _addLog(std::string logText); // adds new log text to log array
	void _increaseHealth(int amountToIncrease); // increase health by specified amount
	bool _performAZombieInfectedMove(); // returns false if player died during the move
	void _performAnImpairedMove();

	void _moveUp();
	void _moveDown();
	void _moveLeft();
	void _moveRight();

	void _enterShop(); // starts the shop
	bool _buyItem(std::string itemName, int itemPrize); // returns true if player has enough money to buy the item
};

