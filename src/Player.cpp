#include "../include/Player.h"

const int Player::PLAYER_HEALTH = 100;
const int Player::PLAYER_STARTING_MONEY = 100;

Player::Player() {
	_currentLevel = nullptr;
	_camera = nullptr;

	_posX = -1;
	_posY = -1;

	_currentHealth = -1;
	_money = -1;
	_artifactsCollected = -1;
}

Player::Player(Level* level, Camera* camera) {
	_currentLevel = level;
	_camera = camera;

	_posX = _currentLevel->getPlayerX();
	_posY = _currentLevel->getPlayerY();

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
	int oldX = _posX;
	int oldY = _posY;

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
			oldX = _posX;
			oldY = _posY;
			_addLog("Game loaded");
		}
		break;

	case 'n': // new game
		_currentLevel->loadLevel(_currentLevel->getLevelFileLocation());
		updatePlayerAfterGameStateChange();

		// prevent triggering player moving functionality
		oldX = _posX;
		oldY = _posY;
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
	if (_posX != oldX || _posY != oldY) {

		if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_SNAKE ||
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_ZOMBIE ||
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_WITCH ||
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_MONSTER) {

            Enemy* currentEnemy = _currentLevel->getEnemyGrid()[_posY][_posX];

			_combatEnemy(currentEnemy);

			if (!currentEnemy->isALive()) { // enemy died
				_currentLevel->setPlayer(_posX, _posY, oldX, oldY);
				_camera->setCameraPosition(_posX, _posY);
			}
			else { // player took damage
				
				// player did not move
				_posX = oldX;
				_posY = oldY;
				
				if (_currentHealth <= 0) { // player died
					_currentLevel->erasePlayer(oldX, oldY);
					continueGame = false;
					_addLog("Player died!");
				}
				
				currentEnemy->setResting();
			}
		}
		else {
            // collected an artifact
            if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_ARTIFACT) {
                _artifactsCollected++;
				_addLog("Artifact collected");

                // open escape gate if all artifacts are collected
                if (_artifactsCollected == _currentLevel->getNumberOfArtifacts()) {
                    _currentLevel->openEscapeGate();
					_addLog("All artifacts collected");
					_addLog("Escape gate opened");
                }

                _currentLevel->setPlayer(_posX, _posY, oldX, oldY);
                _camera->setCameraPosition(_posX, _posY);
            }

            // game is ended if player accessed the escape gate
            if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_GATE_OPEN) {
                continueGame = false;
				_addLog("Escaped!");
            }

			_currentLevel->setPlayer(_posX, _posY, oldX, oldY);
			_camera->setCameraPosition(_posX, _posY);
		}
	}

	// move enemies
	if (moveEnemies) {
		int tempDamage = 0; // for getting damage dealt by a moving enemy
		std::string tempName = ""; // for getting the name of the enemy

		_currentLevel->moveEnemies(_posX, _posY, _currentHealth, tempDamage, tempName);

		if (tempDamage > 0) { // enemy attacked the player
			_currentHealth -= tempDamage;
			_addLog("Taken " + std::to_string(tempDamage) + " damage from " + tempName);

			if (_currentHealth <= 0) { // player died
				continueGame = false;
				_addLog("Player died!");
			}
		}
	}

	_camera->render();
	printPlayerInfo();

	return continueGame;
}

void Player::printPlayerInfo() {
	static int logLineWidth = 40;

	std::string infoStr = "> " + _playerLog[0] + std::string(logLineWidth - _playerLog[0].size(), ' ') + " | Health: " + std::to_string(_currentHealth) + "\n"
		+ "> " + _playerLog[1] + std::string(logLineWidth - _playerLog[1].size(), ' ') + " | Money: " + std::to_string(_money) + "\n"
		+ "> " + _playerLog[2] + std::string(logLineWidth - _playerLog[2].size(), ' ') + " | Artifacts Collected: " + std::to_string(_artifactsCollected) + " / " + std::to_string(_currentLevel->getNumberOfArtifacts()) + "\n"
		+ "> " + _playerLog[3] + std::string(logLineWidth - _playerLog[3].size(), ' ') + " |\n";

	std::cout << infoStr;
}

void Player::updatePlayerAfterGameStateChange() {
	_posX = _currentLevel->getPlayerX();
	_posY = _currentLevel->getPlayerY();

	_currentHealth = _currentLevel->getPlayerHealth();
	_money = _currentLevel->getPlayerMoney();
	_artifactsCollected = _currentLevel->getArtifactsCollected();

	_currentLevel->setPlayer(_posX, _posY);
	_camera->setCameraPosition(_posX, _posY);

	_camera->render();
	printPlayerInfo();
}

void Player::_combatEnemy(Enemy* enemy) {
    if (enemy->isALive()) {
        static std::uniform_int_distribution<int> getAttacker(1, 2);

        //int attacker = getAttacker(Enemy::RandomEngine);
		int attacker = 2;

        switch (attacker) {
        case 1: // player attack
			enemy->die();
			_money += enemy->getMoney();
			_addLog("Attacked a " + enemy->getName() + ". Got " + std::to_string(enemy->getMoney()) + " currency.");
            break;

        case 2: // enemy attack
            _currentHealth -= enemy->getDamage();
			_addLog("Taken " + std::to_string(enemy->getDamage()) + " damage from " + enemy->getName());
            break;
        }
    }
}

//std::string Player::_processAttackFromEnemy(Enemy* enemy) {
//	std::string enemyName;
//
//	switch (enemy->getType()) {
//	case EnemyType::SNAKE:
//		enemyName = "snake";
//		break;
//
//	case EnemyType::ZOMBIE:
//		enemyName = "zombie";
//		break;
//	
//	case EnemyType::WITCH:
//		enemyName = "witch";
//		break;
//
//	case EnemyType::MONSTER:
//		enemyName = "monster";
//		break;
//	}
//	return enemyName;
//}

void Player::_addLog(std::string logText) {
	for (int i = 2; i >= 0; i--) {
		_playerLog[i + 1] = _playerLog[i];
	}
	_playerLog[0] = logText;
}