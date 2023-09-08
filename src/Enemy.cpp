#include "../include/Enemy.h"

std::random_device Enemy::_seed;
std::mt19937 Enemy::RandomEngine(_seed());

void Enemy::move(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	int disX = _posX - playerX;
	int disY = _posY - playerY;
	int absDisX = abs(disX);
	int absDisY = abs(disY);
	int distance = absDisX + absDisY;

	if (distance <= 5) {
		if (absDisY < absDisX) {
			if (disY > 0) {

			}
		}
	}
	else {
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
}

void Enemy::_moveUp(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {

	if (levelGrid[_posY - 1][_posX] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posY--;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY + 1][_posX];
		enemyGrid[_posY + 1][_posX] = nullptr;
	}
}

void Enemy::_moveDown(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY + 1][_posX] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posY++;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY - 1][_posX];
		enemyGrid[_posY - 1][_posX] = nullptr;
	}
}

void Enemy::_moveLeft(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY][_posX - 1] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posX--;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY][_posX + 1];
		enemyGrid[_posY][_posX + 1] = nullptr;
	}
}

void Enemy::_moveRight(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid) {
	if (levelGrid[_posY][_posX + 1] == ' ') {
		levelGrid[_posY][_posX] = ' ';
		_posX++;
		levelGrid[_posY][_posX] = _sign;

		enemyGrid[_posY][_posX] = enemyGrid[_posY][_posX - 1];
		enemyGrid[_posY][_posX - 1] = nullptr;
	}
}

Snake::Snake(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 1;
	_damage = 10;
	_moneyToGain = 20;
	_type = EnemyType::SNAKE;
	_sign = 'S';
}

Zombie::Zombie(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 3;
	_damage = 5;
	_moneyToGain = 50;
	_type = EnemyType::ZOMBIE;
	_sign = 'Z';
}

Witch::Witch(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 2;
	_damage = 30;
	_moneyToGain = 120;
	_type = EnemyType::WITCH;
	_sign = 'W';
}

Monster::Monster(int posX, int posY) {
	_posX = posX;
	_posY = posY;
	_strikesNeeded = 5;
	_damage = 45;
	_moneyToGain = 200;
	_type = EnemyType::MONSTER;
	_sign = 'M';
}