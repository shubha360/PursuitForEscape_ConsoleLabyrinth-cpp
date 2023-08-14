#include "../include/Player.h"

#include <iostream>

const int Player::PLAYER_HEALTH = 100;
const int Player::PLAYER_STARTING_MONEY = 100;

Player::Player(Level* level, Camera* camera) {
	_posX = 2;
	_posY = 2;

	_currentLevel = level;
	_currentLevel->setPlayer(_posX, _posY);

	_camera = camera;

	_currentHealth = 100;
	_money = 100;
	_artifactsCollected = 0;
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
	cout << "Health: " << _currentHealth << endl;
	cout << "Money: " << _money << endl;
	cout << "Artifacts Collected: " << _artifactsCollected << " / " << _currentLevel->getNumberOfArtifacts() << endl;
}