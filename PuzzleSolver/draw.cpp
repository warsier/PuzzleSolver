#include "draw.h"

PuzzleDrawer::PuzzleDrawer(int row, int line, vector<vector<Coordinate>> content)
{
	n = row;
	m = line;
	tiles = content;

	colors.push_back(BLUE);
	colors.push_back(DARKGRAY);
	colors.push_back(RED);
	colors.push_back(GREEN);
	colors.push_back(YELLOW);
	colors.push_back(LIGHTBLUE);
	colors.push_back(MAGENTA);
	colors.push_back(LIGHTGRAY);
	colors.push_back(LIGHTRED);
}

void PuzzleDrawer::DrawTiles()
{
	initgraph(m * WIDTH, n * WIDTH);
	setbkcolor(BLACK);
	cleardevice();

	int colorIndex = 0;
	int colorMap[201][201];
	memset(colorMap, 0, sizeof(colorMap));

	for (int i = 0; i < tiles.size(); i++)
	{
		vector<Coordinate>thisTile = tiles[i];
		bool check_flag = false;
		while (!check_flag)
		{
			for (int j = 0; j < thisTile.size(); j++)
			{
				int x = thisTile[j].x + 1;
				int y = thisTile[j].y + 1;
				vector<int> neighborColor;
				neighborColor.push_back(colorMap[x - 1][y]);
				neighborColor.push_back(colorMap[x + 1][y]);
				neighborColor.push_back(colorMap[x][y - 1]);
				neighborColor.push_back(colorMap[x][y + 1]);
				for (int k = 0; k < 4; k++)
				{
					if (colors[colorIndex] == neighborColor[k])
					{
						colorIndex = (colorIndex + 1) % colors.size();
						break;
					}
				}
			}
			check_flag = true;
		}
		setfillcolor(colors[colorIndex]);
		for (int j = 0; j < thisTile.size(); j++)
		{
			int x = thisTile[j].x + 1;
			int y = thisTile[j].y + 1;
			solidrectangle((y - 1)*WIDTH, (x - 1)*WIDTH, y*WIDTH, x*WIDTH);
			settextstyle(WIDTH, WIDTH, _T("Consolas"));
			setbkmode(TRANSPARENT);
			outtextxy((y - 1)*WIDTH, (x - 1)*WIDTH, _T(thisTile[j].content));
			colorMap[x][y] = colors[colorIndex];
		}
		colorIndex = (colorIndex + 1) % colors.size();
	}
}

