#pragma once

#include <SDL2/SDL_mixer.h>
#include <memory>
#include <vector>
#include "global.hpp"
#include "image.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"
#include "client.hpp"
#include "server.hpp"
#include "sstream"


class Bullet {
    public :
    int dir_;
    bool present_;
    Point pos_;
    const ImageManager *image_manager_;

    Bullet(const ImageManager *image_manager) 
      : image_manager_(image_manager),present_(false){}

    void draw() {
        SDL_Texture *bullet_texture;
        bullet_texture = image_manager_->get(14);
        const SDL_Rect dst = {(pos_.x),(pos_.y),20,20};
        const SDL_Rect src = {0,0,20,20};
        image_manager_->render_copy(*bullet_texture, src, dst);
        SDL_DestroyTexture(bullet_texture);
    }
    void move(const Maze &maze) {
        int a = (pos_.x/5)*5;
        int b = (pos_.y/5)*5;
        if (dir_ == 0){
            pos_.y = b + 5;
        }else if (dir_ == 1){
            pos_.x = a - 5;
        }else if (dir_ == 2){
            pos_.y = b - 5;
        }else{
            pos_.x = a + 5;
        }
        if (pos_.x%20 == 0 && pos_.y%20 == 0){
            Point dst_block = {pos_.x/20,pos_.y/20};
            const maze_state dst_block_state = maze.check_state(dst_block);
            if (dst_block_state == maze_state::block){
                present_ = false;
            }
        }
    }
};