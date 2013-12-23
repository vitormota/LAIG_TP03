#include "Board.h"

Board::Board(){
	int j =0;
	for(int i = 0; i < 81;i++){
		tray[i] = new Cell(j % 9, i % 9);
		if(i % 9 == 0) j++;
	}
}

Board::~Board(){

}

void Board::draw(){
	drawAllCells();
}

void Board::drawAllCells(){
	for(Cell *c : tray){
		c->draw();
	}
}


Cell::Cell(int x,int y){
	posx = x;
	posy = y;
	plane = new scene::Plane("",50);
}

Cell::~Cell(){

}

void Cell::draw(){
	float alpha;
	if( selected )
		alpha = 0.3;
	else
		alpha = 0;
	glPushMatrix();
	if(!piece)
		glColor4f(0,1,0,alpha);
	else
		glColor4f(1,0,0,alpha);
	glTranslatef(posx -4,0.20,posy-4);
	glLoadName(posy);
	glPushName(posx);
	plane->draw();
	glPopName();
	glPopMatrix();
}