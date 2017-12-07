#pragma once
#include<vector>
#include<cstring>
#include<graphics.h>
#include<conio.h>

#define WIDTH 20

using namespace std;

class PuzzleDrawer {
private:
	int n, m;	//n:row, m:line
	vector<vector<pair<int, char>>> tiles;
	vector<int> colors;

public:
	PuzzleDrawer(int, int, vector<vector<pair<int, char>>>);
	void DrawTiles();
};