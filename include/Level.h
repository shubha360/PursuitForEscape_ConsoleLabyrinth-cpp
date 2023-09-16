#pragma once

#include <string>
#include <fstream>

#include "Enemy.h"

class Level {
public:
	static const char SIGN_EMPTY;
	static const char SIGN_WALL;
	static const char SIGN_GATE_WALL;
	static const char SIGN_GATE_LOCKED;
	static const char SIGN_GATE_OPEN;
	static const char SIGN_PLAYER;
	static const char SIGN_ARTIFACT;
	static const char SIGN_SNAKE;
	static const char SIGN_ZOMBIE;
	static const char SIGN_WITCH;
	static const char SIGN_MONSTER;
	static const char SIGN_RANDOM_MONEY;
	static const char SIGN_10_HEALTH;
	static const char SIGN_REFILL_HEALTH;
	static const char SIGN_SHIELD;

	static const std::string SAVE_FILE_LOCATION;
	static const std::string SAVE_FILE_DEFAULT_TEXT;

	Level();
	~Level();
	void addLevelFile(std::string levelFileLocation);
	bool loadLevel(std::string currentFileLocation);
	void saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts,
                int zombieInfectedMoves, int shields);
	void deleteSaveGame();
	char getTileAtGrid(int x, int y); // get the character at xy coordinate
	void setPlayer(int newX, int newY); // set player at xy coordinate
	void setPlayer(int newX, int newY, int _oldX, int _oldY); // set player at new xy coordinate and reset old xy coordinate
	void erasePlayer(int playerX, int playerY); // erase player from grid
	void openEscapeGate(); // open the escape gate when all artifacts are collected

	// damageArr holds damages dealt by multiple enemies on player when they to the same spot as player
	// enemyNameArr holds the name of the enemies
	// at most 3 enemies can attack the player
	void moveEnemies(int playerX, int playerY, int playerHealth, int playerShields, int damageArr[], Enemy* enemyArr[]);

	void printLevel(); // print whole level at once

	std::string getLevelFileLocation() { return _levelFileLocation; }

	int getRows() { return _rows; }
	int getColumns() { return _columns; }
	int getNumberOfArtifacts(){ return _numberOfArtifacts; }
	int getEscapeX() { return _escapeX; }
	int getEscapeY() { return _escapeY; }

	int getPlayerX() { return _playerX; }
	int getPlayerY() { return _playerY; }
	int getPlayerHealth() { return _playerHealth; }
	int getPlayerMoney() { return _playerMoney; }
	int getArtifactsCollected() { return _artifactsCollected; }
	int getZombieInfectedMoves() { return _zombieInfMovesLeft; }
	int getShields() { return _shieldsLeft; }

	std::vector<std::string>& getLevelGrid() { return _levelGrid; }
	std::vector<std::vector<Enemy*>>& getEnemyGrid() { return _enemyGrid; }

private:
	std::string _levelName;
	std::string _levelFileLocation;
	std::vector<std::string> _levelGrid;

	std::vector<Enemy*> _enemies;
	std::vector<std::vector<Enemy *>> _enemyGrid;

	std::ifstream _inputStream;
	std::ofstream _outputStream;

	int _rows, _columns;
	int _playerX, _playerY;
	int _playerHealth, _playerMoney, _artifactsCollected;
	int _zombieInfMovesLeft, _shieldsLeft;
	int _numberOfArtifacts;
	bool _levelLoaded;
	int _escapeX, _escapeY;

	void _deleteEnemies();
};

