#pragma once

#include <vector>
#include <iostream>
#include "global.hpp"


class InputManager {

  bool edge_key_[2][5];
  bool press_key_[2][5];

 public:
  InputManager()  {}

   void update()  {
    int num_devices_ = 2;
    bool new_press_key[num_devices_][5];
    for (int i = 0; i < num_devices_; ++i) {
      for (int j = 0; j < 5; ++j) {
        new_press_key[i][j] = false;
      }
    }


    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    new_press_key[0][0] =
      state[SDL_SCANCODE_UP] == SDL_PRESSED
      || state[SDL_SCANCODE_K] == SDL_PRESSED;
    new_press_key[0][1] =
      state[SDL_SCANCODE_DOWN] == SDL_PRESSED
      || state[SDL_SCANCODE_J] == SDL_PRESSED;
    new_press_key[0][2] =
      state[SDL_SCANCODE_LEFT] == SDL_PRESSED
      || state[SDL_SCANCODE_H] == SDL_PRESSED;
    new_press_key[0][3] =
      state[SDL_SCANCODE_RIGHT] == SDL_PRESSED
      || state[SDL_SCANCODE_L] == SDL_PRESSED;

    new_press_key[1][0] = state[SDL_SCANCODE_W] == SDL_PRESSED;
    new_press_key[1][1] = state[SDL_SCANCODE_S] == SDL_PRESSED;
    new_press_key[1][2] = state[SDL_SCANCODE_A] == SDL_PRESSED;
    new_press_key[1][3] = state[SDL_SCANCODE_D] == SDL_PRESSED;

    new_press_key[0][4] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
    new_press_key[1][4] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;

    for (int i = 0; i < num_devices_; ++i) {
      for (int j = 0; j < 5; ++j) {
        edge_key_[i][j] = !press_key_[i][j] && new_press_key[i][j];
        press_key_[i][j] = new_press_key[i][j];
      }
    }
  }

   bool edge_key_p(int player_type, int button) const  {
    return edge_key_[player_type][button];
  }

   bool press_key_p(int player_type, int button) const  {
    return press_key_[player_type][button];
  }

  ~InputManager()  {
    
  }
};
