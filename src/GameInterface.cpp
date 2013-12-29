#if _WIN32
#include <GL\glew.h>
#include <GL\glui.h>
#include <CGFapplication.h>
#else
#include "CGFapplication.h"
#endif

#include "GameInterface.h"

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

GameInterface::GameInterface(GameScene *gs){
	this->gs = gs;
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
	pos1 = false;
	pos2 = false;
	ambientID = 0;
	modeID = 0;
	viewID = 0;
	timeID = 0;
    
	for(int i = 0; i < 19; i++)
	{
		currentMessage[i] = ' ';
	}
    
	currentMessage[19] = '\0';
}

void GameInterface::processKeyboard(unsigned char key, int x, int y)
{
	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
	// CGFinterface::processKeyboard(key, x, y);
    
	switch(key)
	{
	}
}

void GameInterface::initGUI()
{
	// Main panel
	GLUI_Panel *mainPanel= addPanel("Game Options", 1);
	mainPanel->set_alignment(GLUI_ALIGN_CENTER);
    
	// Lights
	GLUI_Panel *lightsPanel= addPanelToPanel(mainPanel, "Lights", 1);
	addCheckboxToPanel(lightsPanel, "North", ((GameScene *) gs)->light_0, 2);
	addCheckboxToPanel(lightsPanel, "South", ((GameScene *) gs)->light_1, 3);
	addCheckboxToPanel(lightsPanel, "East", ((GameScene *) gs)->light_2, 4);
	addCheckboxToPanel(lightsPanel, "West", ((GameScene *) gs)->light_3, 5);
    
    addColumnToPanel(mainPanel);
    
    // Game difficulty
	GLUI_Panel *diffPanel = addPanelToPanel(mainPanel, "Difficulty", GLUI_PANEL_EMBOSSED);
	diffPanel->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_RadioGroup *gameDiff = addRadioGroupToPanel(diffPanel, &diffID, 6);
    
	addRadioButtonToGroup(gameDiff, "Hard");
	addRadioButtonToGroup(gameDiff, "Normal");
	addRadioButtonToGroup(gameDiff, "Easy");
    
	addColumnToPanel(mainPanel);
    
	// Show Message
	GLUI_Panel *msgPanel= addPanelToPanel(mainPanel, "Message", GLUI_PANEL_EMBOSSED);
	msgPanel->set_alignment(GLUI_ALIGN_CENTER);
	addStaticTextToPanel(msgPanel, currentMessage);
    //GLUI_TextBox(msgPanel, ((GameScene *) gs)->getMessage());
    //GLUI_StaticText *text = GLUI::add_statictext_to_panel(msgPanel, ((GameScene *) gs)->getMessage());
    //GLUI_StaticText *text= GLUI::add_statictext_to_panel(msgPanel,((GameScene *) gs)->getMessage());
    
    // Start
    GLUI_Panel *startPanel= addPanelToPanel(mainPanel, "New Game", GLUI_PANEL_EMBOSSED);
    startPanel->set_alignment(GLUI_ALIGN_CENTER);
    addButtonToPanel(startPanel, "Start", 7);
    
    addColumnToPanel(mainPanel);
    
	// Undo
	GLUI_Panel *undoPanel= addPanelToPanel(mainPanel, "Undo", GLUI_PANEL_EMBOSSED);
	undoPanel->set_alignment(GLUI_ALIGN_CENTER);
	addButtonToPanel(undoPanel, "Undo", 8);
    
	// Play Movie
	GLUI_Panel *gamePanel= addPanelToPanel(mainPanel, "Game Movie", GLUI_PANEL_EMBOSSED);
	gamePanel->set_alignment(GLUI_ALIGN_CENTER);
	addButtonToPanel(gamePanel, "Play", 9);
    
    addColumnToPanel(mainPanel);
    
	// Ambients
	GLUI_Panel *ambientsPanel= addPanelToPanel(mainPanel, "Ambient", GLUI_PANEL_EMBOSSED);
	ambientsPanel->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_Listbox *ambientListBox = addListboxToPanel(ambientsPanel, "Choose ambient: ",&ambientID, 10);
    
	ambientListBox->add_item(1,"Marble");
	ambientListBox->add_item(2,"Jewels");
	ambientListBox->add_item(3,"Fabric");
    
	// Views
	GLUI_Panel *viewPanel= addPanelToPanel(mainPanel, "View", GLUI_PANEL_EMBOSSED);
	viewPanel->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_Listbox *viewListBox = addListboxToPanel(viewPanel, "Choose view: ",&viewID, 11);
    
	viewListBox->add_item(1,"Default");
	viewListBox->add_item(2,"Center");
	viewListBox->add_item(3,"Top");
    
    addColumnToPanel(mainPanel);
    
	// Time to Play
	GLUI_Panel *timePanel= addPanelToPanel(mainPanel, "Play time", GLUI_PANEL_EMBOSSED);
	timePanel->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_Spinner *timeSpinner = addSpinnerToPanel (timePanel, "Seconds:", 2, &timeID, 12);
	timeSpinner->set_float_limits(5, 500); // the number of seconds to play is limited between [5, 500] seconds
    
	// Game Mode
	GLUI_Panel *gameModePanel= addPanelToPanel(mainPanel, "Game Mode", GLUI_PANEL_EMBOSSED);
	gameModePanel->set_alignment(GLUI_ALIGN_CENTER);
	GLUI_RadioGroup *gameMode = addRadioGroupToPanel(gameModePanel, &modeID, 13);
    
	addRadioButtonToGroup(gameMode, "Player vs Player");
	addRadioButtonToGroup(gameMode, "Player vs Moscovites");
	addRadioButtonToGroup(gameMode, "Player vs Swedish");
    
}

