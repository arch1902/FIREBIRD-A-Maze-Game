#pragma once

#include "global.hpp"
#include "image.hpp"
#include "input.hpp"
#include "maze.hpp"

struct player2{
    bool isready;
    int curframe;
    int type_;
    const ImageManager *image_manager_;
    const InputManager *input_manager_;
    Point pos_;
    Point block_;
    Point next_block_;
    unsigned char dir_;           // 0: down, 1: left, 2: up, 3: right
    unsigned char anime_count_;   // 0 or 1
    unsigned char anime_weight_;  // max value is 4
    int life_;
    unsigned int score_;
    bool damaged_p_;
    unsigned int power_mode_;  // 0: not power mode, not 0: power mode

    player2(const unsigned char player_type, const ImageManager *image_manager,const InputManager *input_manager); 
      

   void init_pos()  ;

   void draw(const game_mode mode);

   void move(const Maze &maze, const game_mode mode,int game_level);

   Point get_pos() const  { return pos_; }

   void set_pos(const Point &p)  { pos_ = p; }
   void set_pos(const Point &&p)  { pos_ = p; }

   Point get_block() const  { return block_; }
   Point get_nextblock() const  { return next_block_; }
   int get_dir(){ return dir_;}
   int get_anc(){ return anime_count_;}
   int get_anw(){ return anime_weight_;}

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

  void setId(int i){
    isready=true;
    type_=i;
  }
  int getId(){
    return type_;
  }
  bool isReady(){
    return isready;
  }
  int getCurframe();
};