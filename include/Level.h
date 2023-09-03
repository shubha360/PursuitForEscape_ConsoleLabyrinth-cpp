#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class Level {
public:
	static const char SIGN_EMPTY;
	static const char SIGN_WALL;
	static const char SIGN_GATE_WALL;
	static const char SIGN_GATE_LOCKED;
	static const char SIGN_GATE_OPEN;
	static const char SIGN_PLAYER;
	static const char SIGN_ARTIFACT;

	static const std::string SAVE_FILE_LOCATION;
	static const std::string SAVE_FILE_DEFAULT_TEXT;

	Level();
	Level(std::string levelFileLocation);
	bool loadLevel(std::string currentFileLocation);
	void saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts);
	void deleteSaveGame();
	char getPositionAtGrid(int x, int y); // get the character at xy coordinate
	void setPlayer(int newX, int newY); // set player at xy coordinate
	void setPlayer(int newX, int newY, int oldX, int oldY); // set player at new xy coordinate and reset old xy coordinate
	void openEscapeGate(); // open the escape gate when all artifacts are collected

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

private:
	std::string _levelName;
	std::string _levelFileLocation;
	std::vector<std::string> _levelGrid;

	std::ifstream _inputStream;
	std::ofstream _outputStream;

	int _rows, _columns;
	int _playerX, _playerY;
	int _playerHealth, _playerMoney, _artifactsCollected;
	int _numberOfArtifacts;
	bool _levelLoaded;
	int _escapeX, _escapeY;
};

