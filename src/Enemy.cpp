#include "../include/Enemy.h"

std::random_device Enemy::_seed;
std::mt19937 Enemy::RandomEngine(_seed());

void Enemy::move(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    if (!_moveTowardsPlayers(playerX, playerY, levelGrid, enemyGrid)) {
	    _moveRandomly(levelGrid, enemyGrid);
	}
}

bool Enemy::_moveTowardsPlayers(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	int absDisX = abs(_posX - playerX);
	int absDisY = abs(_posY - playerY);
	int distance = absDisX + absDisY;

	if (distance <= 5) {
		if (absDisY < absDisX) {
            if (absDisY == 0) {
                _moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
            }
            else {
                if (_posY < playerY) {
                    if (!_moveUp(levelGrid, enemyGrid)) {
                        _moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
                else if (_posY > playerY) {
                    if (!_moveDown(levelGrid, enemyGrid)) {
                        _moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
            }
		}
		else if (absDisX < absDisY) {
            if (absDisX == 0) {
                _moveVertically(playerX, playerY, levelGrid, enemyGrid);
            }
            else {
                if (_posX < playerX) {
                    if (!_moveLeft(levelGrid, enemyGrid)) {
                        _moveVertically(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
                else if (_posX > playerX) {
                    if (!_moveRight(levelGrid, enemyGrid)) {
                        _moveVertically(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
            }
		}
        return true;
	}
	return false;
}

void Enemy::_moveRandomly(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    static std::uniform_int_distribution<int> getMove(0, 4);

    int nextMove = getMove(RandomEngine);

    switch (nextMove) {

    case 0: // no move
        break;

    case 1: // up
        _moveUp(levelGrid, enemyGrid);
        break;

    case 2: // down
        _moveDown(levelGrid, enemyGrid);
        break;

    case 3: // left
        _moveLeft(levelGrid, enemyGrid);
        break;

    case 4: // right
        _moveRight(levelGrid, enemyGrid);
        break;
    }
}

void Enemy::_moveHorizontally(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    if (_posX < playerX) {
        _moveLeft(levelGrid, enemyGrid);
    }
    else if (_posX > playerX) {
        _moveRight(levelGrid, enemyGrid);
    }
}

void Enemy::_moveVertically(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    if (_posY < playerY) {
        _moveDown(levelGrid, enemyGrid);
    }
    else if (_posY > playerY) {
        _moveUp(levelGrid, enemyGrid);
    }
}

bool Enemy::_moveUp(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY - 1][_posX] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posY--;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY + 1][_posX];
		enemyGrid[_posY + 1][_posX] = nullptr;
		return true;
	}
	return false;
}

bool Enemy::_moveDown(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY + 1][_posX] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posY++;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY - 1][_posX];
		enemyGrid[_posY - 1][_posX] = nullptr;
		return true;
	}
	return false;
}

bool Enemy::_moveLeft(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY][_posX - 1] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posX--;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY][_posX + 1];
		enemyGrid[_posY][_posX + 1] = nullptr;
		return true;
	}
	return false;
}

bool Enemy::_moveRight(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY][_posX + 1] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posX++;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY][_posX - 1];
		enemyGrid[_posY][_posX - 1] = nullptr;
		return true;
	}
	return false;
}

Snake::Snake(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 1;
	_damage = 10;
	_moneyToGain = 20;
	_type = EnemyType::SNAKE;
	_sign = 'S';
	_alive = true;
}

Zombie::Zombie(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 3;
	_damage = 5;
	_moneyToGain = 50;
	_type = EnemyType::ZOMBIE;
	_sign = 'Z';
	_alive = true;
}

Witch::Witch(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 2;
	_damage = 30;
	_moneyToGain = 120;
	_type = EnemyType::WITCH;
	_sign = 'W';
	_alive = true;
}

Monster::Monster(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 5;
	_damage = 45;
	_moneyToGain = 200;
	_type = EnemyType::MONSTER;
	_sign = 'M';
	_alive = true;
}