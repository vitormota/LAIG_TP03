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

void PickScene::init() 
{
	game_states = new stack<string>;
	cells = new Cell*[81];

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

	//obj=new ExampleObject();
	p = new scene::Cylinder("",2.0, 2.0, 1.0, 10, 10);
	tile = new BoardTile("1");
	materialAppearance=new CGFappearance();

	//request server init board and parse it
	initBoard();

}

void PickScene::display() 
{

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

	materialAppearance->apply();

	//// scale down a bit
	//glTranslated(NUM_COLS-2+0.2, 0.0, -NUM_COLS);
	//glRotatef(90,0,0,1);
	//glScalef(0.2, 0.2, 0.2);

	//// picking example, the important parts are the gl*Name functions
	//// and the code in the associted PickInterface class

	//// Example 1: simple naming
	//glPushMatrix();

	//glPushName(-1);		// Load a default name

	//for (int i=0; i< NUM_OBJS;i++)
	//{
	//	glPushMatrix();
	//	glTranslatef(i*NUM_ROWS,0,0);
	//	glLoadName(i);		//replaces the value on top of the name stack
	//	//obj->draw();
	//	p->draw();
	//	tile->draw();
	//	glPopMatrix();
	//}
	//glPopMatrix();

	//// example 2: structured naming
	//for (int r=0; r < NUM_ROWS; r++)
	//{
	//	glPushMatrix();
	//	glTranslatef(0, r*NUM_COLS, 0);
	//	glLoadName(r);
	//	for (int c=0; c < NUM_COLS; c++)
	//	{
	//		glPushMatrix();
	//		glTranslatef(0,0,(c+1)*NUM_ROWS);
	//		glRotatef(90,0,1,0);

	//		// TODO adicionar aqui

	//		/*
	//		if(FAZER PICK A PEÇA) então => glPushName só da peça
	//		draw da peça
	//		glPopName
	//		else
	//		if(FAZER PICK AO TILE) então => glPushName só do tile
	//		draw do tile
	//		glPopName
	//		*/
	//		glPushName(c);
	//		//obj->draw();
	//		p->draw();
	//		tile->draw();
	//		glPopName();
	//		glPopMatrix();
	//	}
	//	glPopMatrix();
	//}


	//draw pieces
	//move to cell 0,0
	glPushMatrix();{
		glTranslatef(0.0,1.0,0.0);
		int row=0;
		for(int i = 0; i < 81 ; i++){
			if(cells[i]->piece){
				cells[i]->piece->draw();
			}
			if(row < 8){
				glTranslatef(3,0,0);
				row++;
			}
			else{
				glTranslatef(-24,0,3);
				row = 0;
			}
		}
	}glPopMatrix();

	//example code to be used with picking interface
	//a move is sent to server, if accepted changes are made to board
	//the board is saved for the undo feature in a stack

	string newBoard;
	if(con->play(3,0,3,1,board,newBoard)){
		//save board
		game_states->push(newBoard);
		//change board
		cells[1*9+3]->piece = cells[0*9+3]->piece;
		cells[0*9+3]->piece = NULL;
	}

	//safety
	con->quit();

	//end example code

	// ---- END feature demos

	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

PickScene::~PickScene()
{
	delete(materialAppearance);
	delete(obj);
	delete(p);
	delete(tile);
	delete(light0);
}

void PickScene::initBoard(){
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
			p->player='s';
		}
		if(board[i] == ',' || i == board.length()-1){
			cells[pos++] = new Cell(p,x,y);
			p = NULL;
			x++;
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

