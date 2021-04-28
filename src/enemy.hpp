#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>
#include <vector>
#include<string>
#include "global.hpp"
#include "image.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"

static map<string,int> enemy_character = {
    {"mon1",0},
    {"mon2",1},
    {"mon3",2},
    {"mon4",3}
};

class Enemy {
  enum class enemy_state {
    normal,
    lose,
  };

  struct Enemy_data {
    const unsigned char type;
    Point pos;
    Point block;
    Point next_block;
    unsigned char dir;           // max value is 4
    unsigned char anime_count;   // 0 or 1
    unsigned char anime_weight;  // max value is 8
    enemy_state state;

    Enemy_data(const unsigned char enemy_type) : type(enemy_type) {}
  };

  std::vector<Enemy_data> enemies_;
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;

  void update() noexcept {
    for (auto &enemy : enemies_) {
      ++enemy.anime_weight;
      if (enemy.anime_weight >= 8) {
        enemy.anime_weight = 0;
        ++enemy.anime_count;
        enemy.anime_count %= 2;
      }
    }
  }

  void move_normal_enemy(Enemy_data &enemy, const Maze &maze, const Player &p1,
                         const Player &p2) noexcept;

  void move_lose_enemy(Enemy_data &enemy, const Maze &maze, const Player &p1,
                       const Player &p2) noexcept;

 public:
  Enemy(const ImageManager *image_manager,
        const MixerManager *mixer_manager) noexcept
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {
    enemies_.reserve(4);
    for (unsigned char i = 0; i < 4; ++i) {
      enemies_.push_back(Enemy_data(i));
    }
  }

  void init() noexcept {
    const Point start_block[4] = {
        {11, 12}, {12, 12}, {11, 11}, {12, 11}};
    for (auto &enemy : enemies_) {
      enemy.pos = {block["size"] * start_block[enemy.type].x,
                   block["size"] * start_block[enemy.type].y};
      enemy.block = start_block[enemy.type];
      enemy.next_block = start_block[enemy.type];
      enemy.dir = 2;
      enemy.anime_count = 0;
      enemy.anime_weight = 0;
      enemy.state = enemy_state::normal;
    }
  }

  void draw() const noexcept {
    SDL_Texture *enemies_texture[enemy_character.size()];
    enemies_texture[enemy_character["mon1"]] = image_manager_->get(images["mon1"]);
    enemies_texture[enemy_character["mon2"]] = image_manager_->get(images["mon2"]);
    enemies_texture[enemy_character["mon3"]] = image_manager_->get(images["mon3"]);
    enemies_texture[enemy_character["mon4"]] = image_manager_->get(images["mon4"]);
    SDL_Texture *mon_run_texture = image_manager_->get(images["mon_run"]);
    for (const auto &enemy : enemies_) {
      const SDL_Rect dst = {(enemy.pos.x),
                            (enemy.pos.y),
                            block["size"],
                            block["size"]};
      switch (enemy.state) {
        case enemy_state::normal: {
          const SDL_Rect src = {(block["size"] * enemy.dir),
                                 (block["size"] * enemy.anime_count),
                                block["size"],
                                block["size"]};
          image_manager_->render_copy(*enemies_texture[enemy.type], src, dst);
          break;
        }
        case enemy_state::lose: {
          const SDL_Rect src = {0,(block["size"] * enemy.anime_count),
                                block["size"],block["size"]};
          image_manager_->render_copy(*mon_run_texture, src, dst);
          break;
          }
        }
      }
    

      for (auto &t : enemies_texture) {
        SDL_DestroyTexture(t);
      }
      SDL_DestroyTexture(mon_run_texture);
    
  }

  void move(const bool debug_lose_enemy, const Maze &maze,
                   const Player &p1, const Player &p2) noexcept {
    for (auto &enemy : enemies_) {
      if (debug_lose_enemy || enemy.state == enemy_state::lose) {
        move_lose_enemy(enemy, maze, p1, p2);
      } else {
        move_normal_enemy(enemy, maze, p1, p2);
      }
    }
  }

  /**
   * Return true if the player whose state is normal hits enemy, and false
   * otherwise.
   */
  bool check_hit_enemy(const game_mode mode, Player &p1, Player &p2) noexcept;

  ~Enemy() noexcept {}
};


