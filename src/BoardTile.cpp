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
    this->r = new scene::Rectangle("",-5, -5, 5, 5);
}

BoardTile::BoardTile(string id) {
    this->id = id;
    this->r = new scene::Rectangle(id,-5, -5, 5, 5);
}

BoardTile::~BoardTile() {
    
}

void BoardTile::setId(string id) {
    this->id = id;
}

string BoardTile::getId()
{
    return this->id;
}
void BoardTile::draw()
{
    r->scene::Rectangle::draw();
}


