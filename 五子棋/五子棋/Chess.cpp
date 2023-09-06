#include "Chess.h"
#include<mmsystem.h>
#include<math.h>
#include<conio.h>
#pragma comment(lib,"winmm.lib")

// 载入PNG图并去透明部分
void Chess::drawAlpha(IMAGE * picture, int picture_x, int picture_y)
{
	// 变量初始化
	DWORD *dst = GetImageBuffer();    // GetImageBuffer()函数，用于获取绘图设备的显存指针，EASYX自带
	DWORD *draw = GetImageBuffer();
	DWORD *src = GetImageBuffer(picture); //获取picture的显存指针
	int picture_width = picture->getwidth(); //获取picture的宽度，EASYX自带
	int picture_height = picture->getheight(); //获取picture的高度，EASYX自带
	int graphWidth = getwidth();       //获取绘图区的宽度，EASYX自带
	int graphHeight = getheight();     //获取绘图区的高度，EASYX自带
	int dstX = 0;    //在显存里像素的角标

	// 实现透明贴图 公式： Cp=αp*FP+(1-αp)*BP ， 贝叶斯定理来进行点颜色的概率计算
	for (int iy = 0; iy < picture_height; iy++)
	{
		for (int ix = 0; ix < picture_width; ix++)
		{
			int srcX = ix + iy * picture_width; //在显存里像素的角标
			int sa = ((src[srcX] & 0xff000000) >> 24); //0xAArrggbb;AA是透明度
			int sr = ((src[srcX] & 0xff0000) >> 16); //获取RGB里的R
			int sg = ((src[srcX] & 0xff00) >> 8);   //G
			int sb = src[srcX] & 0xff;              //B
			if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
			{
				dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; //在显存里像素的角标
				int dr = ((dst[dstX] & 0xff0000) >> 16);
				int dg = ((dst[dstX] & 0xff00) >> 8);
				int db = dst[dstX] & 0xff;
				draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  //公式： Cp=αp*FP+(1-αp)*BP  ； αp=sa/255 , FP=sr , BP=dr
					| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         //αp=sa/255 , FP=sg , BP=dg
					| (sb * sa / 255 + db * (255 - sa) / 255);              //αp=sa/255 , FP=sb , BP=db
			}
		}
	}
}


Chess::Chess(int gradeSize, int margin_x, int margin_y, float chessSize)
{
	this->gradeSize = gradeSize;
	this->margin_x = margin_x;
	this->margin_y = margin_y;
	this->chessSize = chessSize;
	playerFlag = CHESS_BLACK;

	//初始化，把存储棋子信息的容器清零
	for (int i = 0; i < gradeSize; i++)
	{
		vector<int> row;
		for (int j = 0; j < gradeSize; j++)
		{
			row.push_back(0);
		}
		chessMap.push_back(row);
	}
}

void Chess::Init()
{
	//创建游戏窗口
	initgraph(897,895);

	//显示我们的棋盘图片
	loadimage(0,"res/棋盘2.jpg");

	//播放开始提示音
	mciSendString("play res/start.wav",0,0,0);

	//加载黑棋和白旗图片
	loadimage(&chessBlackImg, "res/black1.jpg",chessSize,chessSize,true);
	loadimage(&chessWhiteImg, "res/white1.jpg",chessSize,chessSize,true);

	//棋盘清零
	for (int i = 0; i < chessMap.size(); i++)
	{
		for (int j = 0; j < chessMap[i].size(); j++)
		{
			chessMap[i][j] = 0;
		}
	}

	playerFlag = true;
}

//鼠标点击位置减去边界的距离，除以每个格子的宽度，就等于第几行，求列也是同理
bool Chess::clickBorad(int x, int y, ChessPos * pos)
{
	//1.鼠标点击位置，减去边界距离，再对格子宽度取整，就得到为第几列，第几行
	int row = (y - margin_y) / chessSize;
	int col = (x - margin_x) / chessSize;

	//2.根据上一步结果*chessSize + 边界距离即得到棋子应该落子的位置
	int leftTopPosX = margin_x + col * chessSize;
	int leftTopPosY = margin_y + row * chessSize;
	
	bool res = false;
	int len;
	//允许误差
	int offset = chessSize * 0.4;

	//判断棋子落子位置离左上角，右上角，左下角和右下角那个距离最近
	do
	{
		//左上角判断
		len = sqrt((x - leftTopPosX)*(x - leftTopPosX) + (y - leftTopPosY)*(y - leftTopPosY));
		if (len < offset)
		{
			pos->col = col;
			pos->row = row;
			if (chessMap[pos->row][pos->col] == 0)
			{
				res = true;
			}
			break;
		}

		//右上角判断
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset)
		{
			pos->col = col+1;
			pos->row = row;
			if (chessMap[pos->row][pos->col] == 0)
			{
				res = true;
			}
			break;
		}

		//左下角判断
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset)
		{
			pos->col = col;
			pos->row = row+1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				res = true;
			}
			break;
		}


		//右下角判断
		len = sqrt((x - leftTopPosX - chessSize) * (x - leftTopPosX - chessSize) + (y - leftTopPosY - chessSize) * (y - leftTopPosY - chessSize));
		if (len < offset)
		{
			pos->col = col+1;
			pos->row = row+1;
			if (chessMap[pos->row][pos->col] == 0)
			{
				res = true;
			}			
			break;
		}
	} while (0);	
	return res;
}


