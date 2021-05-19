#pragma once

#include "global.hpp"
#include "image.hpp"
#include "input.hpp"
#include "maze.hpp"
#include "bullet.hpp"

class Player {
  public:
  const unsigned char type_;
  const ImageManager *image_manager_;
  const InputManager *input_manager_;
  Point pos_;
  Point block_;
  Point next_block_;
  int dir_;           // 0: down, 1: left, 2: up, 3: right
  int anime_count_;   // 0 or 1
  unsigned char anime_weight_;  // max value is 4
  int life_;
  unsigned int score_;
  bool damaged_p_;
  unsigned int power_mode_;  // 0: not power mode, not 0: power mode

  Player(const unsigned char player_type, const ImageManager *image_manager,const InputManager *input_manager) 
      : type_(player_type),
        image_manager_(image_manager),
        input_manager_(input_manager){}
  
   void init_pos()  {
    switch (type_) {
      case 0: {
        pos_ = {20 * 9, 20 * 18};
        block_ = {9, 18};
        next_block_ = {9, 18};
        dir_ = 1;
        anime_count_ = 0;
        anime_weight_ = 0;
        return;
      }
      case 1: {
        pos_ = {20 * 14, 20 * 18};
        block_ = {14, 18};
        next_block_ = {14, 18};
        dir_ = 3;
        anime_count_ = 0;
        anime_weight_ = 0;
        return;
      }
    }
  }

   void draw(const game_mode mode) const  {
    switch (type_) {
      case 0: {
        SDL_Texture *p_texture = image_manager_->get(0);
        const SDL_Rect src = {(20 * dir_),(20 * anime_count_),20,20};
        const SDL_Rect dst = {(pos_.x), (pos_.y),20,20};
        image_manager_->render_copy(*p_texture, src, dst);
        SDL_DestroyTexture(p_texture);
        return;
      }
      case 1: {
        if (mode != game_mode::multiplayer) {
          return;
        }
        SDL_Texture *p_texture = image_manager_->get(1);
        const SDL_Rect src = {(20 * dir_),(20 * anime_count_),20,20};
        const SDL_Rect dst = {(pos_.x),(pos_.y),20, 20};
        image_manager_->render_copy(*p_texture, src, dst);
        SDL_DestroyTexture(p_texture);
        return;
      }
    }
  }

   void move(const Maze &maze, const game_mode mode,int game_level)  {
    if (type_ == 1 && mode != game_mode::multiplayer) {
      return;
    }

    const Point dst_pos = {next_block_.x * 20,next_block_.y * 20};
    if (pos_.x != dst_pos.x || pos_.y != dst_pos.y) {
      anime_weight_++;
      if (anime_weight_ > 4) {
        anime_count_ = 1 - anime_count_;
        anime_weight_ = 0;
      }
      int move_value;
      if(game_level==1) move_value=2;
      else if (game_level==2) move_value=4;
      else if (game_level==3) move_value=5;
      else move_value=10;
      
      if (dst_pos.x > pos_.x) pos_.x += move_value;
      else if (dst_pos.x < pos_.x) pos_.x -= move_value;
      
      if (dst_pos.y > pos_.y) pos_.y += move_value; 
      else if (dst_pos.y < pos_.y) pos_.y -= move_value;

      return;
    }

    block_ = next_block_;

    // The button precedence is Down > Left > Up > Right.
    Point mut_dst_block = next_block_;
    if (input_manager_->press_key_p(type_, 1)) {
      dir_ = 0;
      mut_dst_block.y++;
    } else if (input_manager_->press_key_p(type_, 2)) {
      dir_ = 1;
      mut_dst_block.x--;
    } else if (input_manager_->press_key_p(type_, 0)) {
      dir_ = 2;
      mut_dst_block.y--;
    } else if (input_manager_->press_key_p(type_, 3)) {
      dir_ = 3;
      mut_dst_block.x++;
    }
    const Point dst_block = mut_dst_block;

    const maze_state dst_block_state = maze.check_state(dst_block);
    const maze_state dst_right_block_state =
        maze.check_state(Point{dst_block.x + 1, dst_block.y});
    const maze_state dst_left_block_state =
        maze.check_state(Point{dst_block.x - 1, dst_block.y});

    if (dst_block_state == maze_state::food
        || dst_block_state == maze_state::init_p1_pos
        || dst_block_state == maze_state::init_p2_pos
        || dst_block_state == maze_state::counter_food
        || dst_block_state == maze_state::warp_street
        || dst_block_state == maze_state::left_warp_pos
        || dst_right_block_state == maze_state::left_warp_pos
        || dst_block_state == maze_state::right_warp_pos
        || dst_left_block_state == maze_state::right_warp_pos) {
          if(!(dst_block == Point {11,23}||dst_block == Point {13,0})){
            next_block_ = dst_block;
          }
    }

    // Circle corner
    if (maze.check_state(Point{dst_block.x + 2, dst_block.y}) == maze_state::left_warp_pos && dir_ == 1) {
      next_block_.x = 24;
      pos_.x = 20 * next_block_.x;
    }
    if (maze.check_state(Point{dst_block.x - 2, dst_block.y}) == maze_state::right_warp_pos && dir_ == 3) {
      next_block_.x = -1;
      pos_.x = 20 * next_block_.x;
    }
  }

   Point get_pos() const  { return pos_; }

   void set_pos(const Point &p)  { pos_ = p; }
   void set_pos(const Point &&p)  { pos_ = p; }

   int get_dir() const { return dir_;}
   void set_dir(int n) {dir_ = n;} 

   int get_count() const { return anime_count_;}
   void set_count(int n) {anime_count_ = n;} 
  
   Point get_block() const  { return block_; }

   int get_life() const  { return life_; }

   void set_life(const int life)  { life_ = life; }

   unsigned int get_score() const  { return score_; }

   void set_score(const unsigned int score)  { score_ = score; }

   bool get_damaged() const  { return damaged_p_; }

   void set_damaged(const bool damaged_p)  {
    damaged_p_ = damaged_p;
  }

   unsigned int get_power_mode() const  { return power_mode_; }

   void set_power_mode(const unsigned int power_mode)  {
    power_mode_ = power_mode;
  }

  ~Player()  {}
};
