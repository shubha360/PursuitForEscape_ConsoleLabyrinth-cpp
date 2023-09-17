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

    _shields = -1;
    _zombieInfectionHealers = -1;
    _impairedMovementHealers = -1;

	_zombieInfectedMoves = 0;
	_zombieAttackedNow = false;
	_impairedMoves = 0;
	_artifactsOfMonster = 0;
}

Player::Player(Level* level, Camera* camera) {
	_currentLevel = level;
	_camera = camera;

    updatePlayerAfterGameStateChange();
}

bool Player::movePlayer(char input) {

	// to determine if the game ended
	bool continueGame = true;

	// any of w, a ,s, d was pressed
	bool playerTriedToMove = false;

	switch (input) {
	case 'w':
	case 's':
	case 'a':
	case 'd':

		if (_impairedMoves > 0) {
			_performAnImpairedMove();
		}
		else {
			if (input == 'w') {
				_moveUp();
			}
			else if (input == 's') {
				_moveDown();
			}
			else if (input == 'a') {
				_moveLeft();
			}
			else if (input == 'd') {
				_moveRight();
			}
		}

        if (_zombieInfectedMoves > 0) {
            continueGame = _performAZombieInfectedMove();
		}

		playerTriedToMove = true;
		break;

    case 'r': // use zombie infection healer
        if (_zombieInfectionHealers > 0) {

            if (_zombieInfectedMoves == 0) {
                _addLog("No zombie-infected moves left.");
            }
            else {
                _zombieInfectedMoves = 0;
                _zombieInfectionHealers--;
                _addLog("Used a healer to heal zombie infection.");
            }
        }
        else {
            _addLog("No zombie infection healer left.");
        }
        break;

    case 't': // use impaired movement healer

        if (_impairedMovementHealers > 0) {

            if (_impairedMoves == 0) {
                _addLog("No impaired moves left.");
            }
            else {
            _impairedMoves = 0;
            _impairedMovementHealers--;
            _addLog("Used a healer to heal impaired movement.");
            }
        }
        else {
            _addLog("No impaired movement healer left.");
        }
        break;

	case 'j': // save game

		_currentLevel->saveLevel(_posX, _posY, _currentHealth, _money, _artifactsCollected,
                           _shields, _zombieInfectionHealers, _impairedMovementHealers,
                           _zombieInfectedMoves, _impairedMoves, _artifactsOfMonster);

		_addLog("Game saved");
		break;

	case 'm': // load game
		if (!_currentLevel->loadLevel(Level::SAVE_FILE_LOCATION)) {
			_addLog("No load game exists");
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

	case 'k': // delete save game
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
			_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_MONSTER) { // bumped into an enemy

			Enemy* currentEnemy = _currentLevel->getEnemyGrid()[_posY][_posX];

			_combatEnemy(currentEnemy);

			if (_currentHealth <= 0) { // player died
				_currentLevel->erasePlayer(_oldX, _oldY);
				continueGame = false;
				_addLog("Player died!");
			}
		}
		else { // got something or escaped

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
            }

			// found random money
			else if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_RANDOM_MONEY) {
				static std::uniform_int_distribution<int> getRandomMoney(5, 50);
				int gotMoney = getRandomMoney(Enemy::RandomEngine);

				_money += gotMoney;
				_addLog("Found " + std::to_string(gotMoney) + " currency.");
			}

			// found 10 health
			else if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_10_HEALTH) {
				_increaseHealth(15);
				_addLog("Found 15 health.");
			}

			// found health refill
			else if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_REFILL_HEALTH) {
				_increaseHealth(100);
				_addLog("Found health refill.");
			}

			// found a shield
			else if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_SHIELD) {
				_shields++;
				_addLog("Found a shield. Total shields " + std::to_string(_shields) + ".");
			}

			// game is ended if player accessed the escape gate
			else if (_currentLevel->getTileAtGrid(_posX, _posY) == Level::SIGN_GATE_OPEN) {
                continueGame = false;
				_addLog("Escaped!");
            }

			_currentLevel->setPlayer(_posX, _posY, _oldX, _oldY);
			_camera->setCameraPosition(_posX, _posY);
		}
		_oldX = _posX;
		_oldY = _posY;
	}

	// player triggered a movement and if player did not die
	if (playerTriedToMove && continueGame) {

        continueGame = _moveEnemies();

        if (!continueGame) { // false means player died
            _addLog("Player died!");
        }
	}

	_camera->render(getPlayerInfo());

	return continueGame;
}

