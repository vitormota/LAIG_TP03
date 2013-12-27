//
//  Ambient.h
//  LAIG - P3
//
//  Created by Maria Araújo on 27/12/13.
//  Copyright (c) 2013 me. All rights reserved.
//

#ifndef __LAIG___P3__Ambient__
#define __LAIG___P3__Ambient__

#include <iostream>
#include "Primitive.h"

//Windows specific includes
#if _WIN32
#include <CGFappearance.h>
#endif
//end

//MAC OSX specific includes
#ifdef __APPLE__
#include "CGFappearance.h"
#endif
//end

using std::string;
class Ambient
{
private:
    string id;
    string kingTexture, sTexture, mTexture;
    string wallsTexture;
    string gTileText, sTileText, mTileText, kTileText;
    Primitive* wall;
    int piecesType;
    
public:
    Ambient(string id);
    ~Ambient();
    string getID();
    int getPiecesType();
    void setPiecesType(int type);
    
    void setTextures(string kingTexture, string sTexture, string mTexture, string wallsTexture, string gTileText, string sTileText, string mTileText, string kTileText);
    
    CGFappearance* wallApp;
    CGFappearance* kApp;
    CGFappearance* sApp;
    CGFappearance* mApp;
    CGFappearance* tileGApp;
    CGFappearance* tileKApp;
    CGFappearance* tileSApp;
    CGFappearance* tileMApp;
    
    void apply();
    void drawWalls();
};

#endif /* defined(__LAIG___P3__Ambient__) */
