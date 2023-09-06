#include "AI.h"
#include<random>
#include<ctime>

void AI::Init(Chess * chess)
{
	this->chess = chess;

	int size = chess->getGradeSize();
	for(int i = 0; i < size; i++)
	{
		vector<int> row;
		for (int j = 0; j < size; j++)
		{
			row.push_back(0);
		}
		scoreMap.push_back(row);
	}
}

void AI::go()
{
	ChessPos pos = think();
	Sleep(1000);//假装思考
	chess->chessDown(&pos, CHESS_WHITE);
}

void AI::go2()
{
	ChessPos pos = think();
	Sleep(1000);//假装思考
	chess->chessDown(&pos, CHESS_BLACK);
}

void AI::calculateScore()
{
	// 统计玩家或者电脑有几颗连续的棋子，并进行评分
	int personNum = 0;  // 玩家连成子的个数
	int aiNum = 0;     // AI连成子的个数
	int emptyNum = 0;   // 各方向空白位的个数

	// 评分向量数组清零
	for (int i = 0; i < scoreMap.size(); i++) {
		for (int j = 0; j < scoreMap[i].size(); j++) {
			scoreMap[i][j] = 0;
		}
	}
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			// 如果落子位置为空白
			if (chess->getChessData(row, col) == 0) {
				// 遍历周围八个方向 （y=-1,x=-1即为左上角）（y=-1,x=0即为左）（y=-1,x=1即为左下角）（y=0,x=-1即为上）（y=0,x=1即为下）
				//					（y=1,x=-1即为右上角）（y=1,x=0即为右）（y=1,x=1即为右下角）
				for (int y = -1; y <= 1; y++) {
					for (int x = -1; x <= 1; x++)
					{
						// 重置
						personNum = 0;
						aiNum = 0;
						emptyNum = 0;

						// 原坐标不算
						if (!(y == 0 && x == 0))
						{
							// 每个方向向外延伸4个子
							// 对黑棋评分（正反两个方向）
							for (int i = 1; i <= 4; i++)
							{
								int curRow = row + i * y;
								int curCol = col + i * x;
								if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 1) // 记录真人玩家某个方向上棋子的个数
								{
									personNum++;
								}
								else if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // 空白位
								{
									emptyNum++;
									break;
								}
								else            // 出边界
									break;
							}

							//反方向
							for (int i = 1; i <= 4; i++)
							{
								int curRow = row - i * y;
								int curCol = col - i * x;
								if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 1) // 真人玩家的子
								{
									personNum++;
								}
								else if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // 空白位
								{
									emptyNum++;
									break;
								}
								else            // 出边界
									break;
							}

							if (personNum == 1)                      // 连二
								scoreMap[row][col] += 10;
							else if (personNum == 2)                 // 连三
							{
								if (emptyNum == 1)//死三
									scoreMap[row][col] += 30;
								else if (emptyNum == 2)//活三
									scoreMap[row][col] += 40;
							}
							else if (personNum == 3)                 // 连四
							{
								// 量变空位不一样，优先级不一样
								if (emptyNum == 1)//死四
									scoreMap[row][col] += 60;
								else if (emptyNum == 2)//活四
									scoreMap[row][col] += 200;
							}
							else if (personNum == 4)                 // 连五
								scoreMap[row][col] += 20000;

							// 进行一次清空
							emptyNum = 0;

							// 对白棋评分
							for (int i = 1; i <= 4; i++)
							{
								int curRow = row + i * y;
								int curCol = col + i * x;
								if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == -1) // ai的子
								{
									aiNum++;
								}
								else if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // 空白位
								{
									emptyNum++;
									break;
								}
								else            // 出边界
									break;
							}

							for (int i = 1; i <= 4; i++)
							{
								int curRow = row - i * y;
								int curCol = col - i * x;
								if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == -1) // 玩家的子
								{
									aiNum++;
								}
								else if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // 空白位
								{
									emptyNum++;
									break;
								}
								else            // 出边界
									break;
							}

							if (aiNum == 0)                      // 普通下子
								scoreMap[row][col] += 5;
							else if (aiNum == 1)                 // 活二
								scoreMap[row][col] += 10;
							else if (aiNum == 2)
							{
								if (emptyNum == 1)                // 死三
									scoreMap[row][col] += 25;
								else if (emptyNum == 2)
									scoreMap[row][col] += 50;  // 活三
							}
							else if (aiNum == 3)
							{
								if (emptyNum == 1)                // 死四
									scoreMap[row][col] += 55;
								else if (emptyNum == 2)
									scoreMap[row][col] += 10000; // 活四
							}
							else if (aiNum >= 4)
								scoreMap[row][col] += 30000;   // 活五，应该具有最高优先级
						}
					}
				}
				int diff[4][2] = { {0,-1}, {-1,-1}, {-1, 0}, {-1, 1} };
				//遍历4个大方向 - | / \
				//1.-方向
				for (int i = 0; i < 4; i++)
				{
					int cur_row = row + diff[i][0];
					int cur_col = col + diff[i][1];

					int count_white = 0;
					int count_black = 0;
					int count_empty = 0;

					while (cur_row >= 0 && cur_row < size &&
						cur_col >= 0 && cur_col < size &&
						chess->getChessData(cur_row, cur_col) == -1)
					{
						cur_row += diff[i][0];
						cur_col += diff[i][1];
						count_white++;
					}
					if (cur_row >= 0 && cur_row < size && cur_col >= 0 && cur_col < size &&chess->getChessData(cur_row, cur_col) == 1)
					{
						continue;					
					}
					cur_row = row - diff[i][0];
					cur_col = col - diff[i][1];

					//再换个方向计数			   
					while (cur_row >= 0 && cur_row < size &&
						cur_col >= 0 && cur_col < size &&
						chess->getChessData(cur_row, cur_col) == -1)
					{
						cur_row -= diff[i][0];
						cur_col -= diff[i][1];
						count_white++;
					}
					if (cur_row >= 0 && cur_row < size && cur_col >= 0 && cur_col < size &&chess->getChessData(cur_row, cur_col) == 1)
					{
						continue;						
					}
					//评分
					if (count_white == 3) 
					{
						scoreMap[row][col] = max(scoreMap[row][col],30000);
					}
					else if (count_white == 4) 
					{
						scoreMap[row][col] = max(scoreMap[row][col], 50000);
					}
				}
			}
		}
	}
}

ChessPos AI::think()
{
	calculateScore();
	//考虑到有最大分数的点可能不止有一个
	vector<ChessPos> maxPoints;
	int maxScore = 0;
	int size = chess->getGradeSize();
	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			if (chess->getChessData(row, col) == 0)
			{
				if (scoreMap[row][col] > maxScore)
				{
					maxScore = scoreMap[row][col];
					maxPoints.clear();
					maxPoints.push_back(ChessPos(row, col));
				}
				else if (scoreMap[row][col] == maxScore)
				{
					maxPoints.push_back(ChessPos(row, col));
				}
			}
		}
	}
	//在多个最大分数点中随机选择一个点
	srand(time(nullptr));
	int index = rand() % maxPoints.size();

	return maxPoints[index];

	//return ChessPos();
}