std::string Player::getPlayerInfo() {
	static int plInfoWidth = 35;

	std::string healthStr = " Health: " + std::to_string(_currentHealth);
	std::string moneyStr = " Money: " + std::to_string(_money);
	std::string artifactsStr = " Artifacts Collected: " + std::to_string(_artifactsCollected) + " / " + std::to_string(_currentLevel->getNumberOfArtifacts());
	std::string shieldsText = " Shields: " + std::to_string(_shields);
	std::string zombieInfHealersText = " Zombie Infection Healers: " + std::to_string(_zombieInfectionHealers);
	std::string impairedMoveHealersText = " Impaired Movement Healers: " + std::to_string(_impairedMovementHealers);
    std::string zombieInfText = " Zombie-infected Moves: " + std::to_string(_zombieInfectedMoves);
	std::string impairedMovesText = " Impaired Moves: " + std::to_string(_impairedMoves);
	std::string artifactsOfMonsterText = " Artifacts Hold By Monster: " + std::to_string(_artifactsOfMonster);

	std::string infoStr = healthStr + std::string(plInfoWidth - healthStr.size(), ' ') + "|> " + _playerLog[0] + "\n"
		+ moneyStr + std::string(plInfoWidth - moneyStr.size(), ' ') + "|> " + _playerLog[1] + "\n"
		+ artifactsStr + std::string(plInfoWidth - artifactsStr.size(), ' ') + "|> " + _playerLog[2] + "\n"
		+ std::string(plInfoWidth, ' ') + "|> " + _playerLog[3] + "\n"
		+ shieldsText + std::string(plInfoWidth - shieldsText.size(), ' ') + "|> " + _playerLog[4] + "\n"
		+ zombieInfHealersText + std::string(plInfoWidth - zombieInfHealersText.size(), ' ') + "|> " + _playerLog[5] + "\n"
		+ impairedMoveHealersText + std::string(plInfoWidth - impairedMoveHealersText.size(), ' ') + "|> " + _playerLog[6] + "\n"
		+ std::string(plInfoWidth, ' ') + "|> " + _playerLog[7] + "\n"
		+ zombieInfText + std::string(plInfoWidth - zombieInfText.size(), ' ') + "|> " + _playerLog[8] + "\n"
		+ impairedMovesText + std::string(plInfoWidth - impairedMovesText.size(), ' ') + "|> " + _playerLog[9] + "\n"
		+ artifactsOfMonsterText + std::string(plInfoWidth - artifactsOfMonsterText.size(), ' ') + "|> " + _playerLog[10] + "\n";

	return infoStr;
}

void Player::updatePlayerAfterGameStateChange() {
    _posX = _currentLevel->getPlayerX();
	_posY = _currentLevel->getPlayerY();
	_oldX = _currentLevel->getPlayerX();
	_oldY = _currentLevel->getPlayerY();

	_currentHealth = _currentLevel->getPlayerHealth();
	_money = _currentLevel->getPlayerMoney();
	_artifactsCollected = _currentLevel->getArtifactsCollected();

	_shields = _currentLevel->getShields();
	_zombieInfectionHealers = _currentLevel->getZombieInfectionHealers();
	_impairedMovementHealers = _currentLevel->getImpairedMoveHealers();

	_zombieInfectedMoves = _currentLevel->getZombieInfectedMoves();
	_zombieAttackedNow = false;

	_impairedMoves = _currentLevel->getImpairedMoves();
	_artifactsOfMonster = _currentLevel->getArtifactsHoldByMonster();

	_currentLevel->setPlayer(_posX, _posY);
	_camera->setCameraPosition(_posX, _posY);
}

void Player::_combatEnemy(Enemy* enemy) {
    if (enemy->isALive()) {
        static std::uniform_int_distribution<int> getAttacker(1, 2);

        //int attacker = getAttacker(Enemy::RandomEngine);
		int attacker = 2;

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

            // only set resting if an enemy defended and countered, otherwise enemy will attack immediately after countering
            enemy->setResting();

			_addLog(log);
            break;
        }
    }
}

// moves enemies after player moves, returns false if player died from an enemy attack
bool Player::_moveEnemies() {
	int enemyDamageArr[3]; // for getting damage dealt by a moving enemy
	Enemy* enemyArr[3] { nullptr,nullptr,nullptr }; // for getting the name of the enemy

	_currentLevel->moveEnemies(_posX, _posY, _currentHealth, _shields, enemyDamageArr, enemyArr);

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
    std::string output = "";

    if (_shields > 0) {
        _shields--;
        output = " Blocked using a shield. Shields remaining: " + std::to_string(_shields) + ".";
    }
    else {
        _currentHealth -= damage;

        std::string affects;

        switch (enemy->getType()) {
        case EnemyType::SNAKE:
            affects = ".";
            break;

        case EnemyType::ZOMBIE:
            _zombieInfectedMoves = 5;
            _zombieAttackedNow = true;
            affects = " and got 5 zombie-infected moves.";
            break;

        case EnemyType::WITCH:
			_impairedMoves += 3;
            affects = " and got 3 impaired moves.";
            break;

        case EnemyType::MONSTER:
			_artifactsOfMonster += _artifactsCollected;
			_artifactsCollected = 0;
            affects = " and took all the artifacts.";
            break;
        }
        output = " Lost " + std::to_string(damage) + " health" + affects;
    }

    // player did not move
    _posX = _oldX;
    _posY = _oldY;

	return output;
}

