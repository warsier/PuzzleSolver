#include "puzzlesolver.h"
#include "draw.h"
#include "common.h"


int main()
{
	vector<string> testfiles;
	FileFinder("..\\PuzzleSolver\\", "pentominoes3x20.puzzle", testfiles);
	for (auto i = testfiles.begin(); i != testfiles.end(); i++) {
		cout << (*i) << endl;
		Puzzle p(*i);
		p.Solve();
		p.PrintAnswerCnt();
		vector<vector<vector<Coordinate>>> answers = p.ReadAnswers();
		for (int i = 0; i < 48; i++) {
			for (auto j = answers[i].begin(); j != answers[i].end(); j++) {
				for (auto k = j->begin(); k != j->end(); k++)
					k->Print();
				cout << endl;
			}
			PuzzleDrawer d(p.width(), p.length(), answers[i]);
			d.DrawTiles();
			getchar();
		}
			
	}
	system("pause");
	return 0;
}