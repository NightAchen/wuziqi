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
		//��ȡ�������Ϣ
		msg = GetMouseMsg();
		//�ж������Ƿ���Ч���Լ����ӹ���
		if (msg.uMsg == WM_LBUTTONDOWN && chess->clickBorad(msg.x, msg.y, &pos))
		{
			break;
		}
	}
	chess->chessDown(&pos, CHESS_BLACK);
}
