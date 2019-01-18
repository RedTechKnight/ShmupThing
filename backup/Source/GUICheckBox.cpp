#include "GUICheckBox.h"

GUICheckBox::GUICheckBox() : mX(0),mY(0),mWidth(0),mHeight(0),mChecked(false),mActive(false),mID(0)
{
}

GUICheckBox::~GUICheckBox()
{
}

void GUICheckBox::onHover()
{
    mActive = true;
}

void GUICheckBox::onClick()
{
    if(mChecked)
    {
        mChecked = false;
    }
    else if(!mChecked)
    {
        mChecked = true;
    }
}

void GUICheckBox::update()
{
    mActive = false;
}
