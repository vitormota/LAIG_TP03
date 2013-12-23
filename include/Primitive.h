#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include <string>
#ifdef __APPLE__
#include <GLUT/GLUT.h>
#else
#include <GL\glew.h>
#include <GL/glut.h>
#endif
#ifdef __APPLE__
#include <gl/glui.h>
#else
#include <GL/glui.h>
#endif

using std::string;
using std::vector;

namespace scene{

	class Primitive
	{
	private:
		string id;
		float texlength_s;
		float texlength_t;

	public:
		Primitive();
		Primitive(string id);
		~Primitive();
		void setId(string id);
		void setTexlength_s(float texlength_s);
		void setTexlength_t(float texlength_t);
		string getId();
		float getTexlength_s();
		float getTexlength_t();
		virtual void draw(){}
		float *get_normal_newell(float **vertices, int size);
		float *getSurfaceNormalTriangle (const float x[], const float y[], const float z[], int numPontos);
	};

	/* Rectangle */
	class Rectangle: public Primitive{

	private:
		float x1, y1, x2, y2;
		float **verticesRectangle; // vertices of the rectangle primitive
		float *normalRectangle; // normal of the rectangle primitive
		float height,length;

	public:
		Rectangle(string id,float x1, float y1, float x2, float y2);
		virtual void draw();
	};

	/* Triangle */
	class Triangle: public Primitive{

	private:
		float x1, y1, z1, x2, y2, z2, x3, y3, z3;
		float **verticesTriangle; // vertices of the triangle primitive
		float *normalTriangle; // normal of the triangle primitive
		float a,b,c;
		float cos_beta, sin_beta;
		float p0[2], p1[2], p2[2];

	public:
		Triangle(string id, float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);
		virtual void draw();
	};

	/* Cylinder */
	class Cylinder: public Primitive{

	private:
		float base, top, height;
		int slices, stacks;
		GLUquadric* cylinderQuadric;

	public:
		Cylinder(string id, float base, float top, float height, int slices, int stacks);
		~Cylinder();
		virtual void draw();
	};

	/* Sphere */
	class Sphere: public Primitive{

	private:
		float radius;
		int slices, stacks;
		GLUquadric* sphereQuadric;

	public:
		Sphere(string id, float radius, int slices, int stacks);
		virtual void draw();
	};

	/* Torus */
	class Torus: public Primitive{

	private:
		float inner, outer;
		int slices, loops;

	public:
		Torus(string id, float inner, float outer, int slices, int loops);
		virtual void draw();
	};
	
	/* Patch */
	class Patch: public Primitive{
		
	private:
		int order, partsU, partsV;
		string compute;
		vector<vector<float>> controlPoints;
		GLfloat vecControlPointsOrder1[2][3];
		GLfloat vecControlPointsOrder2[4][3];
		GLfloat vecControlPointsOrder3[9][3];
		GLfloat textPoints[4][2];
		
	public:
		Patch(string id, int order, int partsU, int partsV, string compute, vector<vector<float>> controlPoints);
		void setControlPoints(vector<vector<float>> controlPoints);
		vector<vector<float>> getControlPoints();
		virtual void draw();
	};
	
	/* Plane */
	class Plane: public Primitive{
		
	private:
		int parts;
		GLfloat textPoints[4][2];
		GLdouble ctrlpoints[4][3];
		Patch* planePatch;
		
	public:
		Plane(string id, int parts);
		~Plane();
		virtual void draw();
	};
	
	/* Vehicle */
	class Vehicle: public Primitive{
		
	private:
		vector<vector<float>> vehicleControlPoints;
		
	public:
		Vehicle(string id);
		~Vehicle();
		virtual void draw();
		Patch* plane1;
	};
}





#endif