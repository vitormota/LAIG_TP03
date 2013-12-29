#ifndef RECT
#define RECT

#include "Primitive.h"
#include <string>

class Rect: public Primitive{

	private:
		float x1, y1, x2, y2;
		float **verticesRectangle; // vertices of the rectangle primitive
		float *normalRectangle; // normal of the rectangle primitive
		float height,length;

	public:
		Rect(std::string id,float x1, float y1, float x2, float y2);
		virtual void draw();
	};

#endif