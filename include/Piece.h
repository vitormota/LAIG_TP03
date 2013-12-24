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


class Piece : public CGFobject{

public:

	Piece();
	Piece(int slices,int stacks,float radius);
	~Piece();
	
	void draw();
	char player;

	float elevation,radius,radius_2,height;
	int slices,stacks;
	CGFappearance *app;
	CGFtexture *text;
	GLUquadric *quad;

protected:

	void drawBase();

};

#endif