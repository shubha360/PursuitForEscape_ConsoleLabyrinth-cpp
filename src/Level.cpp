#include "../include/Level.h"

const char Level::SIGN_EMPTY = ' ';
const char Level::SIGN_WALL = '#';
const char Level::SIGN_GATE_WALL = '=';
const char Level::SIGN_GATE_LOCKED = 'X';
const char Level::SIGN_GATE_OPEN = '~';
const char Level::SIGN_PLAYER = '@';
const char Level::SIGN_ARTIFACT = '!';

const string Level::SAVE_FILE_LOCATION = "files/save_file.txt";
const string Level::SAVE_FILE_DEFAULT_TEXT = "NO SAVE DATA";

Level::Level(string levelFileLocation) {
	_levelFileLocation = levelFileLocation;
	_levelLoaded = false;
}

bool Level::loadLevel(string currentFileLocation) {
	ifstream loadFile;
	loadFile.open(currentFileLocation);

	if (loadFile.fail()) {
		perror(currentFileLocation.c_str());
		return false;
	}

	string line;
	
	getline(loadFile, line);

	if (line == SAVE_FILE_DEFAULT_TEXT) {
		loadFile.close();
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

		if (_levelGrid.size() < _rows) {
			_levelGrid.push_back(line);
		}
		else {
			_levelGrid[i] = line;
		}
	}
	_levelLoaded = true;
	loadFile.close();
	return true;
}

void Level::saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts) {
	ofstream saveFile;
	saveFile.open(SAVE_FILE_LOCATION);

	if (saveFile.fail()) {
		perror(SAVE_FILE_LOCATION.c_str());
	}

	string output = _levelName + "\n\n" + to_string(_rows) + "\n" + to_string(_columns) + "\n\n"
		+ to_string(playerPosX) + "\n" + to_string(playerPosY) + "\n\n"
		+ to_string(_escapeX) + "\n" + to_string(_escapeY) + "\n\n"
		+ to_string(playerHealth) + "\n" + to_string(playerMoney) + "\n" + to_string(playerArtifacts) + "\n\n"
		+ to_string(_numberOfArtifacts) + "\n\n";

	for (int i = 0; i < _rows; i++) {
		output += _levelGrid[i];

		if (i < _rows - 1) {
			output += "\n";
		}
	}
	saveFile << output;
	saveFile.close();
}

void Level::deleteSaveGame() {
	ofstream saveFile;
	saveFile.open(SAVE_FILE_LOCATION);

	if (saveFile.fail()) {
		perror(SAVE_FILE_LOCATION.c_str());
	}

	saveFile << SAVE_FILE_DEFAULT_TEXT;
	saveFile.close();
}

// print whole level at once
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

// get the character at xy coordinate
char Level::getPositionAtGrid(int x, int y) {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
		return SIGN_WALL;
	}
	return _levelGrid[y][x];
}

// set player at xy coordinate
void Level::setPlayer(int newX, int newY) {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

// set player at new xy coordinate and reset old xy coordinate
void Level::setPlayer(int newX, int newY, int oldX, int oldY) {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[oldY][oldX] = SIGN_EMPTY;
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

// open the escape gate once all artifacts are collected
void Level::openEscapeGate() {
	if (!_levelLoaded) {
		cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[_escapeY][_escapeX] = SIGN_GATE_OPEN;
	}
}