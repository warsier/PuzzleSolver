﻿#pragma once
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

class DancingNode {
public:
	DancingNode *left, *right;
	DancingNode *up, *down;
	DancingNode *column;
	int size;
	int tileid;
	vector<Coordinate> position;
	DancingNode();
};

/*******************************
        length
      ┌────────> y
      │
width │
      │
      V
      x

0 <= x < board.size()
0 <= y < board[0].size()
*******************************/

class Tile {
public:
	int width, length;
	vector<Coordinate> rotateblocks[4];
	vector<Coordinate> reflexblocks[4];
	bool rotateflag[4];
	bool reflexflag[4];

	void Normalize(vector<Coordinate> &);
	bool Differentblocks(vector<Coordinate> &, vector<Coordinate> &);
	void ProcessTile(bool);
	int size();
};

class Puzzle {
private:
	vector<vector<char>> board;
	vector<Tile> tiles;
	vector<vector<Coordinate>> answer;
public:
	Puzzle(string);
	void InputDFS(int, int, int, vector<string> &, vector<vector<bool>> &);
	void PrintPuzzle();
	void DancingNodeGen(vector<DancingNode> &, vector<Coordinate> &, int, int, int);
	void NetworkWeaver(DancingNode &, vector<DancingNode> &, vector<DancingNode> &, vector<DancingNode> &);
	void Puzzle::NetworkDeleter(vector<DancingNode> &);
	void Puzzle::DancingDFS(DancingNode &, vector<DancingNode *> &);
	void Solve();
};

