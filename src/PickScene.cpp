#if _WIN32
#include <GL\glew.h>
#include <GL\glui.h>
#endif

#include "CGFaxis.h"
#include "CGFappearance.h"

#include "Primitive.h"
#include "PickScene.h"
#include "Animation.h"

#include <math.h>

using namespace std;

#define NUM_OBJS 25
#define NUM_ROWS 9
#define NUM_COLS 9
#define BOARD_TILE_SIDE_SIZE 3

string TEST_BOARD[81] = {
	"   ","   ","   ","m05","m06","m07","   ","   ","   " ,
	"   ","   ","   ","   ","m08","   ","   ","   ","   " ,
	"   ","   ","   ","   ","s02","   ","   ","   ","   " ,
	"m01","   ","   ","   ","s01","   ","   ","   ","m09" ,
	"m02","m04","s08","s07","rei","s03","s04","m11","m10" ,
	"m03","   ","   ","   ","s05","   ","   ","   ","m12" ,
	"   ","   ","   ","   ","s06","   ","   ","   ","   " ,
	"   ","   ","   ","   ","m16","   ","   ","   ","   " ,
	"   ","   ","   ","m13","m14","m15","   ","   ","   "  };

void PickScene::init()
{
	//DEBUG -- TODO change this var on new game options
	player = 'm';

	game_states = new stack<string>;

	// Apply transformations corresponding to the camera position relative to the origin
	
	// normal init, no changes needed

	// Enables lighting computations
	glEnable(GL_LIGHTING);

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light

	// Declares and enables a light
	float light0_pos[4] = {6.0, 6.0, 6.0, 1.0};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();

	// Defines a default normal
	glNormal3f(0,0,1);

	//request server init board and parse it
	initBoard();

	// Initialize update period and previousTime
	setUpdatePeriod(100);
	previousTime = 0;

	animatePiece = false;

	// animate chosen piece (chose coordinates on the board of it)
	int x = 8;
	int y = 4;

	// final coordinates
	int xFinal = 8;
	int yFinal = 6;
	anim = new Animation(x, y, xFinal, yFinal, glutGet( GLUT_ELAPSED_TIME ));
    
    // Create Ambients
   
    // Marble ambient
    Ambient* marbleAmbient = new Ambient("MARBLE");
    marbleAmbient->setTextures("/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WALL_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/G_WOOD.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_WOOD.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_WOOD.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_WOOD.jpg");
    
    ambients.push_back(marbleAmbient);
    
    
    // Jewels Ambient
    Ambient* jewelsAmbient = new Ambient("JEWELS");
    jewelsAmbient->setTextures("/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WALL_MARBLE.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/G_WOOD.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/WHITE_WOOD.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/BLACK_WOOD.jpg",
                               "/Users/mj/Documents/Disciplinas/LAIG/3º Trabalho/LAIG - P3/data/RED_WOOD.jpg");
    
    ambients.push_back(jewelsAmbient);
    
    
    currentAmbient = marbleAmbient; // default ambient
    
}

void PickScene::update(unsigned long time)
{
	if(previousTime == 0)
	{
		previousTime = time;
	}
	else
	{
		unsigned long passedTime = time - previousTime;
		previousTime = time;
		animatePiece = true;

		if(animatePiece == true)
		{
			// animate chosen piece (chose coordinates on the board of it)
			int x = 8;
			int y = 4;

			// find piece with the position on the board x,y
			for(unsigned int i = 0; i < boardPieces.size(); i++)
			{
				p = boardPieces[i];
				// if it is the chosen piece
				if((p->getXPos() == x) && (p->getYPos() == y))
				{
					anim->animate(passedTime);
					boardPieces[i]->setXPos(anim->currentXPos);
					boardPieces[i]->setYPos(anim->currentZPos);
				}
			}
		}
	}

}

