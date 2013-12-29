#ifndef GameInterface_H
#define GameInterface_H

#include "GameScene.h"
#if _WIN32
#include <CGFinterface.h>
#else
#include "CGFinterface.h"
#endif

class GameInterface: public CGFinterface {
	public:

		GameInterface(GameScene *gs);

		virtual void processMouse(int button, int state, int x, int y);	
		void performPicking(int x, int y);
		void processHits(GLint hits, GLuint buffer[]);
		void processKeyboard(unsigned char key, int x, int y);
		void initGUI();
		void processGUI(GLUI_Control *ctrl);
		void update(unsigned long time);

		int x1,x2,y1,y2;
		bool pos1,pos2;
	
		int ambientID, modeID, viewID, timeID, diffID;
		char currentMessage[20];

		GameScene *gs;
};


#endif
