#include "../include/Player.h"

const int Player::PLAYER_HEALTH = 100;
const int Player::PLAYER_STARTING_MONEY = 100;

Player::Player() {
	_currentLevel = nullptr;
	_camera = nullptr;

	_posX = -1;
	_posY = -1;
	_oldX = -1;
	_oldY = -1;

	_currentHealth = -1;
	_money = -1;
	_artifactsCollected = -1;
}

Player::Player(Level* level, Camera* camera) {
	_currentLevel = level;
	_camera = camera;

	_posX = _currentLevel->getPlayerX();
	_posY = _currentLevel->getPlayerY();
	_oldX = _currentLevel->getPlayerX();
	_oldY = _currentLevel->getPlayerY();

	_currentHealth = _currentLevel->getPlayerHealth();
	_money = _currentLevel->getPlayerMoney();
	_artifactsCollected = _currentLevel->getArtifactsCollected();

	_currentLevel->setPlayer(_posX, _posY);
	_camera->setCameraPosition(_posX, _posY);
}

bool Player::movePlayer(char input) {

	// to determine if the game ended
	bool continueGame = true;

	// to trigger enemy movement
	bool moveEnemies = false;

	// store the old player coordinates before moving

	switch (input) {
	case 'w': // move up
		if (_currentLevel->getTileAtGrid(_posX, _posY - 1) != Level::SIGN_WALL &&
			_currentLevel->getTileAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_WALL &&
			_currentLevel->getTileAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_OPEN) {

			_posY--;
		}
		moveEnemies = true;
		break;

	case 's': // move down
		if (_currentLevel->getTileAtGrid(_posX, _posY + 1) != Level::SIGN_WALL &&
			_currentLevel->getTileAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_WALL &&
			_currentLevel->getTileAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_LOCKED) {

			_posY++;
		}
		moveEnemies = true;
		break;

	case 'a': // move left
		if (_currentLevel->getTileAtGrid(_posX - 1, _posY) != Level::SIGN_WALL &&
			_currentLevel->getTileAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_WALL &&
			_currentLevel->getTileAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_LOCKED) {

			_posX--;
		}
		moveEnemies = true;
		break;

	case 'd': // move right
		if (_currentLevel->getTileAtGrid(_posX + 1, _posY) != Level::SIGN_WALL &&
			_currentLevel->getTileAtGrid(_posX + 1, _posY) != Level::SIGN_GATE_WALL &&
			_currentLevel->getTileAtGrid(_posX + 1, _posY) != Level::SIGN_GATE_LOCKED) {

			_posX++;
		}
		moveEnemies = true;
		break;

	case 'j': // save game

		_currentLevel->saveLevel(_posX, _posY, _currentHealth, _money, _artifactsCollected);
		_addLog("Game saved");
		break;

	case 'm': // load game
		if (!_currentLevel->loadLevel(Level::SAVE_FILE_LOCATION)) {
			_addLog("No load game exists");
			// std::cout << "No load game exists!\n\n";
		}
		else {
			updatePlayerAfterGameStateChange();

			// prevent triggering player moving functionality
			_oldX = _posX;
			_oldY = _posY;
			_addLog("Game loaded");
		}
		break;

	case 'n': // new game
		_currentLevel->loadLevel(_currentLevel->getLevelFileLocation());
		updatePlayerAfterGameStateChange();

		// prevent triggering player moving functionality
		_oldX = _posX;
		_oldY = _posY;
		_addLog("Started new game");
		break;

	case 'l': // delete save game
		_currentLevel->deleteSaveGame();
		_addLog("Save game deleted");
		break;

	case 27: // exit game
		continueGame = false;
		_addLog("Game exited");
		break;
	}

	// different old and new coordinates mean player moved
	if (_posX != _oldX || _posY != _oldY) {

		if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_SNAKE ||
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_ZOMBIE ||
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_WITCH ||
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_MONSTER) {

			Enemy* currentEnemy = _currentLevel->getEnemyGrid()[_posY][_posX];

			_combatEnemy(currentEnemy);

			if (_currentHealth <= 0) { // player died
				_currentLevel->erasePlayer(_oldX, _oldY);
				continueGame = false;
				_addLog("Player died!");
			}
		}
		else {
            // collected an artifact
            if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_ARTIFACT) {
                _artifactsCollected++;
				_addLog("Artifact collected. Artifacts remaining: " + std::to_string(_currentLevel->getNumberOfArtifacts() - _artifactsCollected));

                // open escape gate if all artifacts are collected
                if (_artifactsCollected == _currentLevel->getNumberOfArtifacts()) {
                    _currentLevel->openEscapeGate();
					_addLog("All artifacts collected");
					_addLog("Escape gate opened");
                }

                _currentLevel->setPlayer(_posX, _posY, _oldX, _oldY);
                _camera->setCameraPosition(_posX, _posY);
            }

            // game is ended if player accessed the escape gate
            if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_GATE_OPEN) {
                continueGame = false;
				_addLog("Escaped!");
            }

			_currentLevel->setPlayer(_posX, _posY, _oldX, _oldY);
			_camera->setCameraPosition(_posX, _posY);
		}
		_oldX = _posX;
		_oldY = _posY;
	}

	// move enemies
	if (moveEnemies) {
		if (!_moveEnemies()) { // false means player died
			continueGame = false;
			_addLog("Player died!");
		}
	}

	_camera->render(getPlayerInfo());

	return continueGame;
}

