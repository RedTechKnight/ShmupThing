#include "GUIButton.h"

GUIButton::GUIButton() : GUIElement()
{

}

GUIButton::~GUIButton()
{

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
