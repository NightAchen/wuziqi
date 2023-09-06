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
		////���������
		//man->go();
		////�ж���Ϸ�Ƿ����
		//if (chess->checkOver())
		//{
		//	chess->Init();
		//	continue;
		//}

		//AI����
		ai->go2();
		//�ж���Ϸ�Ƿ����
		if (chess->checkOver())
		{
			chess->Init();
			continue;
		}

		//AI����
		ai->go();
		//�ж���Ϸ�Ƿ����
		if (chess->checkOver())
		{
			chess->Init();
			continue;
		}
	}
}
