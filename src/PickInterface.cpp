#if _WIN32
#include <GL\glew.h>
#include <GL\glui.h>
#endif


#include "PickInterface.h"
#include "CGFapplication.h"

// buffer to be used to store the hits during picking
#define BUFSIZE 256
GLuint selectBuf[BUFSIZE];

PickInterface::PickInterface(PickScene *ps){
	this->ps = ps;
	x1 = 0;
	x2 = 0;
	y1 = 0;
	y2 = 0;
	pos1 = false;
	pos2 = false;
}

void PickInterface::processKeyboard(unsigned char key, int x, int y)
{
	// Uncomment below if you would like to process the default keys (e.g. 's' for snapshot, 'Esc' for exiting, ...)
	// CGFinterface::processKeyboard(key, x, y);
    
	switch(key)
	{
		case 'a':
		{
			// This is an example of accessing the associated scene
			// To test, create the function toggleSomething in your scene to activate/deactivate something
			//((LightingScene *) scene)->toggleSomething();
			break;
		}
	}
}

void PickInterface::initGUI()
{
    /*
     // Check CGFinterface.h and GLUI documentation for the types of controls available
     GLUI_Panel *varPanel= addPanel("Group", 1);
     addSpinnerToPanel(varPanel, "Val 1(interface)", 2, &testVar, 1);
     
     // You could also pass a reference to a variable from the scene class, if public
     addSpinnerToPanel(varPanel, "Val 2(scene)", 2, &(((LightingScene*) scene)->sceneVar), 2);
     */
    
    // Main panel
    GLUI_Panel *mainPanel= addPanel("Options", 1);
    
    // Lights
    /*GLUI_Panel *lightsPanel= addPanelToPanel(mainPanel, "Luzes", 1);
    addCheckboxToPanel(lightsPanel, "light0", ((LightingScene *) scene)->light_0, 2);
    addCheckboxToPanel(lightsPanel, "light1", ((LightingScene *) scene)->light_1, 3);
    addCheckboxToPanel(lightsPanel, "light2", ((LightingScene *) scene)->light_2, 4);
    addCheckboxToPanel(lightsPanel, "light3", ((LightingScene *) scene)->light_3, 5);
    addCheckboxToPanel(lightsPanel, "windowLight", ((LightingScene *) scene)->window_Light, 6);
    
    addColumnToPanel(mainPanel);
    
    // Clock
    GLUI_Panel *clockPanel= addPanelToPanel(mainPanel, "Clock", GLUI_PANEL_EMBOSSED);
    clockPanel->set_alignment(GLUI_ALIGN_CENTER);
    addButtonToPanel(clockPanel, "Stop/Start clock", 8);
    
    addColumnToPanel(mainPanel);
    
    // Textures
    GLUI_Panel *texturesPanel= addPanelToPanel(mainPanel, "Textures", GLUI_PANEL_EMBOSSED);
    
    GLUI_Listbox *textureListBox = addListboxToPanel(texturesPanel, "Choose texture: ",&itemID,10);
    
    textureListBox->add_item(1,"Default");
    textureListBox->add_item(2,"Blue");
    textureListBox->add_item(3,"Map");
    textureListBox->add_item(4,"Grid");
    textureListBox->add_item(5,"Wood");
    
    // Robot Drawing Mode
    GLUI_Panel *viewPanel= addPanelToPanel(mainPanel, "Drawing Mode", GLUI_PANEL_EMBOSSED);
    GLUI_RadioGroup *viewMode = addRadioGroupToPanel(viewPanel, &viewID, 12);
    
    addRadioButtonToGroup(viewMode, "Textured");
    addRadioButtonToGroup(viewMode, "Wireframe");*/
}

void PickInterface::processGUI(GLUI_Control *ctrl)
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
			//((LightingScene *) scene)->changeLight0();
			break;
		};
            
            
	};
    
}

void PickInterface::processMouse(int button, int state, int x, int y) 
{
	CGFinterface::processMouse(button,state, x, y);

	// do picking on mouse press (GLUT_DOWN)
	// this could be more elaborate, e.g. only performing picking when there is a click (DOWN followed by UP) on the same place
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		performPicking(x,y);
}

void PickInterface::performPicking(int x, int y) 
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
	processHits(hits, selectBuf);
}

void PickInterface::processHits (GLint hits, GLuint buffer[]) 
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
		// this should be replaced by code handling the picked object's ID's (stored in "selected"), 
		// possibly invoking a method on the scene class and passing "selected" and "nselected"
		printf("Picked ID's: ");

		if(!pos1){
			x1 = selected[1];
			y1 = selected[0];
			if(ps->elevatePiece(x1,y1)){
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
			if(ps->emptySpace(x2,y2)){
				//attempt move
				if(ps->movePiece(x1,y1,x2,y2)){
					//move was a success, clear all
					pos1=false;
				}
			}else{
				//piece found on loc 2 so
				ps->un_elevatePiece();
				ps->elevatePiece(x2,y2);
			}

			
		}

		for (int i=0; i<nselected; i++)
			printf("%d ",selected[i]);
		printf("\n");
	}
	else
		printf("Nothing selected while picking \n");	
}
