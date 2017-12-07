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

DancingNode::DancingNode()
{
	left = this;
	right = this;
	up = this;
	down = this;
	size = 0;
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
	// parameter of rotation matrix & reflection matrix
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
	/*****************
	get inputfile
	*****************/
	ifstream ifs(inputfile);

	/*****************
	use DFS to process input file into tiles
	*****************/
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

	/*****************
	find out max tile and pop out (then make it the board)
	*****************/
	int maxtilepos = 0, maxtileval = 0; // if initialize maxtileval = -1, then no zuo no die
	for (int i = 0; i < tiles.size(); i++) {
		if (tiles[i].rotateblocks[0].size() > maxtileval) {
			maxtilepos = i;
			maxtileval = tiles[i].rotateblocks[0].size();
		}
	}
	swap(tiles[maxtilepos], tiles[tiles.size() - 1]);

	Tile maxtile = tiles.back();
	tiles.pop_back();

	maxtile.Normalize(maxtile.rotateblocks[0]);
	maxtile.width = -1; maxtile.length = -1;
	for (auto i = maxtile.rotateblocks[0].begin(); i != maxtile.rotateblocks[0].end(); i++) {
		if (i->x > maxtile.width) maxtile.width = i->x;
		if (i->y > maxtile.length) maxtile.length = i->y;
	}
	maxtile.width++; maxtile.length++;

	board.resize(maxtile.width);
	for (int i = 0; i < board.size(); i++)
		board[i].resize(maxtile.length, ' ');

	for (auto i = maxtile.rotateblocks[0].begin(); i != maxtile.rotateblocks[0].end(); i++) {
		board[i->x][i->y] = i->content;
	}

	/*****************
	find out all rotations and reflextions (if applicable) of that tile
	then configure them in a Normalized form
	*****************/
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
	for (auto i = board.begin(); i != board.end(); i++) {
		for (auto j = i->begin(); j != i->end(); j++)
			cout << (*j);
		cout << endl;
	}
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

void Puzzle::PrintAnswer(vector<DancingNode *> &answerpointer)
{
	for (auto i = answerpointer.begin(); i != answerpointer.end(); i++) {
		cout << "id = " << (*i)->tileid << " ";
		if ((*i)->pos.content != ' ')
			(*i)->pos.Print();
		for (auto j = (*i)->right; j != (*i); j = j->right) {
			if (j->pos.content != ' ')
				j->pos.Print();
		}
		cout << endl;
	}
}

void Puzzle::DancingNodeGen(vector<DancingNode> &validcovers, vector<vector<Coordinate>> &positions, vector<Coordinate> &tile, int tileid, int xoffset, int yoffset)
{
	DancingNode tempd;
	vector<Coordinate> tempp;
	tempd.size = 0; tempd.tileid = tileid;
	for (auto i = tile.begin(); i != tile.end(); i++) {
		tempp.push_back(*i);
		auto p = &tempp[tempp.size() - 1];
		p->x += xoffset;
		p->y += yoffset;
		if (board[p->x][p->y] != p->content) return;
	}

	validcovers.push_back(tempd);
	positions.push_back(tempp);
}

void Puzzle::NetworkWeaver(DancingNode &head, vector<DancingNode> &rows, vector<DancingNode> &validcovers, vector<vector<Coordinate>> &positions)
{
	for (int i = 0; i < board.size() * board[0].size(); i++)
		rows.push_back(DancingNode());
	for (int i = 0; i < tiles.size(); i++)
		rows.push_back(DancingNode());

	/*****************
	find out all valid covers and store them in a vector
	*****************/
	
	// for each tile
	for (int i = 0; i < tiles.size(); i++) {
		// for each rotation in each tile
		for (int ii = 0; ii < 4; ii++) {
			// if this rotation is valid
			if (tiles[i].rotateflag[ii]) {
				// find all valid covers for it
				for (int j = 0; j <= (int)board.size() - (ii % 2 ? tiles[i].length : tiles[i].width); j++) {
					for (int k = 0; k <= (int)board[0].size() - (ii % 2 ? tiles[i].width : tiles[i].length); k++) {
						DancingNodeGen(validcovers, positions, tiles[i].rotateblocks[ii], i, j, k);
					}
				}
			}
		}

		// for each reflection in each tile
		for (int ii = 0; ii < 4; ii++) {
			// if this reflection is valid
			if (tiles[i].reflexflag[ii]) {
				// find all valid covers for it
				for (int j = 0; j <= (int)board.size() - (ii % 2 ? tiles[i].length : tiles[i].width); j++) {
					for (int k = 0; k <= (int)board[0].size() - (ii % 2 ? tiles[i].width : tiles[i].length); k++) {
						DancingNodeGen(validcovers, positions, tiles[i].reflexblocks[ii], i, j, k);
					}
				}
			}
		}
	}


	/*****************
	chain all DancingNodes
	*****************/
	head.right = &rows[0];
	rows[0].left = &head;
	head.left = &rows[rows.size() - 1];
	rows[rows.size() - 1].right = &head;

	for (int i = 0; i < rows.size() - 1; i++) {
		rows[i].right = &rows[i + 1];
		rows[i + 1].left = &rows[i];
	}

	// have to re-point them because pushing them into vector changes their address
	for (int i = 0; i < validcovers.size(); i++) {
		validcovers[i].left = &validcovers[i];
		validcovers[i].right = &validcovers[i];
	}
	
	for (int i = 0; i < rows.size(); i++) {
		rows[i].up = &rows[i];
		rows[i].down = &rows[i];
	}

	/*****************
	Weave the network
	*****************/
	auto pos = positions.begin();
	for (auto i = validcovers.begin(); i != validcovers.end(); i++, pos++) {

		// weave id node into network
		DancingNode *p = new DancingNode;
		DancingNode *tempd = &rows[i->tileid + board.size() * board[0].size()];
		i->left->right = p; p->left = i->left;
		p->right = &(*i); i->left = p;
		tempd->up->down = p; p->up = tempd->up;
		p->down = tempd; tempd->up = p;
		p->column = tempd;
		p->tileid = i->tileid;
		p->pos.content = ' ';

		for (auto j = pos->begin(); j != pos->end(); j++) {
			// weave tile node into network
			p = new DancingNode;
			DancingNode *tempd = &rows[j->y * board.size() + j->x];
			i->left->right = p; p->left = i->left;
			p->right = &(*i); i->left = p;
			tempd->up->down = p; p->up = tempd->up;
			p->down = tempd; tempd->up = p;
			p->column = tempd;
			p->tileid = i->tileid;
			p->pos = (*j);
		}
	}

	/*****************
	remove scaffold
	*****************/

	for (auto i = validcovers.begin(); i != validcovers.end(); i++) {
		i->right->left = i->left;
		i->left->right = i->right;
	}

	/*****************
	count size of rows and number rows
	*****************/

	int tempid = 0;
	for (DancingNode *i = head.right; i != &head; i = i->right) {
		if (tempid < board.size() * board[0].size()) 
			i->tileid = tempid;
		else
			i->tileid = -1;
		tempid++;

		int cnt = 0;
		for (DancingNode *j = i->down; j != i; j = j->down)
			cnt++;
		i->size = cnt;
	}
	/*for (auto i = head.right->down->right; i != head.right->down; i = i->right)
		cout << i << endl;*/
}

void Puzzle::NetworkDeleter(vector<DancingNode> &rows)
{
	/*****************
	Delete dynamic allocated elements in network
	*****************/
	for (auto i = rows.begin(); i != rows.end(); i++) {
		while (i->down != &(*i)) {
			DancingNode *p = i->down;
			i->down = p->down;
			p->down->up = &(*i);
			delete p;
		}
	}
}

void Puzzle::Cover(DancingNode *c)
{
	c->right->left = c->left;
	c->left->right = c->right;
	for (DancingNode *i = c->down; i != c; i = i->down) {
		for (DancingNode *j = i->right; j != i; j = j->right) {
			j->down->up = j->up;
			j->up->down = j->down;
			j->column->size -= 1;
		}
	}
}

void Puzzle::Uncover(DancingNode *c)
{
	for (DancingNode *i = c->up; i != c; i = i->up) {
		for (DancingNode *j = i->left; j != i; j = j->left) {
			j->column->size += 1;
			j->down->up = j;
			j->up->down = j;
		}
	}
	c->right->left = c;
	c->left->right = c;
}

void Puzzle::DancingDFS(DancingNode &head, vector<DancingNode *> &answerpointer)
{
	/*if (answerpointer.size() >= 10)
		cout << answerpointer.size() << endl;*/
	if (head.right == &head) {
		PrintAnswer(answerpointer);
		answercnt++;
		if (answercnt % 10 == 0) cout << answercnt << endl;
		return;
	}

	// choose a column to cover
	DancingNode *c = head.right;
	int minsize = 0x7fffffff;
	for (DancingNode *i = head.right; i != &head; i = i->right) {
		if (i->size < minsize) {
			c = i;
			minsize = i->size;
		}
	}


	Cover(c);
	for (DancingNode *i = c->down; i != c; i = i->down) {
		answerpointer.push_back(i);
		for (DancingNode *j = i->right; j != i; j = j->right)
			Cover(j->column);
		DancingDFS(head, answerpointer);
		for (DancingNode *j = answerpointer.back()->left; j != answerpointer.back(); j = j->left)
			Uncover(j->column);
		answerpointer.pop_back();
	}
	Uncover(c);

}

void Puzzle::Solve()
{
	/*****************
	initialize dancing link
	*****************/
	DancingNode head;
	vector<DancingNode> indices;
	vector<DancingNode> rows;
	vector<DancingNode> validcovers;
	vector<vector<Coordinate>> positions;
	
	NetworkWeaver(head, rows, validcovers, positions);

	/*****************
	Start to dance!
	*****************/
	answercnt = 0;
	vector<DancingNode *> answerpointer;
	DancingDFS(head, answerpointer);

	NetworkDeleter(rows);
	
	/*DancingNode *ddd = validcovers[0].down;
	for (int t = 0; t < 10; t++) {
		cout << ddd << endl;
		ddd = ddd->down;
	}
	cout << endl;*/
}


