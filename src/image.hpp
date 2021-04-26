#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>

map<string,int> images = {
    {"player1",0},
    {"player2",1},
    {"bg",2},
    {"bg_red",3},
    {"bg_green",4},
    {"bg_blue",5},
    {"food",6},
    {"food_counter",7},
    {"mon1",8},
    {"mon2",9},
    {"mon3",10},
    {"mon4",11},
    {"mon_run",12},
    {"plate",13},
    {"count",14}
};

class ImageManager {
    std::vector<SDL_Surface *> images_;
    SDL_Renderer *renderer_;

    void load(const char *path, char image_type) noexcept {
        SDL_Surface *image = IMG_Load(path);
        if (!image) {
            std::cerr << "error: " << IMG_GetError() << '\n';
            exit(EXIT_FAILURE);
        }
        images_[image_type] = image;
    }

    public:
        ImageManager(SDL_Renderer *renderer) noexcept : renderer_(renderer) 
        {
            images_.reserve(images["count"]);

            const int flag = IMG_INIT_PNG;
            if ((IMG_Init(flag) & flag) != flag) {
                std::cerr << "error: " << IMG_GetError() << '\n';
                exit(EXIT_FAILURE);
            }

            load("./data/player1.png", images["player1"]);
            load("./data/player2.png", images["player2"]);
            load("./data/bg.png", images["bg"]);
            load("./data/bg_red.png", images["bg_red"]);
            load("./data/bg_green.png", images["bg_green"]);
            load("./data/bg_blue.png", images["bg_blue"]);
            load("./data/food.png", images["food"]);
            load("./data/food_counter.png", images["food_counter"]);
            load("./data/akabei.png", images["mon1"]);
            load("./data/pinky.png", images["mon2"]);
            load("./data/aosuke.png", images["mon3"]);
            load("./data/guzuta.png", images["mon4"]);
            load("./data/mon_run.png", images["mon_run"]);
            load("./data/plate.png", images["plate"]);
        }
        SDL_Texture *get(char image_type) const noexcept {
            return SDL_CreateTextureFromSurface(renderer_, images_[image_type]);
        }

        void render_copy(SDL_Texture &texture, const SDL_Rect &src,const SDL_Rect &dst) const noexcept {
            SDL_RenderCopy(renderer_, &texture, &src,&dst);
        }
    ~ImageManager() noexcept { atexit(IMG_Quit); }
};
