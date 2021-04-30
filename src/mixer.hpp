#pragma once

#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <memory>

using namespace std;


class MixerManager {
    Mix_Music *music_;
    Mix_Chunk *sound_list_[5];

   void load_music(const char *path)  {
      music_ = Mix_LoadMUS(path);
      if (!music_) {
        cerr << "error: " << Mix_GetError() << '\n';
        exit(EXIT_FAILURE);
      }
   }

   void load_sound(const char *path, const unsigned char sound_type)  {
    sound_list_[sound_type] = Mix_LoadWAV(path);
    if (!sound_list_[sound_type]) {
      cerr << "error: " << Mix_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
  }

  public:
    MixerManager()  {
      if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) !=0) {
          cerr << Mix_GetError() << '\n';
          exit(EXIT_FAILURE);
      }

    load_music("./data/luminousparticle.wav");
    load_sound("./data/66376e_Pacman_Siren_Sound_Effect.wav", 0);
    load_sound("./data/pacman_beginning.wav", 1);
    load_sound("./data/pacman_death.wav", 2);
    load_sound("./data/pacman_chomp.wav", 3);
    load_sound("./data/gun-gird1.wav", 4);
  }

   Mix_Music *get_music() const  { return music_; }

   Mix_Chunk *get_sound(const unsigned char sound_type) const {
    return sound_list_[sound_type];
  }

  ~MixerManager()  {
    Mix_HaltMusic();
    Mix_HaltChannel(-1);
    Mix_FreeMusic(music_);
    for (const auto &se : sound_list_){
      Mix_FreeChunk(se);
    }
    Mix_CloseAudio();
    atexit(Mix_Quit);
  }
};
