#ifndef GUIBUTTON_H_INCLUDED
#define GUIBUTTON_H_INCLUDED
#include <iostream>
#include <functional>
#include "GUIElement.h"
class GUIButton : public GUIElement
{
private:

public:
    GUIButton();
    ~GUIButton();

    virtual void onHover();
    virtual void onClick();
    virtual void update();
};

#endif // GUIBUTTON_H_INCLUDED
