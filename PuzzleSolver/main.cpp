#include "puzzlesolver.h"
#include "draw.h"
#include "common.h"


int main()
{
	vector<string> testfiles;
	FileFinder("..\\PuzzleSolver\\", "trivial.puzzle", testfiles);
	for (auto i = testfiles.begin(); i != testfiles.end(); i++) {
		cout << (*i) << endl;
		Puzzle p(*i);
		p.Solve();
		auto answers = p.ReadAnswers();
		for (int i = 0; i < 10; i++) {
			PuzzleDrawer d(20, 20, answers[i]);
			d.DrawTiles();
		}
			
	}
	system("pause");
	return 0;
}