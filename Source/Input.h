#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include "Dragon_SDLGL.h"
#include <map>

struct inputState
{
    bool pressed;
    bool held;
    bool released;
};

class InputHandler
{
private:
    std::map<unsigned int,inputState> mInputStates;
    std::map<SDL_Keycode,unsigned int> mInputMap;
public:


    InputHandler();
    ~InputHandler();

    void addAction(const SDL_Keycode& keycode,const unsigned int& action);
    void handleInput(const SDL_Event& eventHandle);
    bool keyJustPressed(const unsigned int& key);
    bool keyHeld(const unsigned int& key);
    bool keyReleased(const unsigned int& key);
};


#endif // INPUT_H_INCLUDED
