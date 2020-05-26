#include "../headers/sdl_input.h"

InputHandler::InputHandler() {
  input_states.clear();
  input_map.clear();
}

InputHandler::~InputHandler() {
  input_states.clear();
  input_map.clear();
}

void InputHandler::add_action(const SDL_Keycode &keycode,
                              const unsigned int &action) {
  if (!input_map[keycode]) {
    input_map[keycode] = action;
    input_state state;
    state.held = false;
    state.pressed = false;
    state.released = false;
    input_states[action] = state;
  }
}

void InputHandler::handle_input(const SDL_Event &event) {
  input_state &state = input_states[input_map[event.key.keysym.sym]];
  if (state.released)
    state.released = false;
  if (event.type == SDL_KEYDOWN) {
    if (!state.pressed && !event.key.repeat) {
      state.pressed = true;
      state.released = false;
    }
    if (event.key.repeat) {
      state.pressed = true;
      state.held = true;
      state.released = false;
    }
  } else if (event.type == SDL_KEYUP) {
    if (state.pressed) {
      state.pressed = false;
      state.held = false;
      state.released = true;
    }
  }
}

bool InputHandler::is_key_just_pressed(const unsigned int &key) {
  input_state &state = input_states[key];
  if (state.pressed) {
    if (!state.held) {

      state.pressed = false;
      return true;
    }
    return false;
  }
  return false;
}

bool InputHandler::is_key_held(const unsigned int &key) {
  input_state &state = input_states[key];
  if (state.pressed) {
    if (state.held) {
      return true;
    }
    return true;
  }
  return false;
}

bool InputHandler::is_key_released(const unsigned int &key) {
  input_state &state = input_states[key];
  if (state.released) {
    state.released = false;
    return true;
  }
  return false;
}
