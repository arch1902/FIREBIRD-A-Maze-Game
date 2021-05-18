#pragma once

#include <vector>
#include <iostream>
#include "global.hpp"


class InputManager {

  bool edge_key_[2][8];
  bool press_key_[2][8];

 public:
  InputManager()  {}

   void update()  {
    int num_devices_ = 2;
    bool new_press_key[num_devices_][8];
    for (int i = 0; i < num_devices_; ++i) {
      for (int j = 0; j < 8; ++j) {
        new_press_key[i][j] = false;
      }
    }


    const Uint8 *state = SDL_GetKeyboardState(nullptr);

    new_press_key[0][0] =
      state[SDL_SCANCODE_UP] == SDL_PRESSED;
    new_press_key[0][1] =
      state[SDL_SCANCODE_DOWN] == SDL_PRESSED;
    new_press_key[0][2] =
      state[SDL_SCANCODE_LEFT] == SDL_PRESSED;
    new_press_key[0][3] =
      state[SDL_SCANCODE_RIGHT] == SDL_PRESSED;

    new_press_key[1][0] = state[SDL_SCANCODE_UP] == SDL_PRESSED;
    new_press_key[1][1] = state[SDL_SCANCODE_DOWN] == SDL_PRESSED;
    new_press_key[1][2] = state[SDL_SCANCODE_LEFT] == SDL_PRESSED;
    new_press_key[1][3] = state[SDL_SCANCODE_RIGHT] == SDL_PRESSED;

    new_press_key[0][6] = state[SDL_SCANCODE_I] == SDL_PRESSED;
    new_press_key[1][6] = state[SDL_SCANCODE_I] == SDL_PRESSED;
    new_press_key[0][7] = state[SDL_SCANCODE_M] == SDL_PRESSED;
    new_press_key[1][7] = state[SDL_SCANCODE_M] == SDL_PRESSED;

    new_press_key[0][5] = state[SDL_SCANCODE_LSHIFT] == SDL_PRESSED;
    new_press_key[1][5] = state[SDL_SCANCODE_LSHIFT] == SDL_PRESSED;
    new_press_key[0][4] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
    new_press_key[1][4] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;

    for (int i = 0; i < num_devices_; ++i) {
      for (int j = 0; j < 8; ++j) {
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
