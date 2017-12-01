#include "puzzlesolver.h"

Coordinate::Coordinate(int tx, int ty, char tcontent)
{
	x = tx; y = ty; content = tcontent;
}

void Coordinate::Print()
{
	cout << "(" << x << "," << y << "," << content << ") ";
}

Coordinate& Tile::operator [] (int x)
{
	return blocks[x];
}

bool NormalizeComp(Coordinate a, Coordinate b)
{
	if (a.x == b.x)
		return a.y < b.y;
	return a.x < b.x;
}

void Tile::Normalize()
{
	int minx = 0x7fffffff;
	int miny = 0x7fffffff;
	for (auto i = blocks.begin(); i != blocks.end(); i++) {
		if (i->x < minx) minx = i->x;
		if (i->y < miny) miny = i->y;
	}
	for (auto i = blocks.begin(); i != blocks.end(); i++) {
		i->x -= minx;
		i->y -= miny;
	}
	sort(blocks.begin(), blocks.end(), NormalizeComp);
}

int Tile::size() 
{
	return blocks.size();
}

Puzzle::Puzzle(string inputfile)
{
	ifstream ifs(inputfile);

	vector<string> arr;
	int maxwidth = 0, maxlength;
	string line;

	while (getline(ifs, line)) {
		arr.push_back(line);
		if (line.size() > maxwidth)
			maxwidth = line.size(); // contents in arr not necessarily have the same length
	}
	maxlength = arr.size();

	vector<vector<bool>> visited;
	vector<bool> initv;
	initv.resize(maxwidth, false); // resize initv to maxwidth and fill all values with false
	for (int i = 0; i < maxlength; i++)
		visited.push_back(initv);

	for (int i = 0; i < maxlength; i++) {
		for (int j = 0; j < maxwidth; j++) {
			if (visited[i][j] == false) {
				InputDFS(i, j, -1, arr, visited);
			}
		}
	}

	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		i->Normalize();
	}

	PrintPuzzle();
}

// x and y show the current DFS position
// id shows the id of the tile
void Puzzle::InputDFS(
	int x, int y, int id,
	vector<string> &arr, vector<vector<bool>> &visited)
{
	if (x < 0 || x >=arr.size() ||
		y < 0 || y >= arr[x].size() ||
		visited[x][y] == true || arr[x][y] == ' ')
		return; // boundary check

	visited[x][y] = true;

	// if it is the start of a new tile
	if (id == -1) {
		tiles.push_back(Tile());
		id = tiles.size() - 1;
		tiles[id].blocks.push_back(Coordinate(x, y, arr[x][y]));
	}
	else {
		tiles[id].blocks.push_back(Coordinate(x, y, arr[x][y]));
	}

	InputDFS(x - 1, y, id, arr, visited);
	InputDFS(x + 1, y, id, arr, visited);
	InputDFS(x, y - 1, id, arr, visited);
	InputDFS(x, y + 1, id, arr, visited);
}


void Puzzle::PrintPuzzle()
{
	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		for (auto j = i->blocks.begin(); j != i->blocks.end(); j++) {
			j->Print();
		}
		cout << endl;
	}
}
