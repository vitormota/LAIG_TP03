#include "Piece.h"
#include <math.h>

#define _DEFAULT_STACKS_SLICES 12
#define _USE_MATH_DEFINES
#define _RAD_TO_DEG M_PI / 180.0

Piece::Piece():CGFobject(),exists(true){
	this->elevation = 0.0;
	this->quad = gluNewQuadric();
	this->slices = _DEFAULT_STACKS_SLICES;
	this->stacks= _DEFAULT_STACKS_SLICES;
	this->radius = 1.0;
	this->radius_2 = radius/2;
	this->height=1;
	this->player = '-';
	this->xPos = 0;
	this->yPos = 0;
	this->king = false;
	this->id = new char[4];
	this->type = 0;
    this->animating = false;
    this->currentX = 0;
    this->currentY = 0;
    
	firstC = new Cylinder("",this->radius,this->radius_2,this->height,this->slices,this->stacks);
	secondC = new Cylinder("",this->radius_2,this->radius_2,this->height,this->slices,this->stacks);
	sph = new Sphere("", this->radius_2, this->slices, this->stacks);
	bigsph = new Sphere("", this->radius, this->slices, this->stacks);
	tor = new Torus("", 0.5, 1.0, 6, 6);
    
    gluQuadricNormals(quad, GLU_SMOOTH);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

Piece::Piece(int slices,int stacks,float radius):CGFobject(),exists(true){
	this->elevation = 0.0;
	this->slices = slices;
	this->stacks= stacks;
	this->radius = radius;
	this->radius_2 = radius/2;
	this->height=1;
	this->quad = gluNewQuadric();
	this->player = '-';
	this->xPos = 0;
	this->yPos = 0;
	this->king = false;
	this->id = new char[4];
	this->type = 0;
    this->animating = false;
    this->currentX = 0;
    this->currentY = 0;
    
	firstC = new Cylinder("",this->radius,this->radius_2,this->height,this->slices,this->stacks);
	secondC = new Cylinder("",this->radius_2,this->radius_2,this->height,this->slices,this->stacks);
	sph = new Sphere("", this->radius_2, this->slices, this->stacks);
	bigsph = new Sphere("", this->radius, this->slices, this->stacks);
	tor = new Torus("", 0.5, 1.0, 6, 6);
    
    gluQuadricNormals(quad, GLU_SMOOTH);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
    
}

Piece::~Piece(){
    delete(firstC);
    delete (secondC);
    delete (sph);
    delete (bigsph);
    delete (tor);
	gluDeleteQuadric(quad);
}

void Piece::elevate(){
	this->elevation = 1.0;
}

void Piece::lower(){
    this->elevation = 0.0;
}

void Piece::draw(){
	//if(!exists) return;
    
	// default pieces
	if(type == 0)
	{
		glPushMatrix();
        if(animating)
        {
            glTranslatef(currentX,0,currentY);
        }
        else{
            glTranslatef(xPos*9/3-13.5+1.5,0,yPos*9/3-13.5+1.5);
        }
		glRotatef(90,1,0,0);
		glRotatef(180,0,1,0);
		glTranslatef(0,0,elevation); //+y is +z
        
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
        if(animating)
        {
            glTranslatef(currentX,0,currentY);
        }
        else{
            glTranslatef(xPos*9/3-13.5+1.5,0,yPos*9/3-13.5+1.5);
        }
		glRotatef(90,1,0,0);
		glRotatef(180,0,1,0);
		glTranslatef(0,0,elevation + radius); //+y is +z
        
		glPushMatrix();
		bigsph->draw();
		glPopMatrix();
        
        glPopMatrix();
	}
	// torus pieces
	else if (type == 2)
	{
		glEnable(GL_NORMALIZE);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
        
		glPushMatrix();
		if(animating)
        {
            glTranslatef(currentX,0,currentY);
        }
        else{
            glTranslatef(xPos*9/3-13.5+1.5,0,yPos*9/3-13.5+1.5);
        }
		glRotatef(90,1,0,0);
		glRotatef(180,0,1,0);
		glTranslatef(0,0,elevation + 0.5); //+y is +z
        
		glPushMatrix();
		tor->draw();
		glPopMatrix();
        
		glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
        
        glPopMatrix();
        
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

void Piece::setCurrentX(double currentX)
{
    this->currentX = currentX;
}

void Piece::setCurrentY(double currentY)
{
    this->currentY = currentY;
}