#pragma once

#include "common.h"

struct Tile {

};

class Puzzle {
private:
	Tile board;
	vector<Tile> tiles;
public:
	Puzzle(string inputfile);
};