// processes enemy kill and returns log text
std::string Player::_processEnemyKill(Enemy* enemy) {

	enemy->die();
	_money += enemy->getMoney();

	std::string affects;

	switch (enemy->getType()) {

	case EnemyType::SNAKE:
		_increaseHealth(5);
		affects = " and 5 health.";
		break;

	case EnemyType::ZOMBIE:
		affects = ".";
		break;

	case EnemyType::WITCH:
		affects = ".";
		break;

	case EnemyType::MONSTER:
		_artifactsCollected += _artifactsOfMonster;
		affects = " and " + std::to_string(_artifactsOfMonster) + " artifacts.";
		_artifactsOfMonster = 0;
		break;
	}

	_currentLevel->setPlayer(_posX, _posY, _oldX, _oldY);
	_camera->setCameraPosition(_posX, _posY);

	return " Got " + std::to_string(enemy->getMoney()) + " currency" + affects;
}

void Player::_addLog(std::string logText) {
	for (int i = 9; i >= 0; i--) {
		_playerLog[i + 1] = _playerLog[i];
	}
	_playerLog[0] = logText;
}

void Player::_increaseHealth(int amountToAdd) {
	_currentHealth += amountToAdd;

	if (_currentHealth > 100) {
		_currentHealth = 100;
	}
}

// returns false if player died during the move
bool Player::_performAZombieInfectedMove() {
    bool playerAlive = true;
    // do not perform a zombie infected move if zombie attacked in the previous move
    if (_zombieAttackedNow) {
        _zombieAttackedNow = false;
    }
    else {
        static std::uniform_int_distribution<int> getZombieBiteDamage(2, 4);

        int damage = getZombieBiteDamage(Enemy::RandomEngine);
        _currentHealth -= damage;
        _zombieInfectedMoves--;

        _addLog("Lost " + std::to_string(damage) + " health due to zombie infection."
            + std::to_string(_zombieInfectedMoves) + " more zombie-infected move remains.");

        if (_currentHealth <= 0) { // player died
            _currentLevel->erasePlayer(_posX, _posY);
            _addLog("Player died!");
            playerAlive = false;
        }
    }
    return playerAlive;
}

void Player::_performAnImpairedMove() {
	static std::uniform_int_distribution<int> getMove(0, 4);

	int nextMove = getMove(Enemy::RandomEngine);
	_impairedMoves--;

	switch (nextMove) {

	case 0: // no move
		_addLog("Made no move due to impaired movement. " + std::to_string(_impairedMoves) + " more impaired move remains");
		break;

	case 1: // up
		_moveUp();
		_addLog("Moved up due to impaired movement. " + std::to_string(_impairedMoves) + " more impaired move remains");
		break;

	case 2: // down
		_moveDown();
		_addLog("Moved down due to impaired movement. " + std::to_string(_impairedMoves) + " more impaired move remains");
		break;

	case 3: // left
		_moveLeft();
		_addLog("Moved left due to impaired movement. " + std::to_string(_impairedMoves) + " more impaired move remains");
		break;

	case 4: // right
		_moveRight();
		_addLog("Moved right due to impaired movement. " + std::to_string(_impairedMoves) + " more impaired move remains");
		break;
	}
}

void Player::_moveUp() {
	if (_currentLevel->getTileAtGrid(_posX, _posY - 1) != Level::SIGN_WALL &&
		_currentLevel->getTileAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_WALL &&
		_currentLevel->getTileAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_OPEN) {

		_posY--;
	}
	else {
		_addLog("Moving up is blocked by something.");
	}
}

void Player::_moveDown() {
	if (_currentLevel->getTileAtGrid(_posX, _posY + 1) != Level::SIGN_WALL &&
		_currentLevel->getTileAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_WALL &&
		_currentLevel->getTileAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_LOCKED) {

		_posY++;
	}
	else {
		_addLog("Moving down is blocked by something.");
	}
}

void Player::_moveLeft() {
	if (_currentLevel->getTileAtGrid(_posX - 1, _posY) != Level::SIGN_WALL &&
		_currentLevel->getTileAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_WALL &&
		_currentLevel->getTileAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_LOCKED) {

		_posX--;
	}
	else {
		_addLog("Moving left is blocked by something.");
	}
}

void Player::_moveRight() {
	if (_currentLevel->getTileAtGrid(_posX + 1, _posY) != Level::SIGN_WALL &&
		_currentLevel->getTileAtGrid(_posX + 1, _posY) != Level::SIGN_GATE_WALL &&
		_currentLevel->getTileAtGrid(_posX + 1, _posY) != Level::SIGN_GATE_LOCKED) {

		_posX++;
	}
	else {
		_addLog("Moving right is blocked by something.");
	}
}
