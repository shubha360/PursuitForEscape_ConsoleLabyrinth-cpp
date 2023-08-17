#include "../include/Player.h"

#include <iostream>

const int Player::PLAYER_HEALTH = 100;
const int Player::PLAYER_STARTING_MONEY = 100;

Player::Player(Level* level, Camera* camera) {
	_posX = level->getPlayerX();
	_posY = level->getPlayerY();

	_currentLevel = level;
	_currentLevel->setPlayer(_posX, _posY);

	_camera = camera;
	_camera->setCameraPosition(_posX, _posY);

	_currentHealth = _currentLevel->getPlayerHealth();
	_money = _currentLevel->getPlayerMoney();
	_artifactsCollected = _currentLevel->getArtifactsCollected();
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

	case 'j':
		_currentLevel->saveLevel(_posX, _posY, _currentHealth, _money, _artifactsCollected);
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

	stringstream artifacts;
	artifacts << "Artifacts Collected: " << _artifactsCollected << " / " << _currentLevel->getNumberOfArtifacts() << ' ';

	stringstream first; 
	first << "Health: " << _currentHealth; 
	first << string(artifacts.str().size() - first.str().size(), ' ') << "| WASD - Player movement\n";

	stringstream second;
	second << "Money: " << _money;
	second << string(artifacts.str().size() - second.str().size(), ' ') << "| J - Save game\n";

	stringstream third;
	third << artifacts.str() << "| M - Load game\n";

	cout << first.str();
	cout << second.str();
	cout << third.str();

	/*cout << "Health: " << _currentHealth << endl;
	cout << "Money: " << _money << endl;
	cout << "Artifacts Collected: " << _artifactsCollected << " / " << _currentLevel->getNumberOfArtifacts() << endl << endl;
	cout << "WASD - Player movement\n";
	cout << "J - Save game\n";
	cout << "M - Load game\n";*/
}