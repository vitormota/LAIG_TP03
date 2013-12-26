#ifndef PIECE_H
#define PIECE_H

/*
	Defines a simple game piece
*/

#if _WIN32
#include <CGFobject.h>
#include <CGFappearance.h>
#include <CGFtexture.h>
#endif

//MAC OSX specific includes
#ifdef __APPLE__
#include "CGFobject.h"
#include "CGFappearance.h"
#include "CGFtexture.h"
#endif
//end


class Piece : public CGFobject{

public:

	Piece();
	Piece(int slices,int stacks,float radius);
	~Piece();
    void setXPos(int x);
    void setYPos(int y);
    int getXPos();
    int getYPos();
	
	void draw();
	char player;

	float elevation,radius,radius_2,height;
	int slices,stacks;
	CGFappearance *app;
	CGFtexture *text;
	GLUquadric *quad;
    
private:
    int xPos, yPos;

protected:

	void drawBase();

};

#endif