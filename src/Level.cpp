#include "../include/Level.h"

const char Level::SIGN_EMPTY = ' ';
const char Level::SIGN_WALL = '#';
const char Level::SIGN_GATE_WALL = '=';
const char Level::SIGN_GATE_LOCKED = 'X';
const char Level::SIGN_GATE_OPEN = '~';
const char Level::SIGN_PLAYER = '@';
const char Level::SIGN_ARTIFACT = '!';

const std::string Level::SAVE_FILE_LOCATION = "files/save_file.txt";
const std::string Level::SAVE_FILE_DEFAULT_TEXT = "NO SAVE DATA";

Level::Level() {
	_levelFileLocation = "";
	_levelLoaded = false;
}

Level::Level(std::string levelFileLocation) {
	_levelFileLocation = levelFileLocation;
	_levelLoaded = false;
}

bool Level::loadLevel(std::string currentFileLocation) {
	_inputStream.open(currentFileLocation);

	if (_inputStream.fail()) {
		perror(currentFileLocation.c_str());
		return false;
	}

	std::string line;
	
	getline(_inputStream, line);

	if (line == SAVE_FILE_DEFAULT_TEXT) {
		_inputStream.close();
		return false;
	}

	_levelName = line;
	getline(_inputStream, line);

	_inputStream >> _rows;
	getline(_inputStream, line);
	_inputStream >> _columns;
	getline(_inputStream, line);
	getline(_inputStream, line);

	_inputStream >> _playerX;
	getline(_inputStream, line);
	_inputStream >> _playerY;
	getline(_inputStream, line);
	getline(_inputStream, line);

	_inputStream >> _escapeX;
	getline(_inputStream, line);
	_inputStream >> _escapeY;
	getline(_inputStream, line);
	getline(_inputStream, line);

	_inputStream >> _playerHealth;
	getline(_inputStream, line);

	_inputStream >> _playerMoney;
	getline(_inputStream, line);

	_inputStream >> _artifactsCollected;
	getline(_inputStream, line);
	getline(_inputStream, line);

	_inputStream >> _numberOfArtifacts;
	getline(_inputStream, line);
	getline(_inputStream, line);

	for (int i = 0; i < _rows; i++) {
		getline(_inputStream, line);

		if (_levelGrid.size() < _rows) {
			_levelGrid.push_back(line);
		}
		else {
			_levelGrid[i] = line;
		}
	}
	_levelLoaded = true;
	_inputStream.close();
	return true;
}

void Level::saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts) {
	_outputStream.open(SAVE_FILE_LOCATION);

	if (_outputStream.fail()) {
		perror(SAVE_FILE_LOCATION.c_str());
	}

	std::string output = _levelName + "\n\n" + std::to_string(_rows) + "\n" + std::to_string(_columns) + "\n\n"
		+ std::to_string(playerPosX) + "\n" + std::to_string(playerPosY) + "\n\n"
		+ std::to_string(_escapeX) + "\n" + std::to_string(_escapeY) + "\n\n"
		+ std::to_string(playerHealth) + "\n" + std::to_string(playerMoney) + "\n" + std::to_string(playerArtifacts) + "\n\n"
		+ std::to_string(_numberOfArtifacts) + "\n\n";

	for (int i = 0; i < _rows; i++) {
		output += _levelGrid[i];

		if (i < _rows - 1) {
			output += "\n";
		}
	}
	_outputStream << output;
	_outputStream.close();
}

void Level::deleteSaveGame() {
	_outputStream.open(SAVE_FILE_LOCATION);

	if (_outputStream.fail()) {
		perror(SAVE_FILE_LOCATION.c_str());
	}

	_outputStream << SAVE_FILE_DEFAULT_TEXT;
	_outputStream.close();
}

// print whole level at once
void Level::printLevel() {
	if (!_levelLoaded) {
		std::cout << "Level not loaded!\n";
	}
	else {
		for (int i = 0; i < _rows; i++) {
			std::cout << _levelGrid[i] << std::endl;
		}
	}
}

// get the character at xy coordinate
char Level::getPositionAtGrid(int x, int y) {
	if (!_levelLoaded) {
		std::cout << "Level not loaded!\n";
		return SIGN_WALL;
	}
	return _levelGrid[y][x];
}

// set player at xy coordinate
void Level::setPlayer(int newX, int newY) {
	if (!_levelLoaded) {
		std::cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

// set player at new xy coordinate and reset old xy coordinate
void Level::setPlayer(int newX, int newY, int oldX, int oldY) {
	if (!_levelLoaded) {
		std::cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[oldY][oldX] = SIGN_EMPTY;
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

// open the escape gate once all artifacts are collected
void Level::openEscapeGate() {
	if (!_levelLoaded) {
		std::cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[_escapeY][_escapeX] = SIGN_GATE_OPEN;
	}
}