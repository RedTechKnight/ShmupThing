#ifndef GUI_SYS_H
#define GUI_SYS_H
#include <vector>
#include "GUIButton.h"
#include "GUICheckBox.h"

class GUISystem
{
private:

public:
    GUISystem();
    ~GUISystem();

    std::vector<GUIButton> mButtons;
    std::vector<GUICheckBox> mCheckboxes;

    float mCursorX,mCursorY;
    bool mClicked;

    void update(const float& cursX,const float& cursY);
};

#endif