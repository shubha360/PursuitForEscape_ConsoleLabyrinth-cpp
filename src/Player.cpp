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

	// store the old player coordinates before moving
	int oldX = _posX;
	int oldY = _posY;

	switch (input) {
	case 'w': // move up
		if (_currentLevel->getPositionAtGrid(_posX, _posY - 1) != Level::SIGN_WALL && 
			_currentLevel->getPositionAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_WALL && 
			_currentLevel->getPositionAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_OPEN) {
			
			_posY--;
		}
		break;

	case 's': // move up
		if (_currentLevel->getPositionAtGrid(_posX, _posY + 1) != Level::SIGN_WALL &&
			_currentLevel->getPositionAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_WALL &&
			_currentLevel->getPositionAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_LOCKED) {
			
			_posY++;
		}
		break;

	case 'a': // move up
		if (_currentLevel->getPositionAtGrid(_posX - 1, _posY) != Level::SIGN_WALL &&
			_currentLevel->getPositionAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_WALL &&
			_currentLevel->getPositionAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_LOCKED) {
			
			_posX--;
		}
		break;

	case 'd': // move up
		if (_currentLevel->getPositionAtGrid(_posX + 1, _posY) != Level::SIGN_WALL &&
			_currentLevel->getPositionAtGrid(_posX + 1, _posY) != Level::SIGN_GATE_WALL &&
			_currentLevel->getPositionAtGrid(_posX + 1, _posY) != Level::SIGN_GATE_LOCKED) {
			
			_posX++;
		}
		break;

	case 'j': // save game

		_currentLevel->saveLevel(_posX, _posY, _currentHealth, _money, _artifactsCollected);
		break;

	case 'm': // load game
		if (!_currentLevel->loadLevel(Level::SAVE_FILE_LOCATION)) {
			std::cout << "No load game exists!\n\n";
		}
		else {
			updatePlayerAfterGameStateChange();

			// prevent triggering player moving functionality
			oldX = _posX;
			oldY = _posY;
		}
		break;

	case 'n': // new game
		_currentLevel->loadLevel(_currentLevel->getLevelFileLocation());
		updatePlayerAfterGameStateChange();

		// prevent triggering player moving functionality
		oldX = _posX;
		oldY = _posY;
		break;

	case 'l': // delete save game
		_currentLevel->deleteSaveGame();
		break;

	case 27: // exit game
		continueGame = false;
		break;
	}

	// different old and new coordinates mean player moved
	if (_posX != oldX || _posY != oldY) {
		
		// collected an artifact
		if (_currentLevel->getPositionAtGrid(_posX, _posY) == Level::SIGN_ARTIFACT) {
			_artifactsCollected++;

			// open escape gate if all artifacts are collected
			if (_artifactsCollected == _currentLevel->getNumberOfArtifacts()) {
				_currentLevel->openEscapeGate();
			}
		}

		// game is ended if player accessed the escape gate
		if (_currentLevel->getPositionAtGrid(_posX, _posY) == Level::SIGN_GATE_OPEN) {
			continueGame = false;
		}

		_currentLevel->setPlayer(_posX, _posY, oldX, oldY);
		_camera->setCameraPosition(_posX, _posY);

		_camera->render();
		printPlayerInfo();
	}
	return continueGame;
}

void Player::printPlayerInfo() {

	std::string infoStr = "Health: " + std::to_string(_currentHealth)
		+ "                | WASD - Player movement | N - New game\n"
		+ "Money: " + std::to_string(_money)
		+ "                 | J - Save game          | L - Delete Save Game\n"
		+ "Artifacts Collected: " + std::to_string(_artifactsCollected) + " / " + std::to_string(_currentLevel->getNumberOfArtifacts())
		+ " | M - Load game          | Esc - Exit game\n\n";

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