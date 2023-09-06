#pragma once
#include"graphics.h"//图形库
#include<vector>
using namespace std;


struct ChessPos
{
	int row;
	int col;

	ChessPos(int r=0,int c=0):row(r),col(c){}
};
//白棋			//黑棋
using chess_kind_t = enum { CHESS_WHITE = -1, CHESS_BLACK = 1 };

class Chess
{
public:
	Chess(int gradeSize, int margin_x, int margin_y, float chessSize);

	void Init();

	//鼠标点击坐标
	bool clickBorad(int x, int y, ChessPos *pos);

	//落子位置
	void chessDown(ChessPos *pos, chess_kind_t kind);

	//获取棋盘大小(13,15,19)
	int getGradeSize();

	//获取指定位置是白棋还是黑棋，或者空白
	int getChessData(ChessPos *pos);
	int getChessData(int row, int col);

	//棋局是否结束
	bool checkOver();

	//使用图形库easyx来显示棋盘和棋子
private:
	IMAGE chessBlackImg;//黑子
	IMAGE chessWhiteImg;//白子
	int gradeSize;//棋盘大小
	int margin_x;//棋盘左侧空隙
	int margin_y;//棋盘上侧空隙
	float chessSize;//棋子大小（与棋格大小相同）
	vector<vector<int>> chessMap;//存储当前棋子 1黑子，-1白子，0空白

	bool playerFlag;//现在该谁下棋，true黑子 flase白子

	void updataGameMap(ChessPos* pos);

	void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);

	bool checkWin();

	ChessPos lastPos;//最后的落子位置
};

