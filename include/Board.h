#ifndef BOARD_H
#define BOARD_H

#include "Primitive.h"
#include "CGFtexture.h"

#include <GL\glui.h>

class Cell;

class Board{

public:

	Board();
	~Board();
	void draw();

protected:
	Cell *tray[81];
	scene::Plane *sides[4];

private: 
	void drawAllCells();
};

class Cell{
public:

	Cell(int,int);
	~Cell();
	void draw();

protected:
	bool selected,piece;
	scene::Plane *plane;
	int posx,posy;
};

#endif