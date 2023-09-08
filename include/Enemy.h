#pragma once

#include <iostream>
#include <vector>
#include <random>

enum class EnemyType { SNAKE, ZOMBIE, WITCH, MONSTER };

class Enemy {
public:
	static std::mt19937 RandomEngine;

	void takeDamage() { _strikesNeeded--; };
	void move(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);

	int getStrikesNeeded() { return _strikesNeeded; }
	int getDamage() { return _damage; }
	int getMoney() { return _moneyToGain; }

protected:
	int _posX, _posY;

	int _strikesNeeded;
	int _damage;
	int _moneyToGain;

	EnemyType _type;
	char _sign;

	bool _died;

private:
	static std::random_device _seed;

	void _moveUp(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
	void _moveDown(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
	void _moveLeft(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
	void _moveRight(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
};

class Snake : public Enemy {
public:
	Snake(int posX, int posY);
};

class Zombie : public Enemy {
public:
	Zombie(int posX, int posY);
};

class Witch : public Enemy {
public:
	Witch(int posX, int posY);
};

class Monster : public Enemy {
public:
	Monster(int posX, int posY);
};