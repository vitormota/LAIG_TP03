#include "Ambient.h"

Ambient::Ambient(string id)
{
    this->id = id;
    
    skyApp = new CGFappearance();
    kApp = new CGFappearance();
    sApp = new CGFappearance();
    mApp = new CGFappearance();
    tileGApp = new CGFappearance();
    tileKApp = new CGFappearance();
    tileSApp = new CGFappearance();
    tileMApp = new CGFappearance();
    
    // sets texture's wrap
    skyApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    kApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    sApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    mApp->setTextureWrap(GL_CLAMP, GL_CLAMP);
    tileGApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    tileKApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    tileSApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    tileMApp->setTextureWrap(GL_REPEAT, GL_REPEAT);
    
    // sky
    sphereQuadric = gluNewQuadric();
    gluQuadricDrawStyle(sphereQuadric, GLU_FILL);
    gluQuadricNormals(sphereQuadric, GLU_SMOOTH);
    gluQuadricOrientation(sphereQuadric, GLU_INSIDE);
    gluQuadricTexture(sphereQuadric, GL_TRUE);
    
}

Ambient::~Ambient()
{
    delete(skyApp);
    delete(kApp);
    delete(sApp);
    delete(mApp);
    delete(tileGApp);
    delete(tileKApp);
    delete(tileSApp);
    delete(tileMApp);
}

void Ambient::setTextures(string kingTexture, string sTexture, string mTexture, string skyTexture, string gTileText, string sTileText, string mTileText, string kTileText)
{
    this->kingTexture = kingTexture;
    this->sTexture = sTexture;
    this->mTexture = mTexture;
    this->skyTexture = skyTexture;
    this->gTileText = gTileText;
    this->sTileText = sTileText;
    this->mTileText = mTileText;
    this->kTileText = kTileText;
    
    apply();
}

string Ambient::getID()
{
    return this->id;
}

void Ambient::setPiecesType(int type)
{
    this->piecesType = type;
}

int Ambient::getPiecesType()
{
    return this->piecesType;
}

void Ambient::apply()
{
    // Apply current textures
    skyApp->setTexture(skyTexture);
    kApp->setTexture(kingTexture);
    sApp->setTexture(sTexture);
    mApp->setTexture(mTexture);
    tileGApp->setTexture(gTileText);
    tileKApp->setTexture(kTileText);
    tileSApp->setTexture(sTileText);
    tileMApp->setTexture(mTileText);
}

void Ambient::drawSky()
{
    glPushMatrix();
    skyApp->apply();
    glScaled(5, 5, 5);
    glRotated(90, 1, 0, 0);
    gluSphere(sphereQuadric, 10, 10, 10);
    glPopMatrix();
    
}