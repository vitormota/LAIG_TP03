#ifndef PICKSCENE_H
#define PICKSCENE_H

#include "CGFscene.h"
#include "Primitive.h"
#include "BoardTile.h"
#include "PickInterface.h"

class PickScene : public CGFscene
{
	friend PickInterface;
public:
	void init();
	void display();
	~PickScene();
private:
	CGFlight* light0;
	CGFobject* obj;
	scene::Primitive* p;
	BoardTile* tile;
	CGFappearance* materialAppearance;
};

#endif