#include "puzzlesolver.h"
#include "common.h"


int main()
{
	vector<string> testfiles;
	FileFinder("..\\PuzzleSolver\\", "pentominoes5x12.puzzle", testfiles);
	for (auto i = testfiles.begin(); i != testfiles.end(); i++) {
		cout << (*i) << endl;
		Puzzle p(*i);
		p.Solve();
		p.PrintAnswer();
	}
	system("pause");
	return 0;
}