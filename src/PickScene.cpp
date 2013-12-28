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

class PrologBadResponse{};

void PickScene::init()
{
	//DEBUG -- TODO change this var on new game options
	turn = 'm';
	pause = true;
	gm = pvp;
	game_ended = false;

	game_states = new stack<string>;

	// Apply transformations corresponding to the camera position relative to the origin

	// normal init, no changes needed

	// Enables lighting computations
	glEnable(GL_LIGHTING);

	// Global ambient light (do not confuse with ambient component of individual lights)
	float globalAmbientLight[4]= {0.2,0.2,0.2,1.0};

	// Sets up some lighting parameters
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient/*globalAmbientLight*/);  // Define ambient light

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
	previousTime = 0;
	animatePiece = false;

	// animate chosen piece (chose coordinates on the board of it)
	int x = 8;
	int y = 4;

	// final coordinates
	int xFinal = 8;
	int yFinal = 6;
	anim = new Animation(x, y, xFinal, yFinal, glutGet( GLUT_ELAPSED_TIME ));

	/* Ambients */

	boardBase = new Rect("rec", -0.5, -0.5, 0.5, 0.5);

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
	currentCamera = activeCamera;

	//request server init board and parse it
	initBoard();

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

			//// find piece with the position on the board x,y
			//for(unsigned int i = 0; i < boardPieces.size(); i++)
			//{
			//	p = boardPieces[i];
			//	// if it is the chosen piece
			//	if((p->getXPos() == x) && (p->getYPos() == y))
			//	{
			//		anim->animate(passedTime);
			//		boardPieces[i]->setXPos(anim->currentXPos);
			//		boardPieces[i]->setYPos(anim->currentZPos);
			//	}
			//}
		}
	}

}

void PickScene::display()
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

	// Draw board base
	glPushMatrix();
	currentAmbient->tileGApp->apply();
	//glRotated(90, 1, 0, 0);
	//glScaled(3*4, 0, 3*2);
	BoardTile* b0 = new BoardTile();
	b0->setXPos(0);
	b0->setYPos(0);

	BoardTile* b1 = new BoardTile();
	b1->setXPos(1);
	b1->setYPos(0);

	BoardTile* b2 = new BoardTile();
	b2->setXPos(2);
	b2->setYPos(0);

	BoardTile* b3 = new BoardTile();
	b3->setXPos(3);
	b3->setYPos(0);

	//glScalef(0.5, 0.5, 0.5);
	//glTranslated(0, 0, 0);
	//boardBase->draw();
	b0->draw();
	b1->draw();
	b2->draw();
	b3->draw();
	glPopMatrix();

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

		if((!currentPiece->exists) && (currentPiece->player == this->turn))
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
	delete(obj);
	delete(light0);
}

void PickScene::initBoard(){
	board = con->get_init_board();
	game_states->push(board);
	reDrawBoard();

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

void PickScene::aiMove(){
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
		strcpy(message,"PC has some moves...");
		//apply changes
		Pair p(xi,yi);
		Pair pf(xf,yf);
		int pos = positions[p];
		boardPieces[pos]->xPos = xf;
		boardPieces[pos]->yPos = yf;
		positions.erase(p);
		positions[pf] = pos;
		game_states->push(board);
		board = newBoard;
		check_pieces();
		changePlayer();
	}
	game_ended = isGameFinished();
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
		if(p && p->player == turn){
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
		game_ended = isGameFinished();
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
		aiMove();
		dumpPositions();
	}
	return success;
}

bool PickScene::isGameFinished(){
	return con->gameEnd(board);
}

void PickScene::check_pieces(){
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

void PickScene::changePlayer(){
	if(turn == 'm'){
		turn = 's';
	}else{
		turn = 'm';
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

void PickScene::changeLight0()
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

void PickScene::changeLight1()
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

void PickScene::changeLight2()
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

void PickScene::changeLight3()
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

char* PickScene::getMessage()
{
	return this->message;
}

void PickScene::undo()
{
	// TODO
	strcpy(message,"Cheater...");
	if(game_states->size()==0) return;
	board = game_states->top();
	game_states->pop();
	reDrawBoard();
	changePlayer();
}

void PickScene::playMovie()
{
	// TODO
}

void PickScene::changeAmbient(int ambientID)
{
	currentAmbient = ambients[ambientID-1];
}

void PickScene::changeGameMode(int modeID)
{
	// TODO -> CHANGE GAME MODE -> START NEW GAME WITH THE CHOSEN MODE
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

void PickScene::changeCamera(int viewID)
{
	activeCamera = defaultCamera;

	if(viewID == 1)
	{
		activeCamera->setX(0);
		activeCamera->setY(-0.5);
		activeCamera->setZ(0);
		activeCamera->setRotation(1, 360);
		//activeCamera->updateProjectionMatrix(10, 10);
		activeCamera->applyView();
		CGFscene::display();
	}
	else
		if(viewID == 2)
		{
			activeCamera->setX(0);
			activeCamera->setY(-10);
			activeCamera->setZ(0);
			activeCamera->setRotation(1, 360);
			//activeCamera->updateProjectionMatrix(10, 10);
			activeCamera->applyView();
			CGFscene::display();
		}

}

void PickScene::changeTimeToPlay(int timeID)
{
	// TODO
}

void PickScene::reDrawBoard(){
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
	}
}

//DEBUG
Piece* PickScene::getPiece(int x,int y){
	Pair p(x,y);
	map<Pair,int>::iterator it = positions.find(p);
	return (it == positions.end() ? NULL : boardPieces[it->second]);
}

Piece* PickScene::getPiece(string id){
	for (Piece *p : boardPieces) {
		if(strcmp(p->id.c_str(),id.c_str())==0){
			p->exists = true;
			return p;
		}
	}
	return new Piece();
}

typedef map<Pair,int>::iterator Iter;
void PickScene::dumpPositions(){
	int pos;
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

