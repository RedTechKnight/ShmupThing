#include "Input.h"

InputHandler::InputHandler()
{
    mInputStates.clear();
    mInputMap.clear();
}

InputHandler::~InputHandler()
{
    mInputStates.clear();
    mInputMap.clear();
}

void InputHandler::addAction(const SDL_Keycode& keycode,const unsigned int& action)
{
    if(!mInputMap[keycode])
    {
        mInputMap[keycode] = action;
        inputState state;
        state.held = false;
        state.pressed = false;
        state.released = false;
        mInputStates[action] = state;
    }
}

void InputHandler::handleInput(const SDL_Event& event)
{
    inputState& state = mInputStates[ mInputMap[event.key.keysym.sym] ];
        if(state.released)
            state.released = false;
        if(event.type == SDL_KEYDOWN)
        {
            if(!state.pressed && !event.key.repeat)
            {
                state.pressed = true;
                state.released = false;
            }
            if(event.key.repeat)
            {
                state.pressed = true;
                state.held = true;
                state.released = false;
            }
        }
        else if(event.type == SDL_KEYUP)
        {
            if(state.pressed)
            {
                state.pressed = false;
                state.held = false;
                state.released = true;
            }
        }

}

bool InputHandler::keyJustPressed(const unsigned int& key)
{
        inputState& state = mInputStates[key];
        if(state.pressed)
        {
            if(!state.held)
            {

                state.pressed = false;
                return true;
            }
            return false;
        }
        return false;
}

bool InputHandler::keyHeld(const unsigned int& key)
{
        inputState& state = mInputStates[key];
        if(state.pressed)
        {
            if(state.held)
            {
                return true;
            }
            return true;
        }
        return false;
}

bool InputHandler::keyReleased(const unsigned int& key)
{
        inputState& state = mInputStates[key];
        if(state.released)
        {
            state.released = false;
            return true;
        }
        return false;
}
