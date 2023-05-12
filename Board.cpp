/*
 * Board.cpp
 *
 *  Created on: May 2, 2014
 *      Author: Sibt ul Hussain
 */

#include "Board.h"
#include <cstdio>

const int Board::BOARD_X = 28;
const int Board::BOARD_Y = 36;

// Destructor
Board::~Board(void) {
}

//Constructor
Board::Board(int xsize, int ysize) {
	xcellsize = xsize;
	ycellsize = ysize;
	lwidth = 3; // line width
	cwidth = 3; // curve widh
	/*pcolor = (ColorNames) 2;
	 bcolor = (ColorNames) 1;
	 gcolor = (ColorNames) 0;*/
	pcolor = CHOCOLATE;
	bcolor = DARK_BLUE;
	gcolor = PINK;
//set up board
}

void Board::Draw(void) {
	glColor3f(0, 0, 1);
	glPushMatrix();
//	glTranslatef(10, 10, 1);
//	glScalef(2.5, 2.5, 1);
//  glRotatef(10, 1, 0, 0);

    DrawRoundRect(xcellsize, ycellsize, 930, 550, colors[BLACK], 25); 	
	
	glPopMatrix();
}

//returns whether or not the board is open at the given coords
bool Board::IsOpen(int x, int y) {
//	if (board_array[y][x] > 0) {
	//	return false;
	//}
	return true;
}
