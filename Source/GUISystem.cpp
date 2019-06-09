#include "GUISystem.h"
#include <algorithm>
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
        auto conds = std::vector<bool>{
            mCursorX > button.mX,
            mCursorX < (button.mWidth+button.mX),
            mCursorY > button.mY,
            mCursorY < (button.mHeight+button.mY)
        };
        if(std::all_of(conds.begin(),conds.end(), [](bool a) -> bool {return a;} ))
        {
            button.onHover();
            if(mClicked){
                button.onClick();
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

