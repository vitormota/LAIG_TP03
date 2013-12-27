#include "Piece.h"

#define _DEFAULT_STACKS_SLICES 12
#define _USE_MATH_DEFINES
#include <math.h>
#define _RAD_TO_DEG M_PI / 180.0



Piece::Piece():CGFobject(),exists(true){
	elevation = 0.0;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	this->slices = _DEFAULT_STACKS_SLICES;
	this->stacks= _DEFAULT_STACKS_SLICES;
	this->radius = 1.0;
	this->radius_2 = radius/2;
	this->height=1;
	player = '-';
	this->xPos = 0;
	this->yPos = 0;
	this->king = false;
	id = new char[4];
    type = 0;
    
    firstC = new Cylinder("",this->radius,this->radius_2,this->height,this->slices,this->stacks);
    secondC = new Cylinder("",this->radius_2,this->radius_2,this->height,this->slices,this->stacks);
    sph = new Sphere("", this->radius_2, this->slices, this->stacks);
    bigsph = new Sphere("", this->radius, this->slices, this->stacks);
    tor = new Torus("", 0.5, 1.0, 6, 6);
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

Piece::Piece(int slices,int stacks,float radius):CGFobject(),exists(true){
	elevation = 0.0;
	this->slices = slices;
	this->stacks= stacks;
	this->radius = radius;
	this->radius_2 = radius/2;
	this->height=1;
	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);
	player = '-';
	this->xPos = 0;
	this->yPos = 0;
	this->king = false;
	id = new char[4];
    type = 0;
    
    firstC = new Cylinder("",this->radius,this->radius_2,this->height,this->slices,this->stacks);
    secondC = new Cylinder("",this->radius_2,this->radius_2,this->height,this->slices,this->stacks);
    sph = new Sphere("", this->radius_2, this->slices, this->stacks);
    bigsph = new Sphere("", this->radius, this->slices, this->stacks);
    tor = new Torus("", 0.5, 1.0, 6, 6);
    
    glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   
}

Piece::~Piece(){
	delete(app);
	delete(text);
	delete(id);
	gluDeleteQuadric(quad);
}

void Piece::elevate(){
	elevation=1.0;
}

void Piece::un_elevate(){
	elevation=0.0;
}

void Piece::draw(){
	if(!exists) return;
    
    // default pieces
    if(type == 0)
    {
    glPushMatrix();
	glTranslatef(xPos*9/3-13.5+1.5,0,yPos*9/3-13.5+1.5);
	glRotatef(90,1,0,0);
	glRotatef(180,0,1,0);
    //+y is +z
    glTranslatef(0,0,elevation);
	
	glPushMatrix();{
        
		firstC->draw();
		glTranslatef(0,0.0,1.0);
		secondC->draw();
		glTranslatef(0,0.0,1.0);
		sph->draw();
	}
	glPopMatrix();

	drawBase();
	
	glPopMatrix();
    }
    // round pieces
    else if (type == 1)
    {
        glPushMatrix();
        glTranslatef(xPos*9/3-13.5+1.5,0,yPos*9/3-13.5+1.5);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        //+y is +z
        glTranslatef(0,0,elevation + radius);
        
        glPushMatrix();
        bigsph->draw();
        glPopMatrix();

    }
    // torus pieces
    else if (type == 2)
    {
            glEnable(GL_NORMALIZE);
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        
        glPushMatrix();
        glTranslatef(xPos*9/3-13.5+1.5,0,yPos*9/3-13.5+1.5);
        glRotatef(90,1,0,0);
        glRotatef(180,0,1,0);
        //+y is +z
        glTranslatef(0,0,elevation + 0.5);
        
        glPushMatrix();
        tor->draw();
        glPopMatrix();
        

            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
    
    }

}

void Piece::drawBase(){
	float angle_step = (float) 360.0 / this->slices;
	float angle = angle_step;
	float x, z;
	glPushMatrix();{
		glRotatef(90,1,0,0);
		glBegin(GL_POLYGON);{
			for(int i = 0; i < this->slices ;i++){
				angle += angle_step;
               
                glVertex2f(x, z);
                x = cos(angle*_RAD_TO_DEG)*this->radius_2;
				z = sin(angle*_RAD_TO_DEG)*this->radius_2;
                x = x * this->radius_2 + this->radius_2;
                z = z * this->radius_2 + this->radius_2;
                
                glTexCoord2f(x, z);
			}
		}glEnd();

	}glPopMatrix();
}

void Piece::setXPos(int x)
{
	this->xPos = x;
}

void Piece::setYPos(int y)
{
	this->yPos = y;
}

int Piece::getXPos()
{
	return this->xPos;
}

int Piece::getYPos()
{
	return this->yPos;
}