#ifndef GAME_SCENE_H
#define GAME_SCENE_H

//Windows specific includes
#if _WIN32
#include <CGFscene.h>
#include <CGFaxis.h>
#endif
//end

#include "Cube.h"

class GameScene : public CGFscene{

public:
	void init();
	void display();
	void update(unsigned long);
	~GameScene();
protected:

	//Primitive::Cube *cube;
};

#endif