std::string Player::getPlayerInfo() {
	static int plInfoWidth = 30;

	std::string healthStr = " Health: " + std::to_string(_currentHealth);
	std::string moneyStr = " Money: " + std::to_string(_money);
	std::string artifactsStr = " Artifacts Collected: " + std::to_string(_artifactsCollected) + " / " + std::to_string(_currentLevel->getNumberOfArtifacts());

	std::string infoStr = healthStr + std::string(plInfoWidth - healthStr.size(), ' ') + "|> " + _playerLog[0] + "\n"
		+ moneyStr + std::string(plInfoWidth - moneyStr.size(), ' ') + "|> " + _playerLog[1] + "\n"
		+ artifactsStr + std::string(plInfoWidth - artifactsStr.size(), ' ') + "|> " + _playerLog[2] + "\n"
		+ std::string(plInfoWidth, ' ') + "|> " + _playerLog[3] + "\n";

	return infoStr;
}

void Player::updatePlayerAfterGameStateChange() {
	_posX = _currentLevel->getPlayerX();
	_posY = _currentLevel->getPlayerY();

	_currentHealth = _currentLevel->getPlayerHealth();
	_money = _currentLevel->getPlayerMoney();
	_artifactsCollected = _currentLevel->getArtifactsCollected();

	_currentLevel->setPlayer(_posX, _posY);
	_camera->setCameraPosition(_posX, _posY);

	_camera->render(getPlayerInfo());
}

void Player::_combatEnemy(Enemy* enemy) {
    if (enemy->isALive()) {
        static std::uniform_int_distribution<int> getAttacker(1, 2);

        int attacker = getAttacker(Enemy::RandomEngine);
		//int attacker = 2;

		std::string log;

        switch (attacker) {
        case 1: // player attack

			log = "Attacked and killed a " + enemy->getName() + ".";
			log += _processEnemyKill(enemy);
			_addLog(log);
            break;

        case 2: // enemy attack
			int damage = enemy->getDamage();

			std::string log = "The " + enemy->getName() + " defended and countered.";
			log += _processAttackFromEnemy(enemy, damage);

			_addLog(log);
            break;
        }
    }
}

// moves enemies after player moves, returns false if player died from an enemy attack
bool Player::_moveEnemies() {
	int enemyDamageArr[3]; // for getting damage dealt by a moving enemy
	Enemy* enemyArr[3] { nullptr,nullptr,nullptr }; // for getting the name of the enemy

	_currentLevel->moveEnemies(_posX, _posY, _currentHealth, enemyDamageArr, enemyArr);

	for (int i = 0; i < 3; i++) {
		if (enemyDamageArr[i] > 0) { // enemy attacked the player

			std::string log = "A " + enemyArr[i]->getName() + " attacked.";
			std::string logFromEnemyAttack = _processAttackFromEnemy(enemyArr[i], enemyDamageArr[i]);
			_addLog(log + logFromEnemyAttack);
		}
		else if (enemyDamageArr[i] == -1) {

			std::string log = "Defended and killed a " + enemyArr[i]->getName() + ".";
			log += _processEnemyKill(enemyArr[i]);
			_addLog(log);
		}
	}

	if (_currentHealth <= 0) { // player died
		return false;
	}
	return true;
}

// processes attack from enemy and returns log text
std::string Player::_processAttackFromEnemy(Enemy* enemy, int damage) {

	_currentHealth -= damage;

	std::string affects;

	switch (enemy->getType()) {
	case EnemyType::SNAKE:
		affects = ".";
		break;

	case EnemyType::ZOMBIE:
		affects = " and got 5 zombie-bitten move.";
		break;

	case EnemyType::WITCH:
		affects = " and took all the power ups for good.";
		break;

	case EnemyType::MONSTER:
		affects = " and took all the artifacts.";
		break;
	}

	// player did not move
	_posX = _oldX;
	_posY = _oldY;

	enemy->setResting();

	return " Lost " + std::to_string(damage) + " health" + affects;
}

// processes enemy kill and returns log text
std::string Player::_processEnemyKill(Enemy* enemy) {

	enemy->die();
	_money += enemy->getMoney();

	std::string affects;

	switch (enemy->getType()) {

	case EnemyType::SNAKE:

		_currentHealth += 5;

		if (_currentHealth > 100) {
			_currentHealth = 100;
		}

		affects = " and 5 health.";
		break;

	case EnemyType::ZOMBIE:
		affects = ".";
		break;

	case EnemyType::WITCH:
		affects = ".";
		break;

	case EnemyType::MONSTER:
		affects = " and the artifacts back.";
		break;
	}

	_currentLevel->setPlayer(_posX, _posY, _oldX, _oldY);
	_camera->setCameraPosition(_posX, _posY);

	return " Got " + std::to_string(enemy->getMoney()) + " currency" + affects;
}

void Player::_addLog(std::string logText) {
	for (int i = 2; i >= 0; i--) {
		_playerLog[i + 1] = _playerLog[i];
	}
	_playerLog[0] = logText;
}
