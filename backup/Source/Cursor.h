#ifndef GUICursor_H_INCLUDED
#define GUICursor_H_INCLUDED

class GUICursor
{
private:

public:
    float mX,mY;
    bool mPositionUpdated;
    float mScreenWidth;
    float mScreenHeight;
    GUICursor();
    ~GUICursor();

    void move(const float& xPos,const float& yPos);
};

#endif // GUICursor_H_INCLUDED
