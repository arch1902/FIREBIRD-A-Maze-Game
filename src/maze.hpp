#pragma once

#include "global.hpp"
#include "image.hpp"

enum class maze_state {
  block, // #
  food, // .
  counter_food, // C
  enemy_house, // E
  init_p1_pos, // P
  init_p2_pos,
  left_warp_pos, // L
  right_warp_pos, // R
  warp_street, 
};

class Maze {
    SDL_Renderer *renderer_;
    const ImageManager *image_manager_;
    maze_state block_[24][24]; // Array of array containing element of type maze_state
    int home_distance_[24][24];

    public:
    Maze(SDL_Renderer *renderer, const ImageManager *image_manager) noexcept
    {
        renderer_ = renderer;
        image_manager_ = image_manager_;
    }

    void init(const game_mode mode) noexcept;

    void draw(int game_level) const noexcept;

    //accepts only l-value
    maze_state check_state(const Point &p) const noexcept {
        return block_[p.y][p.x];
    }
    int get_home_distance(const Point &p) const noexcept {
        return home_distance_[p.y][p.x];
    }

    ~Maze() noexcept {}
};
