#pragma once

#include "global.hpp"
#include "image.hpp"
#include "input.hpp"
#include "maze.hpp"
#include<string>
using namespace std;

static map<string,char> player_type{
    {"p1",'0'},
    {"p2",'1'}
};

class Player {
    char type_;
    const ImageManager *image_manager_;
    const InputManager *input_manager_;
    Point pos_;
    Point block_;
    Point next_block_;
    char dir_;           // 0: down, 1: left, 2: up, 3: right
    char anime_count_;   // 0 or 1
    char anime_weight_;  // max value is 4
    int life_;
    int score_;
    bool damaged_p_;
    int power_mode_;  // 0: not power mode, not 0: power mode

    public:
    Player(char player_type, const ImageManager *image_manager,
            const InputManager *input_manager) noexcept
        : type_(player_type),
            image_manager_(image_manager),
            input_manager_(input_manager) {}

    void init_pos() noexcept {
        switch (type_) {
        case '0': {
            pos_ = {block["size"] * 9, block["size"] * 18};
            block_ = {9, 18};
            next_block_ = {9, 18};
            dir_ = 1;
            anime_count_ = 0;
            anime_weight_ = 0;
            return;
        }
        case '1': {
            pos_ = {block["size"] * 14, block["size"] * 18};
            block_ = {14, 18};
            next_block_ = {14, 18};
            dir_ = 3;
            anime_count_ = 0;
            anime_weight_ = 0;
            return;
        }
        }
    }

    void draw(const game_mode mode) const noexcept {
        switch (type_) {
        case '0': {
            SDL_Texture *p_texture = image_manager_->get(images["p1"]);
            const SDL_Rect src = {(block["size"] * dir_),
                                (block["size"] * anime_count_),
                                block["size"],
                                block["size"]};
            const SDL_Rect dst = {pos_.x,
                                pos_.y,
                                block["size"],
                                block["size"]};
            image_manager_->render_copy(*p_texture, src, dst);
            SDL_DestroyTexture(p_texture);
            return;
        }
        case '1': {
            if (mode != game_mode::multiplayer) {
            return;
            }
            SDL_Texture *p_texture = image_manager_->get(images["p2"]);
            const SDL_Rect src = {(block["size"] * dir_),
                                (block["size"] * anime_count_),
                                block["size"],
                                block["size"]};
            const SDL_Rect dst = {pos_.x,
                                pos_.y,
                                block["size"],
                                block["size"]};
            image_manager_->render_copy(*p_texture, src, dst);
            SDL_DestroyTexture(p_texture);
            return;
        }
        }
    }

    void move(const Maze &maze, const game_mode mode) noexcept {
        if (type_ == player_type["p2"] && mode != game_mode::multiplayer ) {
        return;
        }

        const Point dst_pos = {next_block_.x * block["size"],
                            next_block_.y * block["size"]};
        if (pos_.x != dst_pos.x || pos_.y != dst_pos.y) {
        ++anime_weight_;
        if (anime_weight_ > 4) {
            anime_count_ = 1 - anime_count_;
            anime_weight_ = 0;
        }
        const unsigned int move_value = 2;
        if (dst_pos.x > pos_.x) {
            pos_.x += move_value;
        }
        if (dst_pos.y > pos_.y) {
            pos_.y += move_value;
        }
        if (dst_pos.x < pos_.x) {
            pos_.x -= move_value;
        }
        if (dst_pos.y < pos_.y) {
            pos_.y -= move_value;
        }
        return;
        }

        block_ = next_block_;

        // The button precedence is Down > Left > Up > Right.
        Point mut_dst_block = next_block_;
        if (input_manager_->press_key_p(type_, input["down"])) {
        dir_ = 0;
        ++mut_dst_block.y;
        } else if (input_manager_->press_key_p(type_, input["left"])) {
        dir_ = 1;
        --mut_dst_block.x;
        } else if (input_manager_->press_key_p(type_, input["up"])) {
        dir_ = 2;
        --mut_dst_block.y;
        } else if (input_manager_->press_key_p(type_, input["right"])) {
        dir_ = 3;
        ++mut_dst_block.x;
        }
        const Point dst_block = mut_dst_block;
        Point temp;

        const maze_state dst_block_state = maze.check_state(dst_block);
        temp = {dst_block.x + 1, dst_block.y};
        const maze_state dst_right_block_state =
            maze.check_state(temp);
        temp = {dst_block.x - 1, dst_block.y};
        const maze_state dst_left_block_state =
            maze.check_state(temp);
        // TODO: make private function
        if (dst_block_state == maze_state::food
            || dst_block_state == maze_state::init_p1_pos
            || dst_block_state == maze_state::init_p2_pos
            || dst_block_state == maze_state::counter_food
            || dst_block_state == maze_state::warp_street
            || dst_block_state == maze_state::left_warp_pos
            || dst_right_block_state == maze_state::left_warp_pos
            || dst_block_state == maze_state::right_warp_pos
            || dst_left_block_state == maze_state::right_warp_pos) {
        next_block_ = dst_block;
        }

        // Circle corner
        temp = {dst_block.x + 2, dst_block.y};
        if (maze.check_state(temp)
            == maze_state::left_warp_pos) {
        next_block_.x = block["count_x"];
        pos_.x = block["size"] * next_block_.x;
        }
        temp = {dst_block.x - 2, dst_block.y};
        if (maze.check_state(temp)
            == maze_state::right_warp_pos) {
        next_block_.x = -1;
        pos_.x = block["size"] * next_block_.x;
        }
    }

    Point get_pos() const noexcept { return pos_; }

    void set_pos(const Point &p) noexcept { pos_ = p; }
    void set_pos(const Point &&p) noexcept { pos_ = p; }

    Point get_block() const noexcept { return block_; }

    int get_life() const noexcept { return life_; }

    void set_life(const int life) noexcept { life_ = life; }

    unsigned int get_score() const noexcept { return score_; }

    void set_score(const unsigned int score) noexcept { score_ = score; }

    bool get_damaged() const noexcept { return damaged_p_; }

    void set_damaged(const bool damaged_p) noexcept {
        damaged_p_ = damaged_p;
    }

    unsigned int get_power_mode() const noexcept { return power_mode_; }

    void set_power_mode(const unsigned int power_mode) noexcept {
        power_mode_ = power_mode;
    }

    ~Player() noexcept {}
};