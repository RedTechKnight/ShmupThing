#include "GUIElement.h"

GUIElement::GUIElement() : mX(0),mY(0),mWidth(0),mHeight(0),mActive(false),mClicked(false),
mType(GUIElemType::Image),mElementID(0)
{

}

GUIElement::~GUIElement()
{

}

GUISystem::GUISystem() : mCursorX(0),mCursorY(0)
{
    mElements.clear();
}

GUISystem::~GUISystem()
{
    mElements.clear();
}


void GUISystem::addElement(const unsigned int& ID,std::unique_ptr<GUIElement>&& guiElem)
{
    mElements[ID] = std::move(guiElem);
}

void GUISystem::update(const float& cursorX,const float& cursorY)
{
    mCursorX = cursorX;
    mCursorY = cursorY;

    for(auto& elem: mElements)
    {
        elem.second->update();
        if(mCursorX > elem.second->mX && mCursorX < elem.second->mX+elem.second->mWidth)
        {
            if(mCursorY > elem.second->mY && mCursorY < elem.second->mY + elem.second->mHeight)
            {
                elem.second->onHover();
                if(mClicked)
                {
                    elem.second->onClick();
                }
            }
        }
    }
    mClicked = false;
}

void GUISystem::reset()
{
    mElements.clear();
}
