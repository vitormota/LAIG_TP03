#include "Piece.h"

#define _DEFAULT_STACKS_SLICES 12
#define _USE_MATH_DEFINES
#include <math.h>
#define _RAD_TO_DEG M_PI / 180.0

Piece::Piece():CGFobject(){
	elevation = 0.0;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	this->slices = _DEFAULT_STACKS_SLICES;
	this->stacks= _DEFAULT_STACKS_SLICES;
	this->radius = 1.0;
	this->radius_2 = radius/2;
	this->height=1;
	player = '-';
}

Piece::Piece(int slices,int stacks,float radius):CGFobject(){
	elevation = 0.0;
	this->slices = slices;
	this->stacks= stacks;
	this->radius = radius;
	this->radius_2 = radius/2;
	this->height=1;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	player = '-';
}

Piece::~Piece(){
	delete(app);
	delete(text);
	gluDeleteQuadric(quad);
}

void Piece::draw(){

	//text->apply();

	glPushMatrix();{
		glTranslatef(0,elevation,0.0);
		glRotatef(-90,1,0,0);
		gluCylinder(quad,radius,radius_2,height,slices,stacks);
		glTranslatef(0,0.0,1.0);
		gluCylinder(quad,radius_2,radius_2,height,slices,stacks);
		glTranslatef(0,0.0,1.0);
		glutSolidSphere(radius_2,slices,stacks);
	}
	glPopMatrix();

	drawBase();



}

void Piece::drawBase(){
	float angle_step = (float) 360.0 / this->slices;
	float angle = angle_step;
	float x, z;
	glPushMatrix();{
		glTranslatef(0,elevation,0);
		glBegin(GL_POLYGON);{
			for(int i = 0; i < this->slices ;i++){
				angle += angle_step;
				x = cos(angle*_RAD_TO_DEG)*this->radius;
				z = sin(angle*_RAD_TO_DEG)*this->radius;
				glVertex3f(x,0,z);
			}
		}glEnd();

	}glPopMatrix();
}