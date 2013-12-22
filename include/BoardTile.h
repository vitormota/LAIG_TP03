//
//  BoardTile.h
//  LAIG - P2
//
//  Created by Maria Araújo on 17/12/13.
//  Copyright (c) 2013 me. All rights reserved.
//

#ifndef __LAIG___P2__BoardTile__
#define __LAIG___P2__BoardTile__

#include <iostream>
#include "Primitive.h"

using std::string;
class BoardTile
{
private:
    string id;
    scene::Rectangle* r;
    
public:
    BoardTile();
    BoardTile(string id);
    ~BoardTile();
    void setId(string id);
    string getId();
    void draw();
};

#endif /* defined(__LAIG___P2__BoardTile__) */
