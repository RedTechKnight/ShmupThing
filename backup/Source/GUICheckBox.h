#ifndef GUICHECKBOX_H_INCLUDED
#define GUICHECKBOX_H_INCLUDED
#include <iostream>
#include "GUIButton.h"
class GUICheckBox
{
private:
    
public:
    GUICheckBox();
    ~GUICheckBox();

    void onHover();
    void onClick();
    void update();

    float mX,mY,mWidth,mHeight;
    bool mChecked;
    bool mActive;
    unsigned int mID;
    GUIElementType mType;
};


#endif // GUICHECKBOX_H_INCLUDED
