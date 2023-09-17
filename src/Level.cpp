#include "../include/Level.h"

const char Level::SIGN_EMPTY = ' ';
const char Level::SIGN_WALL = '#';
const char Level::SIGN_GATE_WALL = '=';
const char Level::SIGN_GATE_LOCKED = 'X';
const char Level::SIGN_GATE_OPEN = '~';
const char Level::SIGN_PLAYER = '@';
const char Level::SIGN_ARTIFACT = '!';
const char Level::SIGN_SNAKE = 'S';
const char Level::SIGN_ZOMBIE = 'Z';
const char Level::SIGN_WITCH = 'W';
const char Level::SIGN_MONSTER = 'M';
const char Level::SIGN_RANDOM_MONEY = '$';
const char Level::SIGN_10_HEALTH = '+';
const char Level::SIGN_REFILL_HEALTH = '*';
const char Level::SIGN_SHIELD = '^';

const std::string Level::SAVE_FILE_LOCATION = "files/save_file.txt";
const std::string Level::SAVE_FILE_DEFAULT_TEXT = "NO SAVE DATA";

Level::Level() {
	_levelFileLocation = "";
	_levelLoaded = false;
}

Level::~Level() {
	_deleteEnemies();
}

// deletes all the enemies and empties the enemy vector
void Level::_deleteEnemies() {
	for (int i = 0; i < _enemyGrid.size(); i++) {
		for (int j = 0; j < _enemyGrid[i].size(); j++) {
			if (_enemyGrid[i][j] != nullptr) {
				delete _enemyGrid[i][j];
				_enemyGrid[i][j] = nullptr;
			}
		}
	}

	for (int i = 0; i < _enemies.size(); i++) {
		_enemies[i] = nullptr;
	}
	_enemies.clear();
}

void Level::addLevelFile(std::string levelFileLocation) {
	_levelFileLocation = levelFileLocation;
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

	// emptying the enemy vector and grid before filling data again
	_deleteEnemies();

	_levelName = line;
	getline(_inputStream, line);

	_inputStream >> _rows;
	getline(_inputStream, line);
	_inputStream >> _columns;
	getline(_inputStream, line);
	getline(_inputStream, line);

    _inputStream >> _numberOfArtifacts;
	getline(_inputStream, line);
	getline(_inputStream, line);

	_inputStream >> _playerHealth;
	getline(_inputStream, line);
	_inputStream >> _playerMoney;
	getline(_inputStream, line);
	_inputStream >> _artifactsCollected;
	getline(_inputStream, line);
	_inputStream >> _shieldsLeft;
	getline(_inputStream, line);
	getline(_inputStream, line);

	_inputStream >> _zombieInfMovesLeft;
	getline(_inputStream, line);
	_inputStream >> _impairedMovesLeft;
	getline(_inputStream, line);
	_inputStream >> _artifactsOfMonster;
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

		_enemyGrid.push_back(std::vector<Enemy*>());
		_enemyGrid.back().resize(line.size(), nullptr);

		for (int j = 0; j < line.size(); j++) {
			switch (line[j]) {

            case SIGN_PLAYER:
                _playerX = j;
                _playerY = i;
                break;

            case SIGN_GATE_LOCKED:
                _escapeX = j;
                _escapeY = i;
                break;

			case SIGN_SNAKE:
				_enemies.push_back(new Snake(j, i));
				_enemyGrid[i][j] = _enemies.back();
				break;

			case SIGN_ZOMBIE:
				_enemies.push_back(new Zombie(j, i));
				_enemyGrid[i][j] = _enemies.back();
				break;

			case SIGN_WITCH:
				_enemies.push_back(new Witch(j, i));
				_enemyGrid[i][j] = _enemies.back();
				break;

			case SIGN_MONSTER:
				_enemies.push_back(new Monster(j, i));
				_enemyGrid[i][j] = _enemies.back();
				break;
			}
		}
	}
	_levelLoaded = true;
	_inputStream.close();
	return true;
}

void Level::saveLevel(int playerPosX, int playerPosY, int playerHealth, int playerMoney, int playerArtifacts, int shields,
                      int zombieInfectedMoves, int impairedMoves, int artifactsOfMonster) {

	_outputStream.open(SAVE_FILE_LOCATION);

	if (_outputStream.fail()) {
		perror(SAVE_FILE_LOCATION.c_str());
	}

	std::string output = _levelName + "\n\n" + std::to_string(_rows) + "\n" + std::to_string(_columns) + "\n\n"
        + std::to_string(_numberOfArtifacts) + "\n\n"
		+ std::to_string(playerHealth) + "\n" + std::to_string(playerMoney) + "\n" + std::to_string(playerArtifacts) + std::to_string(shields) + "\n\n"
		+ std::to_string(zombieInfectedMoves) + "\n" + std::to_string(impairedMoves) + "\n" + std::to_string(artifactsOfMonster) + "\n\n";

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

// damageArr holds damages dealt by multiple enemies on player when they to the same spot as player
// enemyNameArr holds the name of the enemies
// at most 3 enemies can attack the player
void Level::moveEnemies(int playerX, int playerY, int playerHealth, int playerShields, int damageArr[], Enemy* enemyArr[]) {

	// keeping count of the attacking enemies
	int attackerCount = 0;

	for (Enemy* enemy : _enemies) {
        if (enemy->isALive()) {
			int damageHolder = 0;
			int xHolder;
			int yHolder;

            enemy->move(playerX, playerY, playerHealth, damageHolder, xHolder, yHolder, _levelGrid, _enemyGrid);

			if (damageHolder > 0) { // this enemy attacked the player
				damageArr[attackerCount] = damageHolder;
				enemyArr[attackerCount] = _enemyGrid[yHolder][xHolder];

                if (playerShields > 0) {
                    playerShields--;
                }
                else {
                    playerHealth -= damageHolder;
                }
				attackerCount++;
			}
			else if (damageHolder == -1) { // this enemy was attacked by the player
				damageArr[attackerCount] = damageHolder;
				enemyArr[attackerCount] = _enemyGrid[yHolder][xHolder];
				attackerCount++;
			}
        }
	}

	// if less then 3 enemies attacked, fill the remaining array spots with zero
	while (attackerCount < 3) {
		damageArr[attackerCount] = 0;
		attackerCount++;
	}
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
char Level::getTileAtGrid(int x, int y) {
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
void Level::setPlayer(int newX, int newY, int _oldX, int _oldY) {
	if (!_levelLoaded) {
		std::cout << "Level not loaded!\n";
	}
	else {
		_levelGrid[_oldY][_oldX] = SIGN_EMPTY;
		_levelGrid[newY][newX] = SIGN_PLAYER;
	}
}

void Level::erasePlayer(int playerX, int playerY) {
	_levelGrid[playerY][playerX] = SIGN_EMPTY;
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
