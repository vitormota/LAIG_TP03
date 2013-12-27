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
    
    /* Connection */
	PConnect *con;
    void startCon();
    
    /* Ilumination */
    int* light_0;
    int* light_1;
    int* light_2;
    int* light_3;
    
    void changeLight0();
    void changeLight1();
    void changeLight2();
    void changeLight3();
    
    /* Animation */
    Animation* anim;
	float previousTime;
    bool animatePiece;
	void update(unsigned long time);
    
    /* Ambient */
    vector<Ambient*> ambients;
    Ambient* currentAmbient;
    Primitive* boardBase;
    
    /* Game */
    Piece* p;
    Piece *selected_piece;
	bool pickPiece; // true if picking is active for pieces
	bool pickBoardTile; // true if picking is active for board tiles
	std::map<Pair,int> positions;
    string board;
	vector<Piece*> boardPieces;
	vector<BoardTile*> boardTiles;
    
    void initBoard();
	bool movePiece(int xi, int yi, int xf, int yf);
	bool elevatePiece(int x, int y);
	bool un_elevatePiece();
	bool emptySpace(int x,int y);
    
    void drawBoardTile(int j);
    void drawPiece(int j);
    
    void getMessage(); // get message about the game to show (example: "Suedes" "Movie playing..." "Suedes won" "King captured" etc etc etc)
    void undo();
    void playMovie();
    void changeAmbient(int ambientID);
    void changeGameMode(int modeID);
    void changeCamera(int viewID);
    void changeTimeToPlay(string timeID);
    
    
private:
	CGFlight* light0;
	CGFlight* light1;
    CGFlight* light2;
    CGFlight* light3;
	CGFobject* obj;
	BoardTile* tile;
	stack<std::string> *game_states;
    
	void changePlayer();
    
	/*
     Called after a sucessfull move, checks if piece was remove (on plog) and if so remove it from board.
     */
	void check_pieces();
    
	char player; // current player playing the game
    
};

string replace(string data);

#endif