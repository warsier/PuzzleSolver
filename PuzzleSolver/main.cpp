#include "puzzlesolver.h"
#include "common.h"

int main()
{
	vector<string> testfiles;
	
	string fileName;
	bool flag = false;
	while (!flag)
	{
		cout << "Please input the puzzle file name:" << endl;
		cin >> fileName;
		flag = FileFinder("..\\PuzzleSolver\\", fileName, testfiles);
	}

	//FileFinder("..\\PuzzleSolver\\", "pentominoes5x12.puzzle", testfiles);

	for (auto i = testfiles.begin(); i != testfiles.end(); i++) {
		cout << (*i) << endl;
		Puzzle p(*i);
		p.Solve();
		p.PrintAnswer();
	}
	system("pause");
	return 0;
}