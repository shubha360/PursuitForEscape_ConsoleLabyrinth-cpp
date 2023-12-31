#pragma once

#include <iostream>
#include <vector>
#include <random>

enum class EnemyType { SNAKE, ZOMBIE, WITCH, MONSTER };

class Enemy {
public:
	static std::mt19937 RandomEngine;

	// damageHolder holds damage dealt by an enemy while moving, always pass a integer reference with value 0 to get the damage
	// if enemy attacked damageHolder will be the damage value, if player attacked damageHolder will be -1
	// enemyXHolder and enemyYHolder hold the coordinates of the enemy, pass integer references
	void move(const int& playerX, const int& playerY, const int& playerHealth,
		int& damageHolder, int& enemyXHolder, int& enemyYHolder, 
		std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);

	int getDamage(); // randomly generate damage in range
	int getMoney() { return _moneyToGain; }
	std::string getName() { return _name; }
	EnemyType getType() { return _type; }
	bool isALive() { return _alive; }

	void die() { _alive = false; }
	void setResting() { _resting = true; }

protected:
	int _posX, _posY;

	int _minDamage;
	int _maxDamage;
	int _moneyToGain;

	std::string _name;
	EnemyType _type;
	char _sign;

	bool _alive;
	bool _resting; // to determine if this enemy was in a fight in the previous move of player

	std::uniform_int_distribution<int> _generateDamage;

private:
	static std::random_device _seed;

    bool _moveTowardsPlayers(const int& playerX, const int& playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);
    void _moveRandomly(std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);

	// move vertically towards player
	bool _moveVertically(const int& playerX, const int& playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);

	// move horizontally towards player
    bool _moveHorizontally(const int& playerX, const int& playerY, std::vector<std::string>& levelGrid, std::vector<std::vector<Enemy*>>& enemyGrid);

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
