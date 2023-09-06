#pragma once
#include"Chess.h"


class Man
{
public:
	void Init(Chess *chess);
	void go();

private:
	Chess *chess;
};

