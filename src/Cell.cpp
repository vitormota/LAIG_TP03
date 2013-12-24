#include "Cell.h"

Cell::Cell(Piece *p,int x,int y){
	piece = p;
	this->x = x;
	this->y = y;
}

Cell::~Cell(){
	delete(piece);
}