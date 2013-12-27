//
//  Ambient.cpp
//  LAIG - P3
//
//  Created by Maria Araújo on 27/12/13.
//  Copyright (c) 2013 me. All rights reserved.
//

#include "Ambient.h"

Ambient::Ambient(string id)
{
    this->id = id;
    wall = new Rectangle("wall", -20, -20, 20, 20);
    
    wallApp = new CGFappearance();
    kApp = new CGFappearance();
    sApp = new CGFappearance();
    mApp = new CGFappearance();
    tileGApp = new CGFappearance();
    tileKApp = new CGFappearance();
    tileSApp = new CGFappearance();
    tileMApp = new CGFappearance();
    
    // sets texture's wrap
    wallApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    kApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    sApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    mApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    tileGApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    tileKApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    tileSApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    tileMApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    
}

Ambient::~Ambient()
{
    delete(wall);
}

void Ambient::setTextures(string kingTexture, string sTexture, string mTexture, string wallsTexture, string gTileText, string sTileText, string mTileText, string kTileText)
{
    this->kingTexture = kingTexture;
    this->sTexture = sTexture;
    this->mTexture = mTexture;
    this->wallsTexture = wallsTexture;
    this->gTileText = gTileText;
    this->sTileText = sTileText;
    this->mTileText = mTileText;
    this->kTileText = kTileText;
    
    apply();
}

void Ambient::apply()
{
    // Apply current textures
    wallApp->setTexture(wallsTexture);
    kApp->setTexture(kingTexture);
    sApp->setTexture(sTexture);
    mApp->setTexture(mTexture);
    tileGApp->setTexture(gTileText);
    tileKApp->setTexture(kTileText);
    tileSApp->setTexture(sTileText);
    tileMApp->setTexture(mTileText);
    
    // Draw ambient walls
    drawWalls();
}

void Ambient::drawWalls()
{
    // Left wall
    glPushMatrix();
    wallApp->apply();
    wall->draw();
    glPopMatrix();
    
    // Right Wall
    glPushMatrix();
    wallApp->apply();
    wall->draw();
    glPopMatrix();
}