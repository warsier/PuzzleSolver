#include "puzzlesolver.h"
#include "draw.h"
#include "common.h"


int main()
{
	vector<string> testfiles;
	//FileFinder("..\\PuzzleSolver\\", "pentominoes3x20.puzzle", testfiles);

	string fileName;
	bool flag = false;
	while (!flag)
	{
		cout << "Please input the puzzle file name:" << endl;
		cin >> fileName;
		flag = FileFinder("..\\PuzzleSolver\\", fileName, testfiles);
		if (!flag) cout << "Incorrect file name." << endl;
	}

	for (auto i = testfiles.begin(); i != testfiles.end(); i++) 
	{
		cout << (*i) << endl;
		Puzzle p(*i);
		p.Solve();
		//p.PrintAnswerCnt();
		vector<vector<vector<Coordinate>>> answers = p.ReadAnswers();

		if (answers.size() > 0)
		{
			int displayNo;
			while (true)
			{
				cout << endl << "There are " << answers.size() << "(from 1 to " << answers.size() <<") answers." << endl;
				cout << "Which answer do you want to display? Enter 0 to exit." << endl;
				cout << "The number of answer you want to see is: ";
				cin >> displayNo;
				if (displayNo == 0) break;
				if (displayNo < 0 || displayNo > answers.size())
				{
					cout << "Invalid index." << endl;
					continue;
				}
				displayNo--;

				/*for (auto j = answers[displayNo].begin(); j != answers[displayNo].end(); j++) 
				{
					for (auto k = j->begin(); k != j->end(); k++)
						k->Print();
					cout << endl;
				}*/
				PuzzleDrawer d(p.width(), p.length(), answers[displayNo]);
				d.DrawTiles();
				getchar();
			}
		}
		else cout << "There is no answer to this puzzle." << endl;

		/*for (int i = 0; i < answers.size(); i++) {
			for (auto j = answers[i].begin(); j != answers[i].end(); j++) {
				for (auto k = j->begin(); k != j->end(); k++)
					k->Print();
				cout << endl;
			}
			PuzzleDrawer d(p.width(), p.length(), answers[i]);
			d.DrawTiles();
			getchar();
		}*/
			
	}
	system("pause");
	return 0;
}