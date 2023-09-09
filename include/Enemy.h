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
	bool isALive() { return _alive; }

	void die() { _alive = false; }

protected:
	int _posX, _posY;

	int _strikesNeeded;
	int _damage;
	int _moneyToGain;

	EnemyType _type;
	char _sign;

	bool _alive;

private:
	static std::random_device _seed;

    bool _moveTowardsPlayers(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
    void _moveRandomly(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);

    bool _moveHorizontally(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid); // move horizontally towards player
    bool _moveVertically(int playerX, int playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid); // move vertically towards player

	bool _moveUp(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
	bool _moveDown(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
	bool _moveLeft(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
	bool _moveRight(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
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
