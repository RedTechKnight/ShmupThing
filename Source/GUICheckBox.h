#ifndef GUICHECKBOX_H_INCLUDED
#define GUICHECKBOX_H_INCLUDED
#include <iostream>
#include "GUIElement.h"

class GUICheckBox : public GUIElement
{
private:
public:
    GUICheckBox();
    ~GUICheckBox();

    virtual void onHover();
    virtual void onClick();
    virtual void update();
};


#endif // GUICHECKBOX_H_INCLUDED
