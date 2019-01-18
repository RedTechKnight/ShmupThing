#ifndef GUIBUTTON_H_INCLUDED
#define GUIBUTTON_H_INCLUDED
#include <iostream>
#include <functional>
enum class GUIElementType
{
    Text,
    Image
};
class GUIButton 
{
private:

public:
    GUIButton();
    ~GUIButton();

    void onHover();
    void onClick();
    void update();

    GUIElementType mType;
    bool mActive;
    bool mClicked;
    unsigned int mID;
    float mX,mY;
    float mWidth,mHeight;
};

#endif // GUIBUTTON_H_INCLUDED
