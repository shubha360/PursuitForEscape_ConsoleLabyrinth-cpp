#include "../include/Enemy.h"

std::random_device Enemy::_seed;
std::mt19937 Enemy::RandomEngine(_seed());

int Enemy::getDamage() {
	return _generateDamage(RandomEngine);
}

// damageHolder holds damage dealt by an enemy while moving, always pass a integer reference with value 0 to get the damage
// enemyNameHolder holds the name of the enemy, pass any string reference to get the enemy name
// if enemy attacked damageHolder will be the damage value, if player attacked damageHolder will be -1
void Enemy::move(int playerX, int playerY, int playerHealth, int& damageHolder, int& enemyXHolder, int& enemyYHolder, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	// attack if player is in a spot adjacent to this enemy
	if ((playerX == _posX - 1 && playerY == _posY) ||
		(playerX == _posX + 1 && playerY == _posY) ||
		(playerX == _posX && playerY == _posY - 1) ||
		(playerX == _posX && playerY == _posY + 1)) {

		if (_resting) { // don't attack if this enemy was in fight with player in the previous move from player
			_resting = false;
		}
		else {
			if (playerHealth > 0) { // attack if player is still alive

				static std::uniform_int_distribution<int> getAttacker(1, 2);

				int attacker = getAttacker(RandomEngine);
				//int attacker = 1;

				switch (attacker) {
				case 1: // enemy attack
					damageHolder = _generateDamage(RandomEngine);

					if (playerHealth - damageHolder <= 0) { // if player died, set this enemy's position to player's position
						levelGrid[playerY][playerX] = _sign;
						levelGrid[_posY][_posX] = ' ';

						enemyGrid[playerY][playerX] = enemyGrid[_posY][_posX];
						enemyGrid[_posY][_posX] = nullptr;

						_posX = playerX;
						_posY = playerY;
					}

					enemyXHolder = _posX;
					enemyYHolder = _posY;
					break;

				case 2: // player attack
					damageHolder = -1;
					enemyXHolder = _posX;
					enemyYHolder = _posY;

					levelGrid[_posY][_posX] = ' ';
					this->die();
					break;
				}
			}
		}
	}
	else {
		if (!_moveTowardsPlayers(playerX, playerY, levelGrid, enemyGrid)) {
			_moveRandomly(levelGrid, enemyGrid);
		}
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
                if (playerY < _posY) {
                    if (!_moveUp(levelGrid, enemyGrid)) {
                        _moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
                else if (playerY > _posY) {
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
                if (playerX < _posX) {
                    if (!_moveLeft(levelGrid, enemyGrid)) {
                        _moveVertically(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
                else if (playerX > _posX) {
                    if (!_moveRight(levelGrid, enemyGrid)) {
                        _moveVertically(playerX, playerY, levelGrid, enemyGrid);
                    }
                }
            }
		}
		else {
			if (!_moveVertically(playerX, playerY, levelGrid, enemyGrid)) {
				_moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
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

bool Enemy::_moveHorizontally(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    if (playerX < _posX) {
        return _moveLeft(levelGrid, enemyGrid);
    }
    else if (playerX > _posX) {
        return _moveRight(levelGrid, enemyGrid);
    }
}

bool Enemy::_moveVertically(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    if (playerY < _posY) {
		return _moveUp(levelGrid, enemyGrid);
    }
    else if (playerY > _posY) {
		return _moveDown(levelGrid, enemyGrid);
    }
}

bool Enemy::_moveUp(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY - 1][_posX] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY - 1][_posX] = _sign;

		enemyGrid[_posY - 1][_posX] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posY--;
		return true;
	}
	return false;
}

bool Enemy::_moveDown(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY + 1][_posX] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY + 1][_posX] = _sign;

		enemyGrid[_posY + 1][_posX] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posY++;
		return true;
	}
	return false;
}

bool Enemy::_moveLeft(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY][_posX - 1] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY][_posX - 1] = _sign;

		enemyGrid[_posY][_posX - 1] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posX--;
		return true;
	}
	return false;
}

bool Enemy::_moveRight(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY][_posX + 1] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY][_posX + 1] = _sign;

		enemyGrid[_posY][_posX + 1] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posX++;
		return true;
	}
	return false;
}

Snake::Snake(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_minDamage = 15;
	_maxDamage = 20;
	_moneyToGain = 20;
	_name = "snake";
	_type = EnemyType::SNAKE;
	_sign = 'S';
	_alive = true;
	_resting = false;
	_generateDamage = std::uniform_int_distribution<int>(_minDamage, _maxDamage);
}

Zombie::Zombie(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_minDamage = 10;
	_maxDamage = 15;
	_moneyToGain = 50;
	_name = "zombie";
	_type = EnemyType::ZOMBIE;
	_sign = 'Z';
	_alive = true;
	_resting = false;
	_generateDamage = std::uniform_int_distribution<int>(_minDamage, _maxDamage);
}

Witch::Witch(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_minDamage = 40;
	_maxDamage = 50;
	_moneyToGain = 120;
	_name = "witch";
	_type = EnemyType::WITCH;
	_sign = 'W';
	_alive = true;
	_resting = false;
	_generateDamage = std::uniform_int_distribution<int>(_minDamage, _maxDamage);
}

Monster::Monster(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_minDamage = 75;
	_maxDamage = 95;
	_moneyToGain = 200;
	_name = "monster";
	_type = EnemyType::MONSTER;
	_sign = 'M';
	_alive = true;
	_resting = false;
	_generateDamage = std::uniform_int_distribution<int>(_minDamage, _maxDamage);
}
