#pragma once
#include "common.h"

class Coordinate {
public:
	int x, y;
	char content;

	
	Coordinate();
	Coordinate(int, int, char);
	bool operator != (Coordinate) const;
	void Print();
};

class Tile {
public:
	int width, length;
	vector<Coordinate> rotateblocks[4];
	vector<Coordinate> reflexblocks[4];
	bool rotateflag[4];
	bool reflexflag[4];

	Coordinate& operator [] (int);
	void Normalize(vector<Coordinate> &);
	bool Differentblocks(vector<Coordinate> &, vector<Coordinate> &);
	void ProcessTile(bool);
	int size();
};

class Puzzle {
private:
	vector<vector<char>> board;
	vector<Tile> tiles;
public:
	Puzzle(string);
	void InputDFS(int, int, int, vector<string> &, vector<vector<bool>> &);
	void PrintPuzzle();
	void Solve();
};

