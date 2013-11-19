#ifndef CUBE_H
#define CUBE_H

//Windows specific includes
#if _WIN32
#include <CGFobject.h>
#include <CGFtexture.h>
#endif
//end

//general includes
#include <string>
//end

namespace Primitive{

	//3D rectangle points
	const GLfloat rectangle[]= {
		0.0, 0.0, 0.0,
		1.0, 0.0, 0,0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0
	};

	class Cube : public CGFobject{
	public:
		Cube();
		Cube(int definition,bool smooth,string textfile);
		~Cube();
		void draw();
		void setTexture(string file);
	private:
		void drawFace();
		int definition;
		bool smooth;
		CGFtexture *texture;
	};

}



#endif