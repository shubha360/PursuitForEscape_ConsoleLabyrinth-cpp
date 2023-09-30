#include "../include/Enemy.h"

std::random_device Enemy::_seed;
std::mt19937 Enemy::RandomEngine(_seed());

// randomly generate damage in range
int Enemy::getDamage() {
	return _generateDamage(RandomEngine);
}

// damageHolder holds damage dealt by an enemy while moving, always pass a integer reference with value 0 to get the damage
// if enemy attacked damageHolder will be the damage value, if player attacked damageHolder will be -1
// enemyXHolder and enemyYHolder hold the coordinates of the enemy, pass integer references
void Enemy::move(const int& playerX, const int& playerY, const int& playerHealth, int& damageHolder, int& enemyXHolder, int& enemyYHolder, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

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

				// randomly generate who will attack
				int const attacker = getAttacker(RandomEngine);
				//int attacker = 1;

				switch (attacker) {
				case 1: // enemy attack

					// get random damage
					damageHolder = _generateDamage(RandomEngine);

					if (playerHealth - damageHolder <= 0) { // if player died, set this enemy's position to player's position
						
						// update level grid, set enemy position to player position
						levelGrid[playerY][playerX] = _sign;
						levelGrid[_posY][_posX] = ' ';

						// update enemy grid, set previous position to null
						enemyGrid[playerY][playerX] = enemyGrid[_posY][_posX];
						enemyGrid[_posY][_posX] = nullptr;

						_posX = playerX;
						_posY = playerY;
					}

					enemyXHolder = _posX;
					enemyYHolder = _posY;
					break;

				case 2: // player attack
					damageHolder = -1; // -1 for player attack
					enemyXHolder = _posX;
					enemyYHolder = _posY;

					levelGrid[_posY][_posX] = ' ';
					break;
				}
			}
		}
	}
	else { // moves towards player or move randomly
		if (!_moveTowardsPlayers(playerX, playerY, levelGrid, enemyGrid)) {
			_moveRandomly(levelGrid, enemyGrid);
		}
	}
}

bool Enemy::_moveTowardsPlayers(const int& playerX, const int& playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	
	int absDisX = abs(_posX - playerX); // absolute horizontal distance
	int absDisY = abs(_posY - playerY); // absolute vertical distance
	int distance = absDisX + absDisY; // total distance

	// moves towards player if distance not more than 5
	if (distance <= 5) { 
		
		// moves vertically, if vertical distance is less than horizontal distance
		if (absDisY < absDisX) {
            
			// if vertical distance is zero, move horizontally
			if (absDisY == 0) { 
                _moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
            }
            else {

				// tries to move vertically
				if (!_moveVertically(playerX, playerY, levelGrid, enemyGrid)) {

					// moves horizontally if vertical movement is blocked
					_moveHorizontally(playerX, playerY, levelGrid, enemyGrid);
				}
            }
		}
		// moves horizontally, if horizontal distance is less than vertical distance
		else if (absDisX < absDisY) { 
			
			// if horizontal distance is zero, move vertically// if horizontal distance is zero, move vertically
			if (absDisX == 0) { 
                _moveVertically(playerX, playerY, levelGrid, enemyGrid);
            }
            else {

				// tries to move horizontally
				if (!_moveHorizontally(playerX, playerY, levelGrid, enemyGrid)) {

					// moves vertically if horizontal movement is blocked
					_moveVertically(playerX, playerY, levelGrid, enemyGrid);
				}
            }
		}

		// if both horizontal and vertical distances are same
		else { 

			// tries to move vertically
			if (!_moveVertically(playerX, playerY, levelGrid, enemyGrid)) {

				// moves horizontally if vertical movement is blocked
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

bool Enemy::_moveVertically(const int& playerX, const int& playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	// player is above
	if (playerY < _posY) {
		return _moveUp(levelGrid, enemyGrid);
	}

	// player is below
	else if (playerY > _posY) {
		return _moveDown(levelGrid, enemyGrid);
	}
}

bool Enemy::_moveHorizontally(const int& playerX, const int& playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
    
	// player is on the left
	if (playerX < _posX) {
        return _moveLeft(levelGrid, enemyGrid);
    }
	
	// player is on the right
    else if (playerX > _posX) {
        return _moveRight(levelGrid, enemyGrid);
    }
}

bool Enemy::_moveUp(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	// if desired spot is vacant
	if (levelGrid[_posY - 1][_posX] == ' ') {

		// update level grid
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY - 1][_posX] = _sign;

		// update enemy grid
		enemyGrid[_posY - 1][_posX] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posY--;
		return true;
	}
	return false;
}

bool Enemy::_moveDown(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	// if desired spot is vacant
	if (levelGrid[_posY + 1][_posX] == ' ') {
		
		// update level grid
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY + 1][_posX] = _sign;

		// update enemy grid
		enemyGrid[_posY + 1][_posX] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posY++;
		return true;
	}
	return false;
}

bool Enemy::_moveLeft(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	// if desired spot is vacant
	if (levelGrid[_posY][_posX - 1] == ' ') {

		// update level grid
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY][_posX - 1] = _sign;

		// update enemy grid
		enemyGrid[_posY][_posX - 1] = enemyGrid[_posY][_posX];
		enemyGrid[_posY][_posX] = nullptr;

		_posX--;
		return true;
	}
	return false;
}

bool Enemy::_moveRight(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	// if desired spot is vacant
	if (levelGrid[_posY][_posX + 1] == ' ') {

		// update level grid
		levelGrid[_posY][_posX] = ' ';
		levelGrid[_posY][_posX + 1] = _sign;

		// update enemy grid
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
	_moneyToGain = 30;
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
	_moneyToGain = 60;
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
	_moneyToGain = 100;
	_name = "monster";
	_type = EnemyType::MONSTER;
	_sign = 'M';
	_alive = true;
	_resting = false;
	_generateDamage = std::uniform_int_distribution<int>(_minDamage, _maxDamage);
}
