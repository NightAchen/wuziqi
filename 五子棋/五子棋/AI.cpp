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
	Sleep(1000);//��װ˼��
	chess->chessDown(&pos, CHESS_WHITE);
}

void AI::go2()
{
	ChessPos pos = think();
	Sleep(1000);//��װ˼��
	chess->chessDown(&pos, CHESS_BLACK);
}

void AI::calculateScore()
{
	// ͳ����һ��ߵ����м������������ӣ�����������
	int personNum = 0;  // ��������ӵĸ���
	int aiNum = 0;     // AI�����ӵĸ���
	int emptyNum = 0;   // ������հ�λ�ĸ���

	// ����������������
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
			// �������λ��Ϊ�հ�
			if (chess->getChessData(row, col) == 0) {
				// ������Χ�˸����� ��y=-1,x=-1��Ϊ���Ͻǣ���y=-1,x=0��Ϊ�󣩣�y=-1,x=1��Ϊ���½ǣ���y=0,x=-1��Ϊ�ϣ���y=0,x=1��Ϊ�£�
				//					��y=1,x=-1��Ϊ���Ͻǣ���y=1,x=0��Ϊ�ң���y=1,x=1��Ϊ���½ǣ�
				for (int y = -1; y <= 1; y++) {
					for (int x = -1; x <= 1; x++)
					{
						// ����
						personNum = 0;
						aiNum = 0;
						emptyNum = 0;

						// ԭ���겻��
						if (!(y == 0 && x == 0))
						{
							// ÿ��������������4����
							// �Ժ������֣�������������
							for (int i = 1; i <= 4; i++)
							{
								int curRow = row + i * y;
								int curCol = col + i * x;
								if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 1) // ��¼�������ĳ�����������ӵĸ���
								{
									personNum++;
								}
								else if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // �հ�λ
								{
									emptyNum++;
									break;
								}
								else            // ���߽�
									break;
							}

							//������
							for (int i = 1; i <= 4; i++)
							{
								int curRow = row - i * y;
								int curCol = col - i * x;
								if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 1) // ������ҵ���
								{
									personNum++;
								}
								else if (curRow >= 0 && curRow < size &&
									curCol >= 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // �հ�λ
								{
									emptyNum++;
									break;
								}
								else            // ���߽�
									break;
							}

							if (personNum == 1)                      // ����
								scoreMap[row][col] += 10;
							else if (personNum == 2)                 // ����
							{
								if (emptyNum == 1)//����
									scoreMap[row][col] += 30;
								else if (emptyNum == 2)//����
									scoreMap[row][col] += 40;
							}
							else if (personNum == 3)                 // ����
							{
								// �����λ��һ�������ȼ���һ��
								if (emptyNum == 1)//����
									scoreMap[row][col] += 60;
								else if (emptyNum == 2)//����
									scoreMap[row][col] += 200;
							}
							else if (personNum == 4)                 // ����
								scoreMap[row][col] += 20000;

							// ����һ�����
							emptyNum = 0;

							// �԰�������
							for (int i = 1; i <= 4; i++)
							{
								int curRow = row + i * y;
								int curCol = col + i * x;
								if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == -1) // ai����
								{
									aiNum++;
								}
								else if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // �հ�λ
								{
									emptyNum++;
									break;
								}
								else            // ���߽�
									break;
							}

							for (int i = 1; i <= 4; i++)
							{
								int curRow = row - i * y;
								int curCol = col - i * x;
								if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == -1) // ��ҵ���
								{
									aiNum++;
								}
								else if (curRow > 0 && curRow < size &&
									curCol > 0 && curCol < size &&
									chess->getChessData(curRow, curCol) == 0) // �հ�λ
								{
									emptyNum++;
									break;
								}
								else            // ���߽�
									break;
							}

							if (aiNum == 0)                      // ��ͨ����
								scoreMap[row][col] += 5;
							else if (aiNum == 1)                 // ���
								scoreMap[row][col] += 10;
							else if (aiNum == 2)
							{
								if (emptyNum == 1)                // ����
									scoreMap[row][col] += 25;
								else if (emptyNum == 2)
									scoreMap[row][col] += 50;  // ����
							}
							else if (aiNum == 3)
							{
								if (emptyNum == 1)                // ����
									scoreMap[row][col] += 55;
								else if (emptyNum == 2)
									scoreMap[row][col] += 10000; // ����
							}
							else if (aiNum >= 4)
								scoreMap[row][col] += 30000;   // ���壬Ӧ�þ���������ȼ�
						}
					}
				}
				int diff[4][2] = { {0,-1}, {-1,-1}, {-1, 0}, {-1, 1} };
				//����4������ - | / \
				//1.-����
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

					//�ٻ����������			   
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
					//����
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
	//���ǵ����������ĵ���ܲ�ֹ��һ��
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
	//�ڶ���������������ѡ��һ����
	srand(time(nullptr));
	int index = rand() % maxPoints.size();

	return maxPoints[index];

	//return ChessPos();
}
