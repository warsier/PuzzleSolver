#include "puzzlesolver.h"
#include "common.h"


int main()
{
	vector<string> testfiles;
	FileFinder("..\\PuzzleSolver\\", "trivial.puzzle", testfiles);
	for (auto i = testfiles.begin(); i != testfiles.end(); i++) {
		cout << (*i) << endl;
		Puzzle p(*i);
		p.Solve();
	}
	system("pause");
	return 0;
}