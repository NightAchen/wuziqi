#include "ChessGame.h"

ChessGame::ChessGame(Man * man, AI * ai, Chess * chess)
{
	this->man = man;
	this->chess = chess;
	this->ai = ai;

	man->Init(chess);
	ai->Init(chess);
}

void ChessGame::play()
{
	chess->Init();
	while (1)
	{
		////玩家先落子
		//man->go();
		////判断游戏是否结束
		//if (chess->checkOver())
		//{
		//	chess->Init();
		//	continue;
		//}

		//AI落子
		ai->go2();
		//判断游戏是否结束
		if (chess->checkOver())
		{
			chess->Init();
			continue;
		}

		//AI落子
		ai->go();
		//判断游戏是否结束
		if (chess->checkOver())
		{
			chess->Init();
			continue;
		}
	}
}
