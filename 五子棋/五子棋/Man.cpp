#include "Man.h"

void Man::Init(Chess * chess)
{
	this->chess = chess;
}

void Man::go()
{
	MOUSEMSG msg;
	ChessPos pos;

	while (1)
	{
		//获取鼠标点击信息
		msg = GetMouseMsg();
		//判断落子是否有效，以及落子功能
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBorad(msg.x, msg.y, &pos))
		{
			break;
		}
	}
	chess->chessDown(&pos, CHESS_BLACK);
}
