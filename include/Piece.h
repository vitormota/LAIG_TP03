#ifndef PIECE_H
#define PIECE_H

/*
 Defines a simple game piece
 */

#include <map>
#include "Primitive.h"

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
	Piece(int slices, int stacks, float radius);
	~Piece();
	void setXPos(int x);
	void setYPos(int y);
	void setCurrentX(double currentX);
	void setCurrentY(double currentY);
	int getXPos();
	int getYPos();
	
	void elevate();
	void lower();
	void draw();
	
	float radius, radius_2, height;
	int slices,stacks;
	int xPos, yPos;
	int type;
	bool king;
	bool animating;
	char player;
	bool exists;
	bool captured;
	string id;
	GLUquadric *quad;
	Primitive* firstC;
	Primitive* secondC;
	Primitive* sph;
	Primitive* bigsph;
	Primitive* tor;
	
private:
	double currentX, currentY;
	
	
protected:
	void drawBase();
	float elevation;
	
};
#endif