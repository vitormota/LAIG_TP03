#ifndef __LAIG___P2__BoardTile__
#define __LAIG___P2__BoardTile__

#include <iostream>
#include "Primitive.h"

using std::string;
class BoardTile
{
private:
    Rectangle* r;
    int xPos, yPos;
    
public:
    BoardTile();
    ~BoardTile();
    void setXPos(int x);
    void setYPos(int y);
    int getXPos();
    int getYPos();
    void draw();
};

#endif /* defined(__LAIG___P2__BoardTile__) */