void Chess::chessDown(ChessPos* pos, chess_kind_t kind)
{
	mciSendString("play res/down7.WAV", 0, 0, 0);
	//落子位置 -0.5是因为插入的图片在最终位置的左上半个chessSize的位置
	int x = margin_x + chessSize * (pos->col - 0.5);
	int y = margin_y + chessSize * (pos->row - 0.5);

	if (kind == CHESS_WHITE)
	{
		//drawAlpha(&chessWhiteImg, x, y);
		putimage(x, y, &chessWhiteImg);
	}
	else
	{
		//drawAlpha(&chessBlackImg, x, y);
		putimage(x, y, &chessBlackImg);
	}
	updataGameMap(pos);
}

int Chess::getGradeSize()
{
	return gradeSize;
}

int Chess::getChessData(ChessPos * pos)
{
	return chessMap[pos->row][pos->col]; 
}

int Chess::getChessData(int row, int col)
{
	return chessMap[row][col];
}

bool Chess::checkOver()
{
	if (checkWin())
	{
		Sleep(1500);
		if (playerFlag == false)//说明上一步棋是黑旗，所以黑旗胜利
		{
			mciSendString("play res/不错.mp3", 0, 0, 0);
			loadimage(0, "res/胜利.jpg");
		}
		else
		{
			mciSendString("play res/失败.mp3", 0, 0, 0);
			loadimage(0, "res/失败.jpg");
		}
		_getch();//暂停
		return true;
	}
	
	return false;
}

bool Chess::checkWin()
{
	int row = lastPos.row;
	int col = lastPos.col;

	//落子点水平方向判断是否赢棋
	for (int i = 0; i < 5; i++)
	{
		// 往左5个，往右匹配4个子，20种情况
		if (col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chessMap[row][col - i] == chessMap[row][col - i + 1] &&
			chessMap[row][col - i] == chessMap[row][col - i + 2] &&
			chessMap[row][col - i] == chessMap[row][col - i + 3] &&
			chessMap[row][col - i] == chessMap[row][col - i + 4])
			return true;
	}

	// 竖直方向(上下延伸4个)
	for (int i = 0; i < 5; i++)
	{
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			chessMap[row - i][col] == chessMap[row - i + 1][col] &&
			chessMap[row - i][col] == chessMap[row - i + 2][col] &&
			chessMap[row - i][col] == chessMap[row - i + 3][col] &&
			chessMap[row - i][col] == chessMap[row - i + 4][col])
			return true;
	}

	// “/"方向
	for (int i = 0; i < 5; i++)
	{
		if (row + i < gradeSize &&
			row + i - 4 >= 0 &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			// 第[row+i]行，第[col-i]的棋子，与右上方连续4个棋子都相同
			chessMap[row + i][col - i] == chessMap[row + i - 1][col - i + 1] &&
			chessMap[row + i][col - i] == chessMap[row + i - 2][col - i + 2] &&
			chessMap[row + i][col - i] == chessMap[row + i - 3][col - i + 3] &&
			chessMap[row + i][col - i] == chessMap[row + i - 4][col - i + 4])
			return true;
	}

	// “\“ 方向
	for (int i = 0; i < 5; i++)
	{
		// 第[row+i]行，第[col-i]的棋子，与右下方连续4个棋子都相同
		if (row - i >= 0 &&
			row - i + 4 < gradeSize &&
			col - i >= 0 &&
			col - i + 4 < gradeSize &&
			chessMap[row - i][col - i] == chessMap[row - i + 1][col - i + 1] &&
			chessMap[row - i][col - i] == chessMap[row - i + 2][col - i + 2] &&
			chessMap[row - i][col - i] == chessMap[row - i + 3][col - i + 3] &&
			chessMap[row - i][col - i] == chessMap[row - i + 4][col - i + 4])
			return true;
	}

	return false;
}

void Chess::updataGameMap(ChessPos * pos)
{
	lastPos = *pos;
	chessMap[pos->row][pos->col] = playerFlag ? CHESS_BLACK : CHESS_WHITE;
	playerFlag = !playerFlag;//交换黑白
}

