#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "CGFscene.h"
#include "CGFcamera.h"
#include "Primitive.h"
#include "BoardTile.h"
#include "PConnect.h"
#include "Piece.h"
#include "Ambient.h"
//#include "Rect.h"
#include <string.h>
#include <stack>
#include <vector>

enum game_mode{
	pvp,pvm,pvs
};

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

typedef struct Move{
	int xi,xf,yi,yf;
	Piece *selected;
};

class GameScene : public CGFscene
{

public:
	void init();
	void display();
	~GameScene();

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
	bool animatePiece;
	void pieceAnimation(int xi, int yi, int xf, int yf, int dx, int dy);
	double dx, dy, currentX, currentY;

	/* Ambient */
	vector<Ambient*> ambients;
	Ambient* currentAmbient;

	bool pause;
	bool movie;

	int time;

	/* Game */
	Piece* p;
	Piece* selected_piece;
	bool pickPiece; // true if picking is active for pieces
	bool pickBoardTile; // true if picking is active for board tiles
	bool game_ended;
	std::map<Pair,int> positions;
	string board;
	vector<Piece*> boardPieces;
	vector<Move> moves;
	vector<BoardTile*> boardTiles;
	CGFcamera* defaultCamera;
	CGFcamera* currentCamera;
	game_mode gm; //game mode
	int play_time;
	unsigned long elapse_time;
	unsigned long movie_step;
	void initBoard();
	bool movePiece(int xi, int yi, int xf, int yf);
	bool elevatePiece(int x, int y);
	bool lowerPiece();
	bool emptySpace(int x,int y);
	bool isGameFinished();

	void drawBoardTile(int j);
	void drawPiece(int j);

	char message[20]; // message with information about the game
	char* getMessage(); // get message about the game to show (example: "Suedes" "Movie playing..." "Suedes won" "King captured" etc etc etc)
	void undo();
	void playMovie();
	void changeAmbient(int ambientID);
	void changeGameMode(int modeID);
	void changeCamera(int viewID);
	void changeTimeToPlay(int timeID);
	void aiMove();
	virtual void update(unsigned long);
	Piece* getPiece(int x,int y);
	void restart();

private:
	CGFlight* light0;
	CGFlight* light1;
	CGFlight* light2;
	CGFlight* light3;
	BoardTile* tile;
	stack<std::string> *game_states;
	stack<std::string> *movie_boards;
	void changePlayer();
	Piece* getPiece(string id);
	void redrawBoard();
	void dumpPositions();

	/*
	Called after a sucessfull move, checks if piece was remove (on plog) and if so remove it from board.
	*/
	void check_pieces();

	char turn; // current player playing the game

};

string replace(string data);

#endif