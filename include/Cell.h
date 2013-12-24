#ifndef CELL_H
#define CELL_h

#include "Piece.h"

class Cell{

public:

	Cell(){;}
	Cell(Piece *p,int x, int y);
	~Cell();

	int x,y;
	Piece *piece;

protected:
};

#endif