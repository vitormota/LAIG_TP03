//
//  Animation.cpp
//  LAIG - P2
//
//  Created by Maria Araújo on 26/12/13.
//  Copyright (c) 2013 me. All rights reserved.
//

#include "Animation.h"

Animation::Animation(float ixPos, float iyPos, float fxPos, float fyPos, float time)
{
    this->ixPos = ixPos;
    this->iyPos = iyPos;
    this->fxPos = fxPos;
    this->fyPos = fyPos;
    this->initTime = time;
    
    // Displacement in x and z axis
    float dx = fxPos - ixPos;
    float dz = fyPos - iyPos;
    
    this->velocityX = dx;
    this->velocityY = 0;
    this->velocityZ = dz;
    this->movementSpeed = 0.5;
}

void Animation::animate(float currentTime)
{
    this->animationActive = true;
    
    // Time displacement -> time passed
    float dt = currentTime - this->initTime;
    
    // Calculate current position
    this->currentXPos = ixPos + movementSpeed * velocityX * dt;
    this->currentYPos = 0;
    this->currentZPos = iyPos + movementSpeed * velocityZ * dt;
    
    // Check if the animation has ended
    if(((this->currentXPos < fxPos) && (velocityX < 0)) || ((currentXPos > fxPos) && (velocityX > 0)))
    {
        currentXPos = fxPos;
        this->animationActive = false;
    }
    else if(((this->currentZPos < fyPos) && (velocityZ < 0)) || ((velocityZ > 0) && (currentZPos > fyPos)))
    {
        currentZPos = fyPos;
        this->animationActive = false;
    }

}