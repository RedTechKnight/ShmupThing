#include "GUISystem.h"

GUISystem::GUISystem() : mCursorX(0),mCursorY(0),mClicked(false)
{
    mButtons.clear();
    mCheckboxes.clear();
}

GUISystem::~GUISystem()
{
    mButtons.clear();
    mCheckboxes.clear();
}

void GUISystem::update(const float& cursX,const float& cursY)
{
    mCursorX = cursX;
    mCursorY = cursY;

    for(auto& button: mButtons)
    {
        button.update();
        if(mCursorX > button.mX && mCursorX < (button.mWidth+button.mX))
        {
            if(mCursorY > button.mY && mCursorY < (button.mHeight+button.mY))
            {
                button.onHover();
                if(mClicked)
                {
                    button.onClick();
                }
            }
        }
    }

    for(auto& checkbox: mCheckboxes)
    {
        checkbox.update();
        if(mCursorX > checkbox.mX && mCursorX < (checkbox.mX+checkbox.mWidth))
        {
            if(mCursorY > checkbox.mY && mCursorY < (checkbox.mY+checkbox.mHeight))
            {
                checkbox.onHover();
                if(mClicked)
                {
                    checkbox.onClick();
                }
            }
        }
    }
    mClicked = false;
}

