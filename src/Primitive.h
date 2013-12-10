#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <string>

class Primitive{
protected:
	
public:
	Primitive(){};
	virtual void draw(float s, float t){};
};

class Rectangle: public Primitive
{
	float x1,y1,x2,y2;
public:
	Rectangle(float x1,float y1, float x2, float y2) ;
	void draw(float s, float t);
};

class Triangle: public Primitive
{
	float x1,y1,z1,x2,y2,z2,x3,y3,z3;
public:
	Triangle(float x1,float y1, float z1, float x2, float y2,float z2,float x3,float y3, float z3);
	void draw(float s, float t);
};

class Cylinder: public Primitive
{
	float base, top,height;
	int slices,stacks;
public:
	 Cylinder(float,float,float,int,int);
	 void draw(float s, float t);
};

class Sphere: public Primitive
{
	float radius;
	int stacks,slices;
public:
	Sphere(float,int,int);
	void draw(float s, float t);
};

class Torus: public Primitive
{
	float inner,outer;
	int slices,loops;
public:
	Torus(float,float,int,int);
	void draw(float s, float t);
};

class Plane: public Primitive
{
	int strides;
	float ctrlPoints[4][3], nrmlcompon[4][3], textpoints[4][2], size;
public:
	Plane(int);
	Plane(int,float);
	void draw(float s, float t);
};

#endif