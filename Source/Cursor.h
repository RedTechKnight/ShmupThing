#ifndef CURSOR_H_INCLUDED
#define CURSOR_H_INCLUDED

class Cursor
{
private:

public:
    float mX,mY;
    bool mPositionUpdated;
    float mScreenWidth;
    float mScreenHeight;
    Cursor();
    ~Cursor();

    void move(const float& xPos,const float& yPos);
};

#endif // CURSOR_H_INCLUDED
