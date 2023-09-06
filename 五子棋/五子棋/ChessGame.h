#pragma once
#include"AI.h"
#include"Chess.h"
#include"Man.h"


class ChessGame
{
public:
	ChessGame(Man *man, AI *ai, Chess *chess);

	void play();//开始对局

private:
	Man *man;
	AI *ai;
	Chess *chess;
};

