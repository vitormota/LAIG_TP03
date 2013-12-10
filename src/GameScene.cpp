#include "GameScene.h"


void GameScene::init(){
	glEnable(GL_BLEND);

	// Enables lighting computations
	glEnable(GL_LIGHTING);
	

	// Sets up some lighting parameters
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, CGFlight::background_ambient);  // Define ambient light

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);	

	// Declares and enables a light
	float light0_pos[4] = {0, 7.0, 0, 1.0};
	float light0_difuse[4] = {1, 1, 1, 1.0};
	float light0_ambient[4] = {0, 0,0, 1.0};
	float light0_specular[4] = {1,1, 1, 1.0};
	float light0_spotDirection[3] = {0,-1,0};
	float light0_spotCutoff[1] = {22.5};
	light0 = new CGFlight(GL_LIGHT0, light0_pos);
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_spotDirection);
	glLightfv(GL_LIGHT0,GL_SPOT_CUTOFF,light0_spotCutoff);
	light0->setDiffuse(light0_difuse);
	light0->setAmbient(light0_ambient);
	light0->setSpecular(light0_specular);
	light0->enable();
	
	float light1_pos[4] = {0, 7.0, 0, 1.0};
	float light1_difuse[4] = {0.4, 0.4, 0.4, 1.0};
	float light1_ambient[4] = {0, 0, 0, 1.0};
	float light1_specular[4] = {0, 0, 0, 1.0};
	light1 = new CGFlight(GL_LIGHT1, light1_pos);
	light1->setDiffuse(light1_difuse);
	light1->setAmbient(light1_ambient);
	light1->setSpecular(light1_specular);
	light1->enable();

	// Default normal
	glNormal3f(0,0,1);

	board = new Board();

}

void GameScene::display(){
	glBlendFunc(GL_ONE, GL_ZERO);
	
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
	light1->draw();

	// Draw axis
	axis.draw();


	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	board->draw();
	// ---- END Background, camera and axis setup

	//demoshader->bind();

	//demoshader->unbind();

	// ---- END feature demos

	// We have been drawing in a memory area that is not visible - the back buffer, 
	// while the graphics card is showing the contents of another buffer - the front buffer
	// glutSwapBuffers() will swap pointers so that the back buffer becomes the front buffer and vice-versa
	glutSwapBuffers();
}

void GameScene::update(unsigned long time){

}

GameScene::~GameScene(){

}