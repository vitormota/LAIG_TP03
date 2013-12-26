#ifndef PickInterface_H
#define PickInterface_H

#include "PickScene.h"
#include "CGFinterface.h"

class PickInterface: public CGFinterface {
	public:

		PickInterface(PickScene *ps);

		virtual void processMouse(int button, int state, int x, int y);	
		void performPicking(int x, int y);
		void processHits(GLint hits, GLuint buffer[]);

		int x1,x2,y1,y2;
		bool pos1,pos2;

		PickScene *ps;
};


#endif
