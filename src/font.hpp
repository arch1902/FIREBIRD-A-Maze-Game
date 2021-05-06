#pragma once
#include <SDL2/SDL_ttf.h>
#include <iostream>

using namespace std;

class FontManager {
    TTF_Font *fonts_[2];

    void load(const char *path,unsigned int size,const unsigned char font_size) {
        fonts_[font_size] = TTF_OpenFont(path, size);
        if (!fonts_[font_size]) {
        cerr << "error: "<< TTF_GetError() <<endl;
        exit(EXIT_FAILURE);
        }
    }

    public:
        FontManager() {
            if (TTF_Init() == -1) {
                cerr << "error: " <<TTF_GetError()<<endl;
                exit(EXIT_FAILURE);
            }
            load("./data/GenEiGothicP-Heavy.otf", 36, 0);
            load("./data/GenEiGothicP-Regular.otf", 16, 1);
        }

        TTF_Font *get(const unsigned char font_size) const  {
            return fonts_[font_size];
        }

        ~FontManager() {
            for (const auto &font : fonts_) TTF_CloseFont(font);
            atexit(TTF_Quit);
        }
};
