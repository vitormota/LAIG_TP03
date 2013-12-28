#ifndef PickInterface_H
#define PickInterface_H

#include "PickScene.h"
#if _WIN32
#include <CGFinterface.h>
#else
#include "CGFinterface.h"
#endif

class PickInterface: public CGFinterface {
	public:

		PickInterface(PickScene *ps);

		virtual void processMouse(int button, int state, int x, int y);	
		void performPicking(int x, int y);
		void processHits(GLint hits, GLuint buffer[]);
		void processKeyboard(unsigned char key, int x, int y);
		void initGUI();
		void processGUI(GLUI_Control *ctrl);
		void display();

		int x1,x2,y1,y2;
		bool pos1,pos2;
	
		int ambientID, modeID, viewID, timeID, diff_id;
		char currentMessage[20];

		PickScene *ps;
};


#endif
