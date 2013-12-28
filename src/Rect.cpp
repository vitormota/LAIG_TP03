#include "Rect.h"

Rect::Rect(std::string id, float x1, float y1, float x2, float y2) : Primitive(id) {
	this->x1 = x1;
	this->x2 = x2;
	this->y1 = y1;
	this->y2 = y2;
	this->verticesRectangle = new float*[4];
	this->verticesRectangle[0] = new float[3];
	this->verticesRectangle[1] = new float[3];
	this->verticesRectangle[2] = new float[3];
	this->verticesRectangle[3] = new float[3];
	this->verticesRectangle[0][0] = x1;
	this->verticesRectangle[0][1] = y1;
	this->verticesRectangle[0][2] = 0;
	this->verticesRectangle[1][0] = x2;
	this->verticesRectangle[1][1] = y1;
	this->verticesRectangle[1][2] = 0;
	this->verticesRectangle[2][0] = x2;
	this->verticesRectangle[2][1] = y2;
	this->verticesRectangle[2][2] = 0;
	this->verticesRectangle[3][0] = x1;
	this->verticesRectangle[3][1] = y2;
	this->verticesRectangle[3][2] = 0;
	this->normalRectangle = get_normal_newell(verticesRectangle, 4);

	this->length = fabs(x2-x1);
	this->height = fabs(y2-y1);

}


void Rect::draw() {

	glBegin(GL_QUADS);

	glNormal3f(normalRectangle[0], normalRectangle[1], normalRectangle[2]);

	glTexCoord2f(0, 0);
	glVertex3f(x1, y1, 0);
	glTexCoord2f(length/getTexlength_s(), 0);
	glVertex3f(x2, y1, 0);
	glTexCoord2f(length/getTexlength_s(), height/getTexlength_t());
	glVertex3f(x2, y2, 0);
	glTexCoord2f(0, height/getTexlength_t());
	glVertex3f(x1, y2, 0);

	glEnd();
}