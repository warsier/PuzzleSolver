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

class DancingNode {
public:
	DancingNode *left, *right;
	DancingNode *up, *down;
	DancingNode *column;
	int size;
	int tileid;
	Coordinate pos;
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
	vector<vector<vector<Coordinate>>> answer;
	int answercnt;
public:
	Puzzle(string);
	void InputDFS(int, int, int, vector<string> &, vector<vector<bool>> &);

	void PrintPuzzle();
	void PrintAnswer(vector<DancingNode *> &answerpointer, bool);
	vector<vector<vector<Coordinate>>> ReadAnswers();
	void PrintAnswerCnt();

	void DancingNodeGen(vector<DancingNode> &, vector<vector<Coordinate>> &, vector<Coordinate> &, int, int, int);

	void NetworkWeaver(DancingNode &,  vector<DancingNode> &, vector<DancingNode> &, vector<vector<Coordinate>> &);
	void Puzzle::NetworkDeleter(vector<DancingNode> &);

	void Puzzle::Cover(DancingNode *);
	void Puzzle::Uncover(DancingNode *);
	void Puzzle::DancingDFS(DancingNode &, vector<DancingNode *> &);
	void Solve();
};

