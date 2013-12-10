#ifndef GAME_SCENE_H
#define GAME_SCENE_H

//Windows specific includes
#if _WIN32
#include <CGFscene.h>
#include <CGFaxis.h>
#include <CGFlight.h>
#endif
//end

#include "Board.h"

class GameScene : public CGFscene{

public:
	void init();
	void display();
	void update(unsigned long);
	~GameScene();
protected:

	Board *board;
	CGFlight *light0,*light1;
	//Primitive::Cube *cube;
};

#endif