void GameInterface::update(unsigned long time)
{
	memcpy(currentMessage, ((GameScene *) gs)->getMessage(), 20);
}

void GameInterface::processGUI(GLUI_Control *ctrl)
{
    
	printf ("GUI control id: %d\n  ",ctrl->user_id);
	switch (ctrl->user_id)
	{
        case 1:
		{
			break;
		};
            
        case 2:
		{
			((GameScene *) gs)->changeLight0();
			break;
		};
            
        case 3:
		{
			((GameScene *) gs)->changeLight1();
			break;
		};
            
        case 4:
		{
			((GameScene *) gs)->changeLight2();
			break;
		};
            
        case 5:
		{
			((GameScene *) gs)->changeLight3();
			break;
		};
            
        case 7:
		{   //start game
            gs->pause = false;
            gs->aiMove();
            break;
        };
            
        case 8:
		{
			((GameScene *) gs)->undo();
			break;
		};
            
        case 9:
		{
			((GameScene *) gs)->playMovie();
			break;
		};
            
        case 10:
		{
			((GameScene *) gs)->changeAmbient(ambientID);
			break;
		};
            
        case 11:
		{
			((GameScene *) gs)->changeCamera(viewID);
			break;
		};
            
        case 12:
		{
			((GameScene *) gs)->changeTimeToPlay(timeID);
			break;
		};
            
        case 13:
		{
			((GameScene *) gs)->changeGameMode(modeID);
            //gs->pause = true;
			break;
		};
            
            
	};
    
}

void GameInterface::processMouse(int button, int state, int x, int y)
{
	CGFinterface::processMouse(button,state, x, y);
    
	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void GameInterface::performPicking(int x, int y)
{
	// Sets the buffer to be used for selection and activate selection mode
	glSelectBuffer (BUFSIZE, selectBuf);
	glRenderMode(GL_SELECT);
    
	// Initialize the picking name stack
	glInitNames();
    
	// The process of picking manipulates the projection matrix
	// so we will be activating, saving and manipulating it
	glMatrixMode(GL_PROJECTION);
    
	//store current projmatrix to restore easily in the end with a pop
	glPushMatrix ();
    
	//get the actual projection matrix values on an array of our own to multiply with pick matrix later
	GLfloat projmat[16];
	glGetFloatv(GL_PROJECTION_MATRIX,projmat);
    
	// reset projection matrix
	glLoadIdentity();
    
	// get current viewport and use it as reference for
	// setting a small picking window of 5x5 pixels around mouse coordinates for picking
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
    
	// this is multiplied in the projection matrix
	gluPickMatrix ((GLdouble) x, (GLdouble) (CGFapplication::height - y), 5.0, 5.0, viewport);
    
	// multiply the projection matrix stored in our array to ensure same conditions as in normal render
	glMultMatrixf(projmat);
    
	// force scene drawing under this mode
	// only the names of objects that fall in the 5x5 window will actually be stored in the buffer
	scene->display();
    
	// restore original projection matrix
	glMatrixMode (GL_PROJECTION);
	glPopMatrix ();
    
	glFlush();
    
	// revert to render mode, get the picking results and process them
	GLint hits;
	hits = glRenderMode(GL_RENDER);
	if(gs->pause) return;
	processHits(hits, selectBuf);
}

void GameInterface::processHits (GLint hits, GLuint buffer[])
{
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *selected=NULL;
	GLuint nselected;
    
	// iterate over the list of hits, and choosing the one closer to the viewer (lower depth)
	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			selected = ptr;
			nselected=num;
		}
		for (int j=0; j < num; j++)
			ptr++;
	}
    
	// if there were hits, the one selected is in "selected", and it consist of nselected "names" (integer ID's)
	if (selected!=NULL)
	{
		if(gs->game_ended){
			return;
		}
		// this should be replaced by code handling the picked object's ID's (stored in "selected"),
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");
        
		if(!pos1){
			x1 = selected[1];
			y1 = selected[0];
			if(gs->elevatePiece(x1,y1)){
				//sucess found piece at location
				pos1 = true;
				printf("Piece elevated.\n");
			}else{
				//no piece there sorry
				printf("No piece at target location.\n");
			}
		}else if(pos1){
			x2 = selected[1];
			y2 = selected[0];
			if(gs->emptySpace(x2,y2)){
				//attempt move
				if(gs->movePiece(x1,y1,x2,y2)){
					//move was a success, clear all
					pos1=false;
					x1 = -1;
					x2 = -1;
					y1 = -1;
					y2 = -1;
				}
			}else{
				//piece found on loc 2 so
				gs->lowerPiece();
				gs->elevatePiece(x2,y2);
				x1 = x2;
				y1 = y2;
				x2 = -1;
				y2 = -1;
			}
            
            
		}
        
		Piece *p = gs->getPiece(selected[1],selected[0]);
		if(p){
			printf("\nSelected piece: %s - %i - %i\n",p->id.c_str(),p->xPos,p->yPos);
		}
        
		printf("%d %d",selected[1],selected[0]);
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}
