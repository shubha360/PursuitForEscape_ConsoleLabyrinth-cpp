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
	static const char SIGN_SHOP;
	static const char SIGN_SNAKE;
	static const char SIGN_ZOMBIE;
	static const char SIGN_WITCH;
	static const char SIGN_MONSTER;
	static const char SIGN_RANDOM_MONEY;
	static const char SIGN_10_HEALTH;
	static const char SIGN_REFILL_HEALTH;
	static const char SIGN_SHIELD;
	static const char SIGN_MAP_VIEW;

	static const std::string SAVE_FILE_LOCATION;
	static const std::string SAVE_FILE_DEFAULT_TEXT;

	Level();
	~Level();
	void addLevelFile(std::string levelFileLocation);
	bool loadLevel(std::string fileLocation);
	void saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts,
                int shields, int zombieInfectionHealers, int impairedMoveHealers,
                int zombieInfectedMoves, int impairedMoves, int artifactsOfMonster);
	void deleteSaveGame();
	char getTileAtGrid(int x, int y); // get the tile at xy coordinate
	void setPlayer(int newX, int newY); // set player at xy coordinate
	void setPlayer(int newX, int newY, int _oldX, int _oldY); // set player at new xy coordinate and reset old xy coordinate
	void erasePlayer(int playerX, int playerY); // erase player from grid, if died
	void openEscapeGate(); // open the escape gate when all artifacts are collected

	// damageArr holds damages dealt by multiple enemies on player when they moved to the same spot as player
	// enemyArr holds the enemies
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
	int getShields() { return _shields; }
	int getZombieInfectionHealers() { return _zombieInfHealers; }
	int getImpairedMoveHealers() { return _impairedMovementHealers; }
	int getZombieInfectedMoves() { return _zombieInfMovesLeft; }
	int getImpairedMoves() { return _impairedMovesLeft; }
	int getArtifactsHoldByMonster() { return _artifactsOfMonster; }

	std::vector<std::string>& getLevelGrid() { return _levelGrid; }
	std::vector<std::vector<Enemy*>>& getEnemyGrid() { return _enemyGrid; }

private:
	std::string _levelName;
	std::string _levelFileLocation;
	std::vector<std::string> _levelGrid; // main grid

	std::vector<Enemy*> _enemies; // list of the enemies
	std::vector<std::vector<Enemy *>> _enemyGrid; // enemy grid to find an enemy at constant time

	std::ifstream _inputStream; // for loading level
	std::ofstream _outputStream; // for saving level

	int _rows, _columns;
	int _playerX, _playerY;
	int _playerHealth, _playerMoney, _artifactsCollected;
	int _shields, _zombieInfHealers, _impairedMovementHealers;
	int _zombieInfMovesLeft, _impairedMovesLeft, _artifactsOfMonster;
	int _numberOfArtifacts;
	bool _levelLoaded;
	int _escapeX, _escapeY;

	void _deleteLevel(); // deletes the enemies at the end of the game
};

