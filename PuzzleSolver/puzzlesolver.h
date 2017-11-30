#pragma once
#include "common.h"

class Coordinate {
public:
	int x, y;
	char content;

	Coordinate(int, int, char);
	void Print();
};

class Tile {
public:
	vector<Coordinate> blocks;
	bool reflex[4];
	bool rotate[4];

	Coordinate& operator [] (int);
	int size();
};

class Puzzle {
private:
	vector<vector<char>> board;
	vector<Tile> tiles;
public:
	Puzzle(string);
	void InputDFS(int, int, int, int, int, vector<string> &, vector<vector<bool>> &);
	void PrintPuzzle();
	void Solve();
};

