#pragma once

#include <string>
#include <vector>

using namespace std;

class Level {
public:
	static const char SIGN_EMPTY;
	static const char SIGN_WALL;
	static const char SIGN_GATE_WALL;
	static const char SIGN_GATE_LOCKED;
	static const char SIGN_GATE_OPEN;
	static const char SIGN_PLAYER;
	static const char SIGN_ARTIFACT;

	Level();
	Level(string levelName, string fileLocation);
	bool loadLevel();
	void printLevel();
	char getPositionAtGrid(int x, int y);
	void setPlayer(int newX, int newY);
	void setPlayer(int newX, int newY, int oldX, int oldY);
	void openEscapeGate();

	int getRows() { return _rows; }
	int getColumns() { return _columns; }
	int getNumberOfArtifacts(){ return _numberOfArtifacts; }
	int getEscapeX() { return _escapeX; }
	int getEscapeY() { return _escapeY; }

private:
	string _levelName;
	string _levelFileLocation;
	vector<string> _levelGrid;

	int _rows, _columns;
	int _numberOfArtifacts;
	bool _levelLoaded;
	int _escapeX, _escapeY;
};

