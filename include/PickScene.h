#ifndef PICKSCENE_H
#define PICKSCENE_H

#include "CGFscene.h"
#include "Primitive.h"
#include "BoardTile.h"
#include "PickInterface.h"
#include "Cell.h"
#include "PConnect.h"
#include <string.h>
#include <stack>

class PickScene : public CGFscene
{
	friend PickInterface;
public:
	void init();
	void display();
	~PickScene();

	void initBoard();
	void startCon();

	Cell **cells;
	PConnect *con;

	string board;

private:
	CGFlight* light0;
	CGFobject* obj;
	scene::Primitive* p;
	BoardTile* tile;
	CGFappearance* materialAppearance;
	stack<std::string> *game_states;

};

string replace(string data);

#endif