#include "GUICheckBox.h"

GUICheckBox::GUICheckBox() : GUIElement()
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
    if(mClicked)
    {
        mClicked = false;
    }
    if(!mClicked)
    {
        mClicked = true;
    }
}

void GUICheckBox::update()
{
    mClicked = false;
}
