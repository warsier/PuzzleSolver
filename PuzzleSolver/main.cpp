#include "puzzlesolver.h"
#include "common.h"


int main()
{
	vector<string> testfiles;
	FileFinder("..\\PuzzleSolver\\", "*.puzzle", testfiles);
	for (auto i = testfiles.begin(); i != testfiles.end(); i++) {
		cout << (*i) << endl;
		Puzzle p(*i);
	}
	system("pause");
	return 0;
}