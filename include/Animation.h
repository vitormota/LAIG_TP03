//
//  Animation.h
//  LAIG - P2
//
//  Created by Maria Araújo on 26/12/13.
//  Copyright (c) 2013 me. All rights reserved.
//

#ifndef __LAIG___P2__Animation__
#define __LAIG___P2__Animation__

#include <iostream>

class Animation{
public:
    Animation(float ixPos, float iyPos, float fxPos, float fyPos, float initTime);
    float ixPos, iyPos, fxPos, fyPos, initTime;
    float currentXPos, currentYPos, currentZPos;
    float velocityX, velocityY, velocityZ;
    bool animationActive;
    float movementSpeed;
    void animate(float currentTime);
};

#endif /* defined(__LAIG___P2__Animation__) */