void PickScene::display()
{

	// TEST THE PICKING -> To switch from picking the board tiles or the board pieces
	this->pickBoardTile = true;
	this->pickPiece = true;


	// ---- BEGIN Background, camera and axis setup

	// Clear image and depth buffer everytime we update the scene
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	CGFscene::activeCamera->applyView();

	// Draw (and update) light
	light0->draw();

	// Draw axis
	//axis.draw();


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

			drawPiece(i);

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
			drawPiece(j);
		}
	}
	// Picking pieces and board tiles
	else if((this->pickBoardTile == true) && (this->pickPiece == true))
	{
		for(int i = 0; i < boardPieces.size();i++)
		{
			glPushName(boardPieces[i]->getYPos());
			glPushName(boardPieces[i]->getXPos());

            drawPiece(i);

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
			drawPiece(i);
		}

		for(int j = 0; j < boardTiles.size();j++)
		{
			drawBoardTile(j);
		}

	}
    
    
    // draw auxiliary board
    for(int k = 0; k < boardPieces.size(); k++)
    {
        Piece* currentPiece = boardPieces[k];
        
        if((!currentPiece->exists) && (currentPiece->player == this->player))
        {
            glPushMatrix();
            glTranslated(10, 0, 10); // TODO -> aqui tem q ter uma variável para a peça aparecer ao lado da anterior .. ou seja .. o x ou z tem q aumentar para elas aparecerem umas ao lado das outras
            drawPiece(k);
            glPopMatrix();
        }
    }

	glPopMatrix();

	//example code to be used with picking interface
	//a move is sent to server, if accepted changes are made to board
	//the board is saved for the undo feature in a stack

	/*string newBoard;
	if(con->play(3,0,3,1,board,newBoard)){
	//save board
	game_states->push(newBoard);
	//change board
	cells[1*9+3]->piece = cells[0*9+3]->piece;
	cells[0*9+3]->piece = NULL;
	}

	//safety
	con->quit();*/

	//end example code

	// ---- END feature demos

	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

PickScene::~PickScene()
{
	delete(materialAppearance);
	delete(obj);
	delete(light0);
}

void PickScene::initBoard(){

	board = con->get_init_board();
	game_states->push(board);

	int x=0, y=0, pos = 0;
	Piece *p = NULL;
	for(int i = 0; i < board.length(); i++){
		if(x==9){
			x=0;
			y++;
		}
		if(board[i] == 'm'){
			p = new Piece();
			p->player='m';
			p->id[0] = board[i];
			p->id[1] = board[i+1];
			p->id[2] = board[i+2];
			p->id[3] = '\0';
			//printf("%s",p->id);
		}else if(board[i]=='s'){
			p = new Piece();
			p->player='s';
			p->id[0] = board[i];
			p->id[1] = board[i+1];
			p->id[2] = board[i+2];
			p->id[3] = '\0';
			
		}else if(board[i]=='r'){
			//king belongs to player s
			p = new Piece();
			p->king = true;
			p->player='s';
			p->id[0] = board[i];
			p->id[1] = board[i+1];
			p->id[2] = board[i+2];
			p->id[3] = '\0';
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
	}

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

}

void PickScene::drawBoardTile(int j)
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

void PickScene::drawPiece(int i)
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

bool PickScene::un_elevatePiece(){
	if(selected_piece){
		selected_piece->un_elevate();
		selected_piece = NULL;
		return true;
	}
	return false;
}

bool PickScene::elevatePiece(int x,int y){
	bool res = false;
	int pos = -1;
	Pair p;
	p.x = x;
	p.y = y;
	map<Pair,int>::iterator it = positions.find(p);
	if(it != positions.end()){
		pos = it->second;
		Piece *p = boardPieces[pos];
		if(p && p->player == player){
			res = true;
			this->selected_piece = p;
			p->elevate();
		}
	}
	return res;
}

bool PickScene::emptySpace(int x,int y){
	Pair p(x,y);
	if(positions.find(p)!=positions.end()){
		return false;
	}
	return true;
}

bool PickScene::movePiece(int xi,int yi,int xf,int yf){
	bool success = false;
	string newBoard;
	if(con->play(xi,yi,xf,yf,board,newBoard)){
		success = true;
		//trigger animation
		Pair p(xi,yi);
		Pair pf(xf,yf);
		int pos = positions[p];
		positions.erase(p);
		positions[pf] = pos;
		selected_piece->xPos = xf;
		selected_piece->yPos = yf;
		//printf("%i,%i",selected_piece->xPos,selected_piece->yPos);
		selected_piece->un_elevate();
		selected_piece = NULL;
		game_states->push(board);
		board = newBoard;
		check_pieces();
		changePlayer();
	}
	return success;
}

void PickScene::check_pieces(){
	string id;
	Piece *p;
	for(int i = 0; i < boardPieces.size(); i++){
		p = boardPieces[i];
		if(board.find(p->id) == string::npos){
			p->exists = false;
			Pair loc(p->xPos,p->yPos);
			positions.erase(loc);
		}
	}
}

void PickScene::changePlayer(){
	if(player == 'm'){
		player = 's';
	}else{
		player = 'm';
	}
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

