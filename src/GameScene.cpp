#if _WIN32
#include <GL\glew.h>
#include <GL\glui.h>
#endif

#include "CGFaxis.h"
#include "CGFappearance.h"

#include "Primitive.h"
#include "GameScene.h"

#include <math.h>
#include <cstring>

using namespace std;

#define NUM_OBJS 25
#define NUM_ROWS 9
#define NUM_COLS 9
#define BOARD_TILE_SIDE_SIZE 3

typedef map<Pair,int>::iterator Iter;

class PrologBadResponse{};

void GameScene::init()
{
	movie_step = 1000;
	elapse_time = 0;
	play_time = 20000;
	turn = 'm';
	pause = true;
	gm = pvp;
	game_ended = false;
	movie = false;
	movie_boards = new stack<string>;
	time = 0;
	game_states = new stack<string>;
    playing = false;

	// Apply transformations corresponding to the camera position relative to the origin

	// normal init, no changes needed

	// Enables lighting computations
	glEnable(GL_LIGHTING);

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light

	// Defines a default normal
	glNormal3f(0,0,1);

	/* Ilumination */
	light_0 = new int;
	light_1 = new int;
	light_2 = new int;
	light_3 = new int;

	*light_0 = 1;
	*light_1 = 1;
	*light_2 = 1;
	*light_3 = 1;

	// Declare and enable lights
	float light0_pos[4] = {-6.0, 6.0, -6.0, 1.0};
	float light1_pos[4] = {6.0, 6.0, 6.0, 1.0};
	float light2_pos[4] = {6.0, 6.0, -6.0, 1.0};
	float light3_pos[4] = {-6.0, 6.0, 6.0, 1.0};

	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light1 = new CGFlight(GL_LIGHT1, light1_pos);
	light2 = new CGFlight(GL_LIGHT2, light2_pos);
	light3 = new CGFlight(GL_LIGHT3, light3_pos);

	light0->enable();
	light1->enable();
	light2->enable();
	light3->enable();

	/* Animation */

	// Initialize update period and previousTime
	setUpdatePeriod(100);
	animatePiece = false;
	currentX = 0;
	currentY = 0;
    currentZ = 0;

	/* Ambients */

	// Marble ambient

#ifdef __APPLE__
	Ambient* marbleAmbient = new Ambient("MARBLE");
	marbleAmbient->setTextures("/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_MARBLE.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_MARBLE.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_MARBLE.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/SKY_MARBLE.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/G_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_WOOD.jpg");

	marbleAmbient->setPiecesType(0);
	ambients.push_back(marbleAmbient);


	// Jewels Ambient
	Ambient* jewelsAmbient = new Ambient("JEWELS");
	jewelsAmbient->setTextures("/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_JEWELS.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_JEWELS.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLUE_JEWELS.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/SKY_JEWELS.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/G_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_WOOD.jpg");

	jewelsAmbient->setPiecesType(1);
	ambients.push_back(jewelsAmbient);

	// Fabric Ambient
	Ambient* fabricAmbient = new Ambient("FABRIC");
	fabricAmbient->setTextures("/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_FABRIC.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_FABRIC.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/FLOWERS_FABRIC.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/SKY_FABRIC.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/G_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_WOOD.jpg",
		"/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_WOOD.jpg");
#else

	Ambient* marbleAmbient = new Ambient("MARBLE");
	marbleAmbient->setTextures("data/RED_MARBLE.jpg",
		"data/WHITE_MARBLE.jpg",
		"data/BLACK_MARBLE.jpg",
		"data/SKY_MARBLE.jpg",
		"data/G_WOOD.jpg",
		"data/WHITE_WOOD.jpg",
		"data/BLACK_WOOD.jpg",
		"data/RED_WOOD.jpg");

	marbleAmbient->setPiecesType(0);
	ambients.push_back(marbleAmbient);


	// Jewels Ambient
	Ambient* jewelsAmbient = new Ambient("JEWELS");
	jewelsAmbient->setTextures("data/RED_JEWELS.jpg",
		"data/WHITE_JEWELS.jpg",
		"data/BLUE_JEWELS.jpg",
		"data/SKY_JEWELS.jpg",
		"data/G_WOOD.jpg",
		"data/WHITE_WOOD.jpg",
		"data/BLACK_WOOD.jpg",
		"data/RED_WOOD.jpg");

	jewelsAmbient->setPiecesType(1);
	ambients.push_back(jewelsAmbient);

	// Fabric Ambient
	Ambient* fabricAmbient = new Ambient("FABRIC");
	fabricAmbient->setTextures("data/RED_FABRIC.jpg",
		"data/WHITE_FABRIC.jpg",
		"data/FLOWERS_FABRIC.jpg",
		"data/SKY_FABRIC.jpg",
		"data/G_WOOD.jpg",
		"data/WHITE_WOOD.jpg",
		"data/BLACK_WOOD.jpg",
		"data/RED_WOOD.jpg");

#endif

	fabricAmbient->setPiecesType(2);
	ambients.push_back(fabricAmbient);


	currentAmbient = marbleAmbient; // default ambient

	/* Interface */
	message[0] = 'W';
	message[1] = 'e';
	message[2] = 'l';
	message[3] = 'c';
	message[4] = 'o';
	message[5] = 'm';
	message[6] = 'e';

	for(int i = 7; i < 19; i++)
	{
		message[i] = ' ';
	}

	message[19] = '\0';

	/* Cameras */
	defaultCamera = activeCamera;

	// Create board tiles
	for (int y = 0; y < NUM_ROWS; y++)
	{
		for (int x = 0; x < NUM_COLS; x++)
		{
			BoardTile* tile = new BoardTile();
			tile->setXPos(x);
			tile->setYPos(y);
			this->boardTiles.push_back(tile);
		}
	}

	//request server init board and parse it
	initBoard();

}

