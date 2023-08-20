#include "../include/Player.h"

#include <iostream>

const int Player::PLAYER_HEALTH = 100;
const int Player::PLAYER_STARTING_MONEY = 100;

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
	
	bool continueGame = true;

	int oldX = _posX;
	int oldY = _posY;

	switch (input) {
	case 'w':
		if (_currentLevel->getPositionAtGrid(_posX, _posY - 1) != Level::SIGN_WALL && 
			_currentLevel->getPositionAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_WALL && 
			_currentLevel->getPositionAtGrid(_posX, _posY - 1) != Level::SIGN_GATE_OPEN) {
			
			_posY--;
		}
		break;

	case 's':
		if (_currentLevel->getPositionAtGrid(_posX, _posY + 1) != Level::SIGN_WALL &&
			_currentLevel->getPositionAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_WALL &&
			_currentLevel->getPositionAtGrid(_posX, _posY + 1) != Level::SIGN_GATE_LOCKED) {
			
			_posY++;
		}
		break;

	case 'a':
		if (_currentLevel->getPositionAtGrid(_posX - 1, _posY) != Level::SIGN_WALL &&
			_currentLevel->getPositionAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_WALL &&
			_currentLevel->getPositionAtGrid(_posX - 1, _posY) != Level::SIGN_GATE_LOCKED) {
			
			_posX--;
		}
		break;

	case 'd':
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
			cout << "No load game exists!\n\n";
		}
		else {
			updatePlayerAfterGameStateChange();
			oldX = _posX;
			oldY = _posY;
		}
		break;

	case 'n': // new game
		_currentLevel->loadLevel(_currentLevel->getLevelFileLocation());
		updatePlayerAfterGameStateChange();
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

	if (_posX != oldX || _posY != oldY) {
		
		if (_currentLevel->getPositionAtGrid(_posX, _posY) == Level::SIGN_ARTIFACT) {
			_artifactsCollected++;

			if (_artifactsCollected == _currentLevel->getNumberOfArtifacts()) {
				_currentLevel->openEscapeGate();
			}
		}

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

	string infoStr = "Health: " + to_string(_currentHealth)
		+ "                | WASD - Player movement | N - New game\n"
		+ "Money: " + to_string(_money)
		+ "                 | J - Save game          | L - Delete Save Game\n"
		+ "Artifacts Collected: " + to_string(_artifactsCollected) + " / " + to_string(_currentLevel->getNumberOfArtifacts())
		+ " | M - Load game          | Esc - Exit game\n\n";

	cout << infoStr;
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