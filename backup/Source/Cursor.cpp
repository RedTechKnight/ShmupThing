#include "Cursor.h"

GUICursor::GUICursor() : mX(0),mY(0),mPositionUpdated(false)
{

}

GUICursor::~GUICursor()
{

}


void GUICursor::move(const float& xPos,const float& yPos)
{
    mPositionUpdated = true;
    mX = xPos/mScreenWidth;
    mY = yPos/mScreenHeight;
}
