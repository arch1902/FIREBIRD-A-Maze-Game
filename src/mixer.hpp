
#pragma once

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <memory>


class MixerManager {
  Mix_Music *music_;
  Mix_Chunk *se_list_[5];

   void load_music(const char *path)  {
    music_ = Mix_LoadMUS(path);
    if (!music_) {
      std::cerr << "error: " << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
  }

   void load_se(const char *path, const unsigned char se_type)  {
    se_list_[se_type] = Mix_LoadWAV(path);
    if (!se_list_[se_type]) {
      std::cerr << "error: " << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
  }

 public:
  MixerManager()  {
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) !=
        0) {
      std::cerr << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    load_music("./data/luminousparticle.wav");
    load_se("./data/66376e_Pacman_Siren_Sound_Effect.wav", 0);
    load_se("./data/pacman_beginning.wav", 1);
    load_se("./data/pacman_death.wav", 2);
    load_se("./data/pacman_chomp.wav", 3);
    load_se("./data/gun-gird1.wav", 4);
  }

   Mix_Music *get_music() const  { return music_; }

   Mix_Chunk *get_se(const unsigned char se_type) const  {
    return se_list_[se_type];
  }

  ~MixerManager()  {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music_);
    for (const auto &se : se_list_) {
      Mix_FreeChunk(se);
    }
    Mix_CloseAudio();
    atexit(Mix_Quit);
  }
};
