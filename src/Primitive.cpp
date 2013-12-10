#include "Primitive.h"

#include <GL\glui.h>

Rectangle::Rectangle(float x1,float y1, float x2, float y2) 
{
	this->x1 = x1;
	this->x2 = x2; 
	this->y1 = y1; 
	this->y2 = y2;

}

void Rectangle::draw(float s, float t)
{
	//glNormal3f(normal->x,normal->y,normal->z);
	glBegin(GL_QUADS);
		if( s!=0 && t!=0) glTexCoord2f(0.0,0.0);
		glVertex2f(x1,y1);
		if( s!=0 && t!=0) glTexCoord2f(x2/s,0.0);
		glVertex2f(x2,y1);
		if( s!=0 && t!=0) glTexCoord2f(x2/s,y2/t);
		glVertex2f(x2,y2);
		if( s!=0 && t!=0) glTexCoord2f(0.0,y2/t);
		glVertex2f(x1,y2);
	glEnd();
}

Triangle::Triangle(float x1,float y1, float z1, float x2, float y2,float z2,float x3,float y3, float z3)
{
	this->x1=x1;
	this->x2 =x2;
	this->x3=x3;
	this->y1=y1;
	this->y2 =y2;
	this->y3=y3;
	this->z1=z1;
	this->z2 =z2;
	this->z3=z3;
}

void Triangle::draw(float s, float t)
{
	//glNormal3f(normal->x,normal->y,normal->z);
	glBegin(GL_TRIANGLES);
		if( s!=0 && t!=0) glTexCoord2f(0.0,0.0);
		glVertex3f(x1,y1,z1);
		if( s!=0 && t!=0) glTexCoord2f(x2/s,y2/t);
		glVertex3f(x2,y2,z2);
		if( s!=0 && t!=0) glTexCoord2f(x3/s,y3/t);
		glVertex3f(x3,y3,z3);
	glEnd();
}

Cylinder::Cylinder(float base,float top,float height,int slices,int stacks)
{
	this->base = base;
	this->top=top;
	this->height=height;
	this->slices=slices;
	this->stacks=stacks;
}

void Cylinder::draw(float s, float t)
{
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluCylinder(quadric,base,top,height,slices,stacks);
	glPushMatrix();
	glRotatef(180,0,1,0);
	gluDisk(quadric,0,base,slices,stacks);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0,0,height);
	gluDisk(quadric,0,top,slices,stacks);
	glPopMatrix();
}

Sphere::Sphere(float radius,int stacks,int slices)
{
	this->radius=radius;
	this->stacks=stacks;
	this->slices=slices;
}

void Sphere::draw(float s, float t)
{
	GLUquadricObj *quadric;
	quadric = gluNewQuadric();

	gluQuadricTexture(quadric, GL_TRUE);
	gluSphere(quadric,radius,slices,stacks);
}

Torus::Torus(float inner, float outer, int slices, int loops)
{
	this->inner = inner;
	this->outer=outer;
	this->slices=slices;
	this->loops=loops;
}

void Torus::draw(float s, float t)
{
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glutSolidTorus(inner,outer,slices,loops);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

Plane::Plane(int strides)
{
	this->strides = strides;
	this->size=1;
	
}

Plane::Plane(int strides, float size)
{
	this->strides = strides;
	this->size=size;

}



void Plane::draw(float s, float t){
	
	float ctrlpoints[4][3] = {	{  -0.5, 0, -0.5},
							{  0.5, 0, -0.5}, 
							{ -0.5, 0, 0.5},
							{ 0.5, 0, 0.5} };

	float nrmlcompon[4][3] = {	{  0.0, 1.0,0},
							{  0.0, 1.0, 0}, 
							{  0.0, 1.0, 0},
							{  0.0, 1.0, 0} };

	float textpoints[4][2] = {	{ 0.0, 1/size},
								{ 1/size, 1/size}, 
								{ 0.0, 0.0},
								{ 1/size, 0.0} };



	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlpoints[0][0]);
	glMap2f(GL_MAP2_NORMAL, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints[0][0]);


	glMapGrid2f(strides,size,0,strides,size,0);
	glEvalMesh2(GL_FILL,0,strides,0,strides);

	glDisable(GL_MAP2_VERTEX_3);
	glDisable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_TEXTURE_COORD_2);
}