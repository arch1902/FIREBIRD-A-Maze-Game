#pragma once

#include <SDL2/SDL_mixer.h>
#include "global.hpp"
#include "enemy.hpp"
#include "image.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"
extern bool music;

class Food {
  enum class food_state {
    nothing,
    food,
    counter_food,
  };

  food_state food_[24][24];
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;

 public:
  Food(const ImageManager *image_manager, const MixerManager *mixer_manager) 
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {}

   void init(const Maze &maze)  {
    for (int y = 0; y < 24; ++y) {
      for (int x = 0; x < 24; ++x) {
        switch (maze.check_state(Point{x, y})) {
          case maze_state::food:
            food_[y][x] = food_state::food;
            break;
          case maze_state::counter_food:
            food_[y][x] = food_state::counter_food;
            break;
          default:
            food_[y][x] = food_state::nothing;
            break;
        }
      }
    }
  }

   void draw()  {
    const SDL_Rect src = {0, 0, 20, 20};
    SDL_Texture *food_texture = image_manager_->get(6);
    SDL_Texture *food_counter_texture = image_manager_->get(7);
    for (int y = 0; y < 24; ++y) {
      for (int x = 0; x < 24; ++x) {
        SDL_Rect dst;
        dst.x = (20 * x);
        dst.y = (20 * y);
        switch (food_[y][x]) {
          case food_state::food: {
            SDL_QueryTexture(food_texture, nullptr, nullptr, &dst.w, &dst.h);
            image_manager_->render_copy(*food_texture, src, dst);
            break;
          }
          case food_state::counter_food: {
            SDL_QueryTexture(food_counter_texture, nullptr, nullptr, &dst.w, &dst.h);
            image_manager_->render_copy(*food_counter_texture, src, dst);
            break;
          }
          default:
            break;
        }
      }
    }
    SDL_DestroyTexture(food_texture);
    SDL_DestroyTexture(food_counter_texture);
  }

   bool check_state(const game_mode mode, Player &p1, Player &p2)  {
    const Point posi = p1.get_pos();
    Point block = {posi.x/20,posi.y/20};
    switch (food_[block.y][block.x]) {
      case food_state::food: {
        if (music){
        Mix_PlayChannel(3, mixer_manager_->get_sound(3),0);
        }
        food_[block.y][block.x] = food_state::nothing;
        p1.set_score(p1.get_score() + 10);
        break;
      }
      case food_state::counter_food: {
        p1.set_power_mode(400);
        food_[block.y][block.x] = food_state::nothing;
        break;
      }
      default:
        break;
    }

    if (mode == game_mode::multiplayer) {
      const Point posi = p2.get_pos();
      Point block = {posi.x/20,posi.y/20};
      switch (food_[block.y][block.x]) {
        case food_state::food: {
          if(music){
            Mix_PlayChannel(3, mixer_manager_->get_sound(3), 0);
          }
          food_[block.y][block.x] = food_state::nothing;
          p2.set_score(p2.get_score() + 10);
          break;
        }
        case food_state::counter_food: {
          p2.set_power_mode(400);
          food_[block.y][block.x] = food_state::nothing;
          break;
        }
        default:
          break;
      }
    }

    if ((p1.get_power_mode() != 0 && p1.get_power_mode() % 80 == 0)
        || (p2.get_power_mode() != 0 && p2.get_power_mode() % 80 == 0)) {
          if (music){
            Mix_PlayChannel(0, mixer_manager_->get_sound(0),0);
          }
    }

    int rest_food = 0;
    for (int y = 0; y < 24; ++y) {
      for (int x = 0; x < 24; ++x) {
        if (food_[y][x] == food_state::food|| food_[y][x] == food_state::counter_food) {
          rest_food++;
        }
      }
    }

    return rest_food == 0;
  }

  ~Food()  {}
};
