#ifndef GUIELEMENT_H_INCLUDED
#define GUIELEMENT_H_INCLUDED
#include <memory>
#include <map>
enum class GUIElemType
{
    Text,
    Image
};
class GUIElement
{
private:

public:
    float mX,mY;
    float mWidth,mHeight;
    bool mActive;
    bool mClicked;
    GUIElemType mType;
    unsigned int mElementID;

    GUIElement();
    ~GUIElement();

    virtual void onHover() = 0;
    virtual void onClick() = 0;
    virtual void update() = 0;
};

class GUISystem
{
private:

public:
    float mCursorX;
    float mCursorY;
    bool mClicked;
    std::map<unsigned int, std::unique_ptr<GUIElement> > mElements;

    GUISystem();
    ~GUISystem();

    void addElement(const unsigned int& ID,std::unique_ptr<GUIElement>&& newElement);
    void update(const float& cursorX,const float& cursorY);
    void reset();
};

#endif // GUIELEMENT_H_INCLUDED
