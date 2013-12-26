#if _WIN32
#include <GL\glew.h>
#include <GL\glui.h>
#endif

#include "CGFaxis.h"
#include "CGFappearance.h"

#include "Primitive.h"
#include "PickScene.h"

#include <math.h>

using namespace std;

#define NUM_OBJS 25
#define NUM_ROWS 9
#define NUM_COLS 9
#define BOARD_TILE_SIDE_SIZE 3

string TEST_BOARD[81] = { "   ","   ","   ","m05","m06","m07","   ","   ","   " ,
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
	game_states = new stack<string>;
    
	// normal init, no changes needed
    
	// Enables lighting computations
	glEnable(GL_LIGHTING);
    
	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light
    
	// Declares and enables a light
	float light0_pos[4] = {4.0, 6.0, 5.0, 1.0};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	light0->enable();
    
	// Defines a default normal
	glNormal3f(0,0,1);
    
	//request server init board and parse it
	initBoard();
    
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
    
	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();
    
	// Draw (and update) light
	light0->draw();
    
	// Draw axis
	axis.draw();
    
    
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
            
            boardPieces[i]->draw();
            
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
            
            boardTiles[i]->draw();
            
            glPopName();
            glPopName();
            
        }
        
        for(int j = 0; j < boardPieces.size();j++)
        {
            boardPieces[j]->draw();
        }
    }
    // Picking pieces and board tiles (FOR TESTING ONLY)
    else if((this->pickBoardTile == true) && (this->pickPiece == true))
    {
        for(int i = 0; i < boardPieces.size();i++)
        {
            glPushName(boardPieces[i]->getYPos());
            glPushName(boardPieces[i]->getXPos());
            
            boardPieces[i]->draw();
            
            glPopName();
            glPopName();
            
        }
        
        for(int j = 0; j < boardTiles.size();j++)
        {
            
            glPushName(boardTiles[j]->getYPos());
            glPushName(boardTiles[j]->getXPos());
            
            boardTiles[j]->draw();
            
            glPopName();
            glPopName();
            
        }
        
    }
    // Only draw the board without picking
    else{
        for(int i = 0; i < boardPieces.size();i++)
        {
            boardPieces[i]->draw();
        }
        
        for(int j = 0; j < boardTiles.size();j++)
        {
            boardTiles[j]->draw();
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
    
	/*
     OLD CODE
     
     board = con->get_init_board();
     
     
     game_states->push(board);
     int x=0, y=0, pos=0;
     Piece *p = NULL;
     for(int i = 0; i < board.length(); i++){
     if(x==9){
     x=0;
     y++;
     }
     if(board[i] == 'm'){
     p = new Piece();
     p->player='m';
     }else if(board[i]=='s'){
     p = new Piece();
     p->player='s';
     }else if(board[i]=='r'){
     p = new Piece();
     p->player='r';
     }
     
     // Set Piece position
     p->setXPos(x);
     p->setYPos(y);
     
     if(board[i] == ',' || i == board.length()-1){
     cells[pos++] = new Cell(p,x,y);
     p = NULL;
     x++;
     }
     }*/
    
    
    // Get Board
    /*board = con->get_init_board();
     game_states->push(board);
     */
    
    // Create board pieces
    int x=0, y=0;
	for(int i = 0; i < 81; i++){
		if(x==9){
			x=0;
			y++;
		}
        
        string pieceStr = TEST_BOARD[i]; // TODO -> change TEST_BOARD to board
        Piece* newPiece = new Piece();
        
		if(pieceStr[0] == 'm'){
			newPiece->player='m';
            // Set Piece position
            newPiece->setXPos(x);
            newPiece->setYPos(y);
            // Save piece
            this->boardPieces.push_back(newPiece);
		}else if(pieceStr[0] =='s'){
			newPiece->player='s';
            // Set Piece position
            newPiece->setXPos(x);
            newPiece->setYPos(y);
            // Save piece
            this->boardPieces.push_back(newPiece);
		}else if(pieceStr[0] == 'r'){
			newPiece->player='s';
            // Set Piece position
            newPiece->setXPos(x);
            newPiece->setYPos(y);
            // Save piece
            this->boardPieces.push_back(newPiece);
		}
        
        x++;
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

string replace(string data){
	string res = data;
	for(int i = 0; i < data.length() ; i++){
		if(res[i]=='[' || res[i] == ']'){
			res[i] = ' ';
		}
	}
	return res;
}

