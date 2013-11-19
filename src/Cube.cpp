#include "Cube.h"

namespace Primitive{
	Cube::Cube()
		:definition(1),smooth(false){;}

	Cube::Cube(int definition,bool smooth,string textfile)
		:definition(definition),smooth(smooth){
			this->texture = new CGFtexture(textfile);
	}

	Cube::~Cube(){
		delete this->texture;
	}

	void Cube::draw(){
		drawFace();
	}

	void Cube::drawFace(){
		glBegin(GL_QUADS);
		{
			glVertex3fv((rectangle));
			glVertex3fv((rectangle+3));
			glVertex3fv((rectangle+6));
			glVertex3fv((rectangle+9));
		}
		glEnd();
	}

	void Cube::setTexture(string file){
		this->texture = new CGFtexture(file);
	}
}