void GameScene::display()
{

	if(game_ended){
		strcpy(message,"Game Over");
	}

	// TEST THE PICKING -> To switch from picking the board tiles or the board pieces
	this->pickBoardTile = true;
	this->pickPiece = true;

	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	activeCamera->applyView();

	// Draw (and update) lights
	light0->draw();
	light1->draw();
	light2->draw();
	light3->draw();

	// Draw axis
	//axis.draw();

	// Draw ambient sky
	currentAmbient->skyApp->apply();
	currentAmbient->drawSky();

	if(currentAmbient->getPiecesType() == 2)
	{
		currentAmbient->kApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
		currentAmbient->sApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
		currentAmbient->mApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
	}

	// Set pieces type
	for(int p = 0; p < boardPieces.size(); p++)
	{
		boardPieces[p]->type = currentAmbient->getPiecesType();
	}

	// ---- END Background, camera and axis setup


	// ---- BEGIN feature demos

	//// scale down a bit
	glPushMatrix();
	glScalef(0.5, 0.5, 0.5);

	// Picking only the pieces
	if((this->pickPiece == true) && (this->pickBoardTile == false))
	{
		for(int i = 0; i < boardPieces.size();i++)
		{
			glPushName(boardPieces[i]->getYPos());
			glPushName(boardPieces[i]->getXPos());

			if(boardPieces[i]->exists)
			{
				drawPiece(i);
			}

			glPopName();
			glPopName();

		}
	}
	// Picking only the board tiles
	else if((this->pickPiece == false) && (this->pickBoardTile == true))
	{
		for(int i = 0; i < boardTiles.size();i++)
		{
			glPushName(boardTiles[i]->getYPos());
			glPushName(boardTiles[i]->getXPos());

			drawBoardTile(i);

			glPopName();
			glPopName();

		}

		for(int j = 0; j < boardPieces.size();j++)
		{
			if(boardPieces[j]->exists)
			{
				drawPiece(j);
			}
		}
	}
	// Picking pieces and board tiles
	else if((this->pickBoardTile == true) && (this->pickPiece == true))
	{
		for(int i = 0; i < boardPieces.size();i++)
		{
			glPushName(boardPieces[i]->getYPos());
			glPushName(boardPieces[i]->getXPos());

			if(boardPieces[i]->exists)
			{
				drawPiece(i);
			}

			glPopName();
			glPopName();
		}

		for(int j = 0; j < boardTiles.size();j++)
		{
			glPushName(boardTiles[j]->getYPos());
			glPushName(boardTiles[j]->getXPos());

			drawBoardTile(j);

			glPopName();
			glPopName();
		}

	}
	// Only draw the board without picking
	else{
		for(int i = 0; i < boardPieces.size();i++)
		{
			if(boardPieces[i]->exists)
			{
				drawPiece(i);
			}
		}

		for(int j = 0; j < boardTiles.size();j++)
		{
			drawBoardTile(j);
		}

	}

	glPopMatrix();

	// ---- END feature demos

	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

void GameScene::pieceAnimation(int xi, int yi, int xf, int yf, int dx, int dy)
{
	animatePiece = true;
	int pos = 0;

	for(int j = 0; j < boardPieces.size(); j++)
	{
		if((boardPieces[j]->getXPos() == xi) && (boardPieces[j]->getYPos() == yi))
		{
			pos = j;
			break;
		}
	}

	if((animatePiece == false) || (((currentX < xf*9/3-13.5+1.5) && (dx < 0)) || ((currentX > xf*9/3-13.5+1.5) && (dx > 0)) || ((currentY < yf*9/3-13.5+1.5) && (dy < 0)) || ((currentY > yf*9/3-13.5+1.5) && (dy > 0))))
	{
		animatePiece = false;
		boardPieces[pos]->animating = false;
	}
	else{
		currentX = currentX + (dx/30.0);
		currentY = currentY + (dy/30.0);

		boardPieces[pos]->setCurrentX(currentX);
		boardPieces[pos]->setCurrentY(currentY);
		boardPieces[pos]->animating = true;
	}
}

GameScene::~GameScene()
{
	delete(p);
	delete(selected_piece);
	delete(game_states);
	delete(tile);
	delete(light0);
	delete(light1);
	delete(light2);
	delete(light3);

}

void GameScene::initBoard(){
	board = con->get_init_board();
	redrawBoard();
}

void GameScene::aiMove(){
	if(game_ended) return;
	string player;
	switch(gm){
	case pvp:
		return;
	case pvs:
		if(turn == 'm') return;
		player = "swedish";
		break;
	case pvm:
		if(turn == 's') return;
		player = "moscov";
		break;
	}
	string newBoard;
	int xi,xf,yi,yf;
	con->calculatePlay(player,board,xi,yi,xf,yf,newBoard);
	if(strcmp(newBoard.c_str(),"invalid")==0){
		//something went wrong so quit and exit
		throw PrologBadResponse();
	}else{
		Move m;
		m.xi = xi;
		m.xf = xf;
		m.yi = yi;
		m.yf = yf;
		m.selected = getPiece(xi,yi);
		if(!movie) moves.push_back(m);
		strcpy(message,"PC has some moves...");
		//apply changes
		Pair p(xi,yi);
		Pair pf(xf,yf);

		// animation
		dx = xf - xi;
		dy = yf - yi;

		animatePiece = true;

		currentX = xi*9/3-13.5+1.5;
		currentY = yi*9/3-13.5+1.5;

		while(animatePiece)
		{
			pieceAnimation(xi, yi, xf, yf, dx, dy);
			
            if(animatePiece != false)
            {
            display();
            }
		}

		int pos = positions[p];
		boardPieces[pos]->xPos = xf;
		boardPieces[pos]->yPos = yf;
		positions.erase(p);
		positions[pf] = pos;
		if(!movie) game_states->push(board);
		board = newBoard;
		check_pieces();
		changePlayer();
	}
	if(!movie) game_ended = isGameFinished();
}

void GameScene::drawBoardTile(int j)
{
	// apply appearances
	// rei -> 40
	// m ->  3,  4,  5, 13, 27, 35, 36, 37, 43, 44, 45, 53, 67, 75, 76, 77
	// s -> 22, 31, 38, 39, 41, 42, 49, 58

	if((j == 3) || (j == 4) || (j == 5) || (j == 13) || (j == 27) || (j == 35) || (j == 36) || (j == 37) || (j == 43) || (j == 44) || (j == 45) || (j == 53) || (j == 67) || (j == 75) || (j == 76) || (j == 77))
	{
		currentAmbient->tileMApp->apply();
	}
	else if((j == 22) || (j == 31) || (j == 38) || (j == 39) || (j == 41) || (j == 42) || (j == 49) || (j == 58))
	{
		currentAmbient->tileSApp->apply();
	}
	else if(j == 40)
	{
		currentAmbient->tileKApp->apply();
	}
	else
	{
		currentAmbient->tileGApp->apply();
	}

	boardTiles[j]->draw();

}

void GameScene::drawPiece(int i)
{

	// apply appearances
	if(boardPieces[i]->king)
	{
		currentAmbient->kApp->apply();
	}
	else
		if(boardPieces[i]->player == 's')
		{
			currentAmbient->sApp->apply();
		}
		else
			if(boardPieces[i]->player == 'm')
			{
				currentAmbient->mApp->apply();
			}
			
    boardPieces[i]->draw();

}

bool GameScene::lowerPiece(){
	if(selected_piece){
        
        // animation
        if(selected_piece->type == 0)
        {
			dz = selected_piece->elevation;
        }
        else
			{
                dz = selected_piece->elevation;
			}
        
        currentZ = dz;
        elevate = true;
        
        while(elevate)
        {
            
			if((elevate == false) || (currentZ < 0.0))
			{
                elevate = false;
                selected_piece->elevating = false;
                currentZ = 0.0;
			}
			else{
                currentZ = currentZ - (dz/10.0);
                
                selected_piece->setCurrentZ(currentZ);
                selected_piece->elevating = true;
                
                if(elevate != false)
                {
                display();
                }
			}
        }
		selected_piece->lower();
		return true;
	}
	return false;
}

bool GameScene::elevatePiece(int x,int y){
	bool res = false;
	int pos = -1;
	Pair p;
	p.x = x;
	p.y = y;
	map<Pair,int>::iterator it = positions.find(p);
	if(it != positions.end()){
		pos = it->second;
		Piece *p = boardPieces[pos];
		if(p && p->player == turn){
			res = true;
			selected_piece = p;
            dz = 0.0;
			// animation
			if(p->type == 0)
			{
			dz = 1.0;
			}
			else
			if(p->type == 1)
			{
			dz = p->radius + p->elevation;
			}
			else
			if(p->type == 2)
			{
                dz = p->elevation + 0.5;
			}

			elevate = true;

			currentZ = 0.0;

			while(elevate)
			{

			if((elevate == false) || (currentZ > dz))
			{
			elevate = false;
			boardPieces[pos]->elevating = false;
			currentZ = 0.0;
			}
			else{
			currentZ = currentZ + (dz/10.0);

			boardPieces[pos]->setCurrentZ(currentZ);
			boardPieces[pos]->elevating = true;

			display();
			}
            }

			p->elevate();
		}
	}
	return res;
}

bool GameScene::emptySpace(int x,int y){
	Pair p(x,y);
	if(positions.find(p)!=positions.end()){
		return false;
	}
	return true;
}

bool GameScene::movePiece(int xi,int yi,int xf,int yf){
	if(game_ended) return false;
	bool success = false;
	string newBoard;
	if(con->play(xi,yi,xf,yf,board,newBoard)){
		Move m;
		m.xi = xi;
		m.xf = xf;
		m.yi = yi;
		m.yf = yf;
		m.selected = selected_piece;
		if(!movie) moves.push_back(m);
		success = true;

		Pair p(xi,yi);
		Pair pf(xf,yf);

		// animation
		dx = xf - xi;
		dy = yf - yi;

		animatePiece = true;

		currentX = xi*9/3-13.5+1.5;
		currentY = yi*9/3-13.5+1.5;

		while(animatePiece)
		{
			pieceAnimation(xi, yi, xf, yf, dx, dy);
			
            if(animatePiece != false)
            {
            display();
            }
		}
        
		int pos = positions[p];
		positions.erase(p);
		positions[pf] = pos;
		selected_piece->xPos = xf;
		selected_piece->yPos = yf;
		//printf("%i,%i",selected_piece->xPos,selected_piece->yPos);
        lowerPiece();
		selected_piece = NULL;
		game_states->push(board);
		board = newBoard;
		check_pieces();
		changePlayer();
		aiMove();
		dumpPositions();
	}
	if(!movie) game_ended = isGameFinished();
	return success;
}

bool GameScene::isGameFinished(){
    
     playing = false;
     strcpy(message,"Game Over          ");
    
	return con->gameEnd(board);
}

void GameScene::check_pieces(){
	string id;
	Piece *p;
	for(int i = 0; i < boardPieces.size(); i++){
		p = boardPieces[i];
		if(board.find(p->id.c_str()) == string::npos){
			p->exists = false;
			strcpy(message,"In your Face!!");
			Pair loc(p->xPos,p->yPos);
			positions.erase(loc);
		}
	}
}

void GameScene::changePlayer(){
	
    playing = true;
    if(turn == 'm'){
		turn = 's';
	}else{
		turn = 'm';
	}
	elapse_time = 0;
}

string replace(string data){
	string res = data;
	for(int i = 0; i < data.length() ; i++){
		if(res[i]=='[' || res[i] == ']'){
			res[i] = ' ';
		}
	}
	return res;
}

void GameScene::changeLight0()
{
	if(*light_0 == 1)
	{
		light0->enable();
	}
	else
	{
		light0->disable();
	}
}

void GameScene::changeLight1()
{
	if(*light_1 == 1)
	{
		light1->enable();
	}
	else
	{
		light1->disable();
	}
}

void GameScene::changeLight2()
{
	if(*light_2 == 1)
	{
		light2->enable();
	}
	else
	{
		light2->disable();
	}
}

void GameScene::changeLight3()
{
	if(*light_3 == 1)
	{
		light3->enable();
	}
	else
	{
		light3->disable();
	}
}

char* GameScene::getMessage()
{
	return this->message;
}

void GameScene::undo()
{
	pause = game_ended = false;

	strcpy(message,"Cheater...");

	message[0] = 'C';
	message[1] = 'h';
	message[2] = 'e';
	message[3] = 'a';
	message[4] = 't';
	message[5] = 'e';
	message[6] = 'r';

	if(game_states->size()==0) return;
	board = game_states->top();
	game_states->pop();
	redrawBoard();
	changePlayer();
}

void GameScene::playMovie()
{
	if(game_ended){
		game_states->push(board);
		pause = movie = true;
		//invert stack
		while(game_states->size()>0){
			movie_boards->push(game_states->top());
			game_states->pop();
		}
		//make pieces visible
		for(Piece *p : boardPieces){
			p->exists = true;
		}
	}
}

void GameScene::changeAmbient(int ambientID)
{
	currentAmbient = ambients[ambientID-1];
}

void GameScene::changeGameMode(int modeID)
{
    initBoard();
    
	switch(modeID){
	case pvp:
		gm = pvp;
		break;
	case pvm:
		gm = pvm;
		break;
	case pvs:
		gm = pvs;
		break;
	}
}

void GameScene::changeCamera(int viewID)
{
       initCameras();
       activeCamera->applyView();
       display();
    
    if(viewID == 2)
	{
        activeCamera->setX(-0.5);
		activeCamera->setY(-0.5);
        activeCamera->setZ(-0.5);
		activeCamera->applyView();
        display();
	}
	else
		if(viewID == 3)
		{
			activeCamera->setX(-8.0);
			activeCamera->setY(-3.0);
			activeCamera->setZ(-8.0);
			activeCamera->applyView();
			display();
		}

}

void GameScene::changeTimeToPlay(int timeID)
{
	play_time = timeID * 1000;
}

void GameScene::redrawBoard(){
	positions.erase(positions.begin(),positions.end());
	int x=0,y=0,pos=0;
	string id("    ");
	Piece *p = NULL;
	for(int i = 0; i < board.length(); i++){
		if(x==9){
			x=0;
			y++;
		}
		if(board[i] == 'm'){
			id[0] = board[i];
			id[1] = board[i+1];
			id[2] = board[i+2];
			id[3] = '\0';
			p = getPiece(id);
			p->player='m';
			p->id = id;
		}else if(board[i]=='s'){
			id[0] = board[i];
			id[1] = board[i+1];
			id[2] = board[i+2];
			id[3] = '\0';
			p = getPiece(id);
			p->player='s';
			p->id = id;
		}else if(board[i]=='r'){
			id[0] = board[i];
			id[1] = board[i+1];
			id[2] = board[i+2];
			id[3] = '\0';
			p = getPiece(id);
			p->id = id;
			//king belongs to player s
			p->king = true;
			p->player='s';
		}

		if((board[i] == ',' || i == board.length()-1)){
			// Set Piece position
			if(p){
				p->xPos = x;
				p->yPos = y;
				boardPieces.push_back(p);
				Pair pair;
				pair.x = x;
				pair.y = y;
				positions[pair] = pos++;
				p = NULL;
			}
			x++;
		}
        
        playing = true;
	}
}

Piece* GameScene::getPiece(int x,int y){
	Pair p(x,y);
	map<Pair,int>::iterator it = positions.find(p);
	return (it == positions.end() ? NULL : boardPieces[it->second]);
}

Piece* GameScene::getPiece(string id){
	for (Piece *p : boardPieces) {
		if(strcmp(p->id.c_str(),id.c_str())==0){
			p->exists = true;
			return p;
		}
	}
	return new Piece();
}

void GameScene::dumpPositions(){
	cout << endl;
	for(int i = 0; i < boardPieces.size(); ++i){
		cout << "Id: " << boardPieces[i]->id 
			<< " x: " << boardPieces[i]->xPos
			<< " y: " << boardPieces[i]->yPos
			<< " " << i
			<< endl;
	}
	for(Iter i = positions.begin() ; i != positions.end() ; i++){
		cout << "x: " << i->first.x
			<< " y: " << i->first.y
			<< " pos: " << i->second
			<< endl;
	}
}

void GameScene::update(unsigned long millis){
	if(!pause) elapse_time += 100;
	if(elapse_time > 0 && elapse_time % play_time == 0){
		//play time over
		elapse_time = 0;
		changePlayer();
		aiMove();
	}
	if(movie) 
		movie_step -= 100;
	if(movie && !movie_step && moves.size() > 0 && movie_boards->size()>0){
		movie_step = 1000;
		board = movie_boards->top();
		if(time++==0) redrawBoard();
		movie_boards->pop();
		Move m = moves.front();
		selected_piece = m.selected;
		movePiece(m.xi,m.yi,m.xf,m.yf);
		moves.erase(moves.begin());

	}
    
}

void GameScene::restart(){
	movie_step = 1000;
	elapse_time = 0;
	play_time = 20000;
	turn = 'm';
	pause = true;
	game_ended = false;
	movie = false;
	movie_boards = new stack<string>;
	time = 0;
	game_states = new stack<string>;
	initBoard();
}