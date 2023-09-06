#pragma once
#include "Chess.h"
class AI
{
public:
	void Init(Chess *chess);
	void go();
	void go2();

private:
	Chess* chess;
	vector<vector<int>> scoreMap;

private:
	void calculateScore();
	ChessPos think();
};

