#include "GameScene.h"
#include "Cube.h"

void GameScene::init(){

	

}

void GameScene::display(){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// Initialize Model-View matrix as identity (no transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Apply transformations corresponding to the camera position relative to the origin
	CGFscene::activeCamera->applyView();
	axis.draw();


	//cube = new Primitive::Cube();
	//cube->draw();
	glutSwapBuffers();
}

void GameScene::update(unsigned long time){

}

GameScene::~GameScene(){

}