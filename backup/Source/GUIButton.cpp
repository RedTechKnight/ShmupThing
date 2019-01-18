#include "GUIButton.h"

GUIButton::GUIButton()
{
    mActive = false;
    mClicked = false;
}

GUIButton::~GUIButton()
{
    mActive = false;
    mClicked = false;
}

void GUIButton::onHover()
{
    mActive = true;
}

void GUIButton::onClick()
{
    mClicked = true;
}

void GUIButton::update()
{
    mActive = false;
    mClicked = false;
}
