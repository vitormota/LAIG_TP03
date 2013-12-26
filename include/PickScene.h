#ifndef PICKSCENE_H
#define PICKSCENE_H

#include "CGFscene.h"
#include "Primitive.h"
#include "BoardTile.h"
#include "PickInterface.h"
#include "PConnect.h"
#include "Piece.h"
#include "Animation.h"
#include <string.h>
#include <stack>
#include <vector>

class PickScene : public CGFscene
{
	friend PickInterface;
public:
	void init();
	void display();
	~PickScene();

	void initBoard();
	void startCon();

	PConnect *con;

	string board;
    vector<Piece*> boardPieces;
    vector<BoardTile*> boardTiles;
    
    Animation* anim;
    float previousTime;
    void update(unsigned long time);
    
    Piece* p;
    bool animatePiece;
    
    bool pickPiece; // true if picking is active for pieces
    bool pickBoardTile; // true if picking is active for board tiles

private:
	CGFlight* light0;
	CGFobject* obj;
	BoardTile* tile;
	CGFappearance* materialAppearance;
	stack<std::string> *game_states;

};

string replace(string data);

#endif