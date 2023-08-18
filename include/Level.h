#pragma once

#include <string>
#include <vector>

using namespace std;

class Level {
public:
	static const char SIGN_EMPTY;
	static const char SIGN_WALL;
	static const char SIGN_GATE_WALL;
	static const char SIGN_GATE_LOCKED;
	static const char SIGN_GATE_OPEN;
	static const char SIGN_PLAYER;
	static const char SIGN_ARTIFACT;

	static const string SAVE_FILE_LOCATION;
	static const string SAVE_FILE_DEFAULT_TEXT;

	Level(string levelFileLocation);
	bool loadLevel(string currentFileLocation);
	void saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts);
	void deleteSaveGame();
	void printLevel();
	char getPositionAtGrid(int x, int y);
	void setPlayer(int newX, int newY);
	void setPlayer(int newX, int newY, int oldX, int oldY);
	void openEscapeGate();

	string getLevelFileLocation() { return _levelFileLocation; }

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
	string _levelName;
	string _levelFileLocation;
	vector<string> _levelGrid;

	int _rows, _columns;
	int _playerX, _playerY;
	int _playerHealth, _playerMoney, _artifactsCollected;
	int _numberOfArtifacts;
	bool _levelLoaded;
	int _escapeX, _escapeY;
};

