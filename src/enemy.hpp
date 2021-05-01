#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>
#include <vector>

#include "global.hpp"
#include "image.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"

using namespace std;

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

  vector<Enemy_data> enemies_;
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;

   void update()  {
    for (auto &enemy : enemies_) {
      enemy.anime_weight++;
      if (enemy.anime_weight >= 8) {
        enemy.anime_weight = 0;
        enemy.anime_count++;
        enemy.anime_count %= 2;
      }
    }
  }

  void move_normal_enemy(Enemy_data &enemy, const Maze &maze,int game_level, const Player &p1, const Player &p2) ;

  void move_lose_enemy(Enemy_data &enemy, const Maze &maze, const Player &p1, const Player &p2) ;

 public:
  Enemy(const ImageManager *image_manager,const MixerManager *mixer_manager) 
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {
    enemies_.reserve(4);
    for (unsigned char i = 0; i < 4; ++i) {
      enemies_.push_back(Enemy_data(i));
    }
  }

   void init()  {
    const Point start_block[4] = {{11, 12}, {12, 12}, {11, 11}, {12, 11}};
    for (auto &enemy : enemies_) {
      enemy.pos = {20 * start_block[enemy.type].x, 20 * start_block[enemy.type].y};
      enemy.block = start_block[enemy.type];
      enemy.next_block = start_block[enemy.type];
      enemy.dir = 2;
      enemy.anime_count = 0;
      enemy.anime_weight = 0;
      enemy.state = enemy_state::normal;
    }
  }

   void draw() const  {
    SDL_Texture *enemies_texture[4];
    enemies_texture[0] = image_manager_->get(8);
    enemies_texture[1] = image_manager_->get(9);
    enemies_texture[2] = image_manager_->get(10);
    enemies_texture[3] = image_manager_->get(11);
    SDL_Texture *mon_run_texture = image_manager_->get(12);
    for (const auto &enemy : enemies_) {
      const SDL_Rect dst = {(enemy.pos.x),(enemy.pos.y),20,20};
      switch (enemy.state) {
        case enemy_state::normal: {
          const SDL_Rect src = {(20 * enemy.dir),(20 * enemy.anime_count),20,20};
          image_manager_->render_copy(*enemies_texture[enemy.type], src, dst);
          break;
        }
        case enemy_state::lose: {
          const SDL_Rect src = {0,(20 * enemy.anime_count),20,20};
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

   void move( const Maze &maze,int game_level, const Player &p1, const Player &p2)  {
    for (auto &enemy : enemies_) {
      if ( enemy.state == enemy_state::lose) {
        move_lose_enemy(enemy, maze, p1, p2);
      } else {
        move_normal_enemy(enemy, maze,game_level, p1, p2);
      }
    }
  }
  bool check_hit_enemy(const game_mode mode, Player &p1, Player &p2) ;

  ~Enemy()  {}
};
