#pragma once

#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>


static map<string,int> font_size = {
    {"x36",0},
    {"x16",1}
};

class FontManager {
  TTF_Font *fonts_[2];

    void load(const char *path, int size, char font_size)  {
        fonts_[font_size] = TTF_OpenFont(path, size);
        if (!fonts_[font_size]) {
            std::cerr << "error: " << TTF_GetError() << '\n';
            exit(EXIT_FAILURE);
        }
    }
    public:
        FontManager()  {
        if (TTF_Init() == -1) {
            std::cerr << "error: " << TTF_GetError() << '\n';
            exit(EXIT_FAILURE);
        }
        load("./data/GenEiGothicP-Heavy.otf", 36, font_size["x36"]);
        load("./data/GenEiGothicP-Regular.otf", 16, font_size["x16"]);
        }

        TTF_Font *get(char font_size) const  {
            return fonts_[font_size];
        }
    ~FontManager()  {
        for (const auto &font : fonts_) {
            TTF_CloseFont(font);
        }
        atexit(TTF_Quit);
    }
};
