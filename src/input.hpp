// This file manages the input from the user.


#pragma once

#include <vector>
#include <iostream>
#include<string>
#include "global.hpp"


map<String,int> input {
	"up":0,
	"down":1,
	"left":2,
	"right":3,
	"space":4,
	"count":5,
}

class InputManager{
	bool edge_key_[2][input["count"]];
	bool press_key_[2][input["count"]];

	public:
		void InputManager() noexcept {}
		void update() noexcept{
			const Uint8 *state = SDL_GetKeyboardState(nullptr);
			bool new_press_key[2][input["count"]];
			new_press_key[0][input["up"]] = state[SDL_SCANCODE_UP] == SDL_PRESSED;
			new_press_key[0][input["down"]] = state[SDL_SCANCODE_DOWN] == SDL_PRESSED;
			new_press_key[0][input["left"]] = state[SDL_SCANCODE_LEFT] == SDL_PRESSED;
			new_press_key[0][input["right"]] = state[SDL_SCANCODE_RIGHT] == SDL_PRESSED;   

			new_press_key[1][input["up"]] = state[SDL_SCANCODE_W] == SDL_PRESSED;
			new_press_key[1][input["down"]] = state[SDL_SCANCODE_S] == SDL_PRESSED;
			new_press_key[1][input["left"]] = state[SDL_SCANCODE_A] == SDL_PRESSED;
			new_press_key[1][input["right"]] = state[SDL_SCANCODE_D] == SDL_PRESSED;

			new_press_key[0][input["space"]] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;
			new_press_key[1][input["space"]] = state[SDL_SCANCODE_SPACE] == SDL_PRESSED;

			for (int i = 0; i < 2; ++i) {
				for (int j = 0; j < input["count"]; ++j) {
					edge_key_[i][j] = !press_key_[i][j] && new_press_key[i][j];
					press_key_[i][j] = new_press_key[i][j];
				}
			}
		}

		bool edge_key_p(char player_type, char button) const noexcept {
			return edge_key_[player_type][button];
		}

		bool press_key_p(char player_type,char button) const noexcept {
			return press_key_[player_type][button];
		}

	~InputManager() noexcept{}
}