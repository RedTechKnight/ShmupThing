#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED
#include "sdl_gl_window.h"
#include <map>

struct input_state {
  bool pressed;
  bool held;
  bool released;
};

class InputHandler {
private:
  std::map<unsigned int, input_state> input_states;
  std::map<SDL_Keycode, unsigned int> input_map;

public:
  InputHandler();
  ~InputHandler();

  void add_action(const SDL_Keycode &keycode, const unsigned int &action);
  void handle_input(const SDL_Event &eventHandle);
  bool is_key_just_pressed(const unsigned int &key);
  bool is_key_held(const unsigned int &key);
  bool is_key_released(const unsigned int &key);
};

#endif // INPUT_H_INCLUDED
