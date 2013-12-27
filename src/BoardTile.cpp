//
//  BoardTile.cpp
//  LAIG - P2
//
//  Created by Maria Araújo on 17/12/13.
//  Copyright (c) 2013 me. All rights reserved.
//

#include "BoardTile.h"
#define BOARD_PIECE_SIZE 1


BoardTile::BoardTile() {
    this->r = new Rectangle("",-1.5, -1.5, 1.5, 1.5);
    this->r->setTexlength_s(5);
    this->r->setTexlength_t(5);
}

BoardTile::~BoardTile() {
    
}

void BoardTile::draw()
{
    glPushMatrix();
    glTranslatef(xPos*9/3-13.5+1.5,0, yPos*9/3-13.5+1.5);
    glRotatef(90,1,0,0);
    glRotatef(180,0,1,0);
    r->draw();
    glPopMatrix();
}

void BoardTile::setXPos(int x)
{
    this->xPos = x;
}

void BoardTile::setYPos(int y)
{
    this->yPos = y;
}

int BoardTile::getXPos()
{
    return this->xPos;
}

int BoardTile::getYPos()
{
    return this->yPos;
}


