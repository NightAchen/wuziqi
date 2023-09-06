#pragma once
#include"graphics.h"//ͼ�ο�
#include<vector>
using namespace std;


struct ChessPos
{
	int row;
	int col;

	ChessPos(int r=0,int c=0):row(r),col(c){}
};
//����			//����
using chess_kind_t = enum { CHESS_WHITE = -1, CHESS_BLACK = 1 };

class Chess
{
public:
	Chess(int gradeSize, int margin_x, int margin_y, float chessSize);

	void Init();

	//���������
	bool clickBorad(int x, int y, ChessPos *pos);

	//����λ��
	void chessDown(ChessPos *pos, chess_kind_t kind);

	//��ȡ���̴�С(13,15,19)
	int getGradeSize();

	//��ȡָ��λ���ǰ��廹�Ǻ��壬���߿հ�
	int getChessData(ChessPos *pos);
	int getChessData(int row, int col);

	//����Ƿ����
	bool checkOver();

	//ʹ��ͼ�ο�easyx����ʾ���̺�����
private:
	IMAGE chessBlackImg;//����
	IMAGE chessWhiteImg;//����
	int gradeSize;//���̴�С
	int margin_x;//��������϶
	int margin_y;//�����ϲ��϶
	float chessSize;//���Ӵ�С��������С��ͬ��
	vector<vector<int>> chessMap;//�洢��ǰ���� 1���ӣ�-1���ӣ�0�հ�

	bool playerFlag;//���ڸ�˭���壬true���� flase����

	void updataGameMap(ChessPos* pos);

	void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);

	bool checkWin();

	ChessPos lastPos;//��������λ��
};

