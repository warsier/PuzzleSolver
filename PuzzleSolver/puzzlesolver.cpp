#include "puzzlesolver.h"

Coordinate::Coordinate() {}

Coordinate::Coordinate(int tx, int ty, char tcontent)
{
	x = tx; y = ty; content = tcontent;
}

void Coordinate::Print()
{
	cout << "(" << x << "," << y << "," << content << ") ";
}

bool Coordinate::operator != (Coordinate a) const
{
	return (x != a.x || y != a.y || content != a.content);
}

Coordinate& Tile::operator [] (int x)
{
	return rotateblocks[0][x];
}

bool NormalizeComp(Coordinate a, Coordinate b)
{
	if (a.x == b.x)
		return a.y < b.y;
	return a.x < b.x;
}

// For a tile represented by the original position in the input,
// normalize this representation by bounding this tile in a rectangle
// and the top-left corner being (0, 0).
// Finally sort the coordinates.
void Tile::Normalize(vector<Coordinate> &blk)
{
	int minx = 0x7fffffff;
	int miny = 0x7fffffff;
	for (auto i = blk.begin(); i != blk.end(); i++) {
		if (i->x < minx) minx = i->x;
		if (i->y < miny) miny = i->y;
	}
	for (auto i = blk.begin(); i != blk.end(); i++) {
		i->x -= minx;
		i->y -= miny;
	}
	sort(blk.begin(), blk.end(), NormalizeComp);
}

bool Tile::Differentblocks(vector<Coordinate> &a, vector<Coordinate> &b)
{
	if (a.size() != b.size()) {
		cerr << "Differentblocks: Unexpected input" << endl;
		system("pause");
	}
	for (int i = 0; i < a.size(); i++) {
		if (a[i] != b[i]) return true;
	}
	return false;
}

// Given the original tile from input,
// computes whether rotate or reflex this tile is a new case.
// The width and length of the original tile is computed to show the boundarys.
// 
void Tile::ProcessTile(bool reflexible = true)
{
	// parameter of rotation matrix
	const int rotate_a[4] = { 1, 0, -1, 0 };
	const int rotate_b[4] = { 0, 1, 0, -1 };
	const int rotate_c[4] = { 0, -1, 0, 1 };
	const int rotate_d[4] = { 1, 0, -1, 0 };

	const int reflex_a[4] = { -1, 0, 1, 0 };
	const int reflex_b[4] = { 0, 1, 0, -1 };
	const int reflex_c[4] = { 0, 1, 0, -1 };
	const int reflex_d[4] = { 1, 0, -1, 0 };

	// check if rotate a degree of 90 * i deg clockwise is different
	for (int i = 1; i < 4; i++) {
		for (auto j = rotateblocks[0].begin(); j != rotateblocks[0].end(); j++) {
			Coordinate tempc;
			tempc.x = rotate_a[i] * (*j).x + rotate_b[i] * (*j).y;
			tempc.y = rotate_c[i] * (*j).x + rotate_d[i] * (*j).y;
			tempc.content = (*j).content;
			rotateblocks[i].push_back(tempc);
		}
	}

	for (int i = 0; i < 4; i++) {
		Normalize(rotateblocks[i]);
	}

	for (int i = 0; i < 4; i++) {
		bool tempflag = true;
		for (int j = 0; j < i; j++)
			tempflag &= Differentblocks(rotateblocks[i], rotateblocks[j]);
		rotateflag[i] = tempflag;
	}

	// if applicable, check if reflex using a vertical axis
	// then rotate a degree of 90 * i + 90 is different
	if (reflexible) {
		for (int i = 0; i < 4; i++) {
			for (auto j = rotateblocks[0].begin(); j != rotateblocks[0].end(); j++) {
				Coordinate tempc;
				tempc.x = reflex_a[i] * (*j).x + reflex_b[i] * (*j).y;
				tempc.y = reflex_c[i] * (*j).x + reflex_d[i] * (*j).y;
				tempc.content = (*j).content;
				reflexblocks[i].push_back(tempc);
			}
		}

		for (int i = 0; i < 4; i++) {
			Normalize(reflexblocks[i]);
		}

		for (int i = 0; i < 4; i++) {
			bool tempflag = true;
			for (int j = 0; j < 4; j++) 
				tempflag &= Differentblocks(reflexblocks[i], rotateblocks[j]);
			for (int j = 0; j < i; j++) 
				tempflag &= Differentblocks(reflexblocks[i], reflexblocks[j]);
			reflexflag[i] = tempflag;
		}
	}

	this->width = -1; this->length = -1;
	for (auto i = rotateblocks[0].begin(); i != rotateblocks[0].end(); i++) {
		if (i->x > this->width) this->width = i->x;
		if (i->y > this->length) this->length = i->y;
	}
	this->width++; this->length++;
	
}

int Tile::size() 
{
	return rotateblocks[0].size();
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
		i->ProcessTile();
	}

	PrintPuzzle();
}

// x and y show the current DFS position,
// id shows the id of the tile.
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
		tiles[id].rotateblocks[0].push_back(Coordinate(x, y, arr[x][y]));
	}
	else {
		tiles[id].rotateblocks[0].push_back(Coordinate(x, y, arr[x][y]));
	}

	InputDFS(x - 1, y, id, arr, visited);
	InputDFS(x + 1, y, id, arr, visited);
	InputDFS(x, y - 1, id, arr, visited);
	InputDFS(x, y + 1, id, arr, visited);
}


void Puzzle::PrintPuzzle()
{
	for (auto i = tiles.begin(); i != tiles.end(); i++) {
		cout << "0 <= x < " << i->width << ", 0 <= y < " << i->length << endl;
		for (auto j = i->rotateblocks[0].begin(); j != i->rotateblocks[0].end(); j++) {
			j->Print();
		}
		cout << endl;
		for (int j = 0; j < 4; j++) cout << (i->rotateflag[j]) << ' ';
		cout << endl;
		for (int j = 0; j < 4; j++) cout << (i->reflexflag[j]) << ' ';
		cout << endl;
	}
	
}


