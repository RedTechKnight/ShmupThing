#include "Cursor.h"

Cursor::Cursor() : mX(0),mY(0),mPositionUpdated(false)
{

}

Cursor::~Cursor()
{

}


void Cursor::move(const float& xPos,const float& yPos)
{
    mPositionUpdated = true;
    mX = xPos/mScreenWidth;
    mY = yPos/mScreenHeight;
}
