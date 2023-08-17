#include "../include/Level.h"

#include <fstream>
#include <iostream>

const char Level::SIGN_EMPTY = ' ';
const char Level::SIGN_WALL = '#';
const char Level::SIGN_GATE_WALL = '=';
const char Level::SIGN_GATE_LOCKED = 'X';
const char Level::SIGN_GATE_OPEN = '~';
const char Level::SIGN_PLAYER = '@';
const char Level::SIGN_ARTIFACT = '!';

Level::Level() {
	_levelName = "NONE";
	_levelFileLocation = "NONE";
	_rows = -1;
	_columns = -1;
	_numberOfArtifacts = -1;
	_levelLoaded = false;
}

bool Level::loadLevel(string levelFileLocation) {
	_levelFileLocation = levelFileLocation;

	ifstream loadFile;
	loadFile.open(_levelFileLocation);

	if (loadFile.fail()) {
		perror(_levelFileLocation.c_str());
		return false;
	}

	string line;
	
	getline(loadFile, line);

	if (line == "NO SAVE DATA") {
		return false;
	}

	_levelName = line;
	getline(loadFile, line);

	loadFile >> _rows;
	getline(loadFile, line);
	loadFile >> _columns;
	getline(loadFile, line);
	getline(loadFile, line);

	loadFile >> _playerX;
	getline(loadFile, line);
	loadFile >> _playerY;
	getline(loadFile, line);
	getline(loadFile, line);

	loadFile >> _escapeX;
	getline(loadFile, line);
	loadFile >> _escapeY;
	getline(loadFile, line);
	getline(loadFile, line);

	loadFile >> _playerHealth;
	getline(loadFile, line);

	loadFile >> _playerMoney;
	getline(loadFile, line);

	loadFile >> _artifactsCollected;
	getline(loadFile, line);
	getline(loadFile, line);

	loadFile >> _numberOfArtifacts;
	getline(loadFile, line);
	getline(loadFile, line);

	for (int i = 0; i < _rows; i++) {
		getline(loadFile, line);
		_levelGrid.push_back(line);
	}

	_levelLoaded = true;
	return true;
}

void Level::saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts) {
	ofstream saveFile;
	string saveFileLocation = "files/save_file.txt";

	saveFile.open(saveFileLocation);

	if (saveFile.fail()) {
		perror(saveFileLocation.c_str());
	}

	saveFile << _levelName << "\n\n";
	
	saveFile << _rows << endl;
	saveFile << _columns << "\n\n";

	saveFile << playerPosX << endl;
	saveFile << playerPosY << "\n\n";

	saveFile << _escapeX << endl;
	saveFile << _escapeY << "\n\n";

	saveFile << playerHealth << endl;
	saveFile << playerMoney << endl;
	saveFile << playerArtifacts << "\n\n";

	saveFile << _numberOfArtifacts << "\n\n";

	for (int i = 0; i < _rows; i++) {
		saveFile << _levelGrid[i];

		if (i < _rows - 1) {
			saveFile << endl;
		}
	}
}

void Level::printLevel() {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		for (int i = 0; i < _rows; i++) {
			cout << _levelGrid[i] << endl;
		}
	}
}

char Level::getPositionAtGrid(int x, int y) {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
		return SIGN_WALL;
	}
	return _levelGrid[y][x];
}

void Level::setPlayer(int newX, int newY) {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

void Level::setPlayer(int newX, int newY, int oldX, int oldY) {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[oldY][oldX] = SIGN_EMPTY;
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

void Level::openEscapeGate() {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[_escapeY][_escapeX] = SIGN_GATE_OPEN;
	}
}