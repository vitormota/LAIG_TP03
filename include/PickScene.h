#ifndef PICKSCENE_H
#define PICKSCENE_H

#include "CGFscene.h"
#include "Primitive.h"
#include "BoardTile.h"
#include "PConnect.h"
#include "Piece.h"
#include "Animation.h"
#include "Ambient.h"
#include <string.h>
#include <stack>
#include <vector>

class Pair{
public:
	int x;
	int y;

	Pair():x(0),y(0){;}
	Pair(int x,int y):x(x),y(y){;}

	bool operator==(const Pair& a) const{
		return (x == a.x && y == a.y);
	}
	bool operator<(const Pair& cord) const{
		if (x == cord.x)
			return y < cord.y;
		return x < cord.x;
	}
	
};

class PickScene : public CGFscene
{

public:
	void init();
	void display();
	~PickScene();

	void initBoard();
	void startCon();

	PConnect *con;

	Piece *selected_piece;

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

	std::map<Pair,int> positions;

	bool movePiece(int xi, int yi,int xf,int yf);

	bool elevatePiece(int x,int y);
	bool un_elevatePiece();
	bool emptySpace(int x,int y);
    
    void drawBoardTile(int j);
    void drawPiece(int j);
    
    vector<Ambient*> ambients;
    Ambient* currentAmbient;

private:
	CGFlight* light0;
	CGFobject* obj;
	BoardTile* tile;
	CGFappearance* materialAppearance;
	stack<std::string> *game_states;

	void changePlayer();

	/*
	Called after a sucessfull move, checks if piece was remove (on plog) and if so remove it from board.
	*/
	void check_pieces();

	char player;

};

string replace(string data);

#endif