#include <iostream>
#include"ChessGame.h"
using namespace std;



int main(void)
{
	Man man;
	AI ai;
	Chess chess(13,44,43,67.3);
	ChessGame game(&man, &ai, &chess);
	game.play();

	return 0;
}