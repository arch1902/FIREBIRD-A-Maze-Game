#pragma once

#include "global.hpp"

class Wipe {
  enum class wipe_type { in, out };

  wipe_type wipe_variation_;
  unsigned int wipe_count_;
  unsigned int wipe_count_reach_;
  SDL_Renderer *renderer_;

 public:
  Wipe(SDL_Renderer *renderer) noexcept : renderer_(renderer) {}

    void set_wipe_in() noexcept {
    wipe_variation_ = wipe_type::in;
    wipe_count_ = screen_specifications["height"] / 10;
    wipe_count_reach_ = 0;
  }
    void set_wipe_out() noexcept {
    wipe_variation_ = wipe_type::out;
    wipe_count_ = 0;
    wipe_count_reach_ = screen_specifications["height"] / 10;
  }
    void draw(const unsigned int limit_x) const noexcept {
    SDL_Rect dst = {0, 0,static_cast<int>(limit_x),static_cast<int>(wipe_count_)};
    const unsigned int dy = screen_specifications["height"] / 10;
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    for (dst.y = 0; dst.y < screen_specifications["height"]; dst.y += dy) {
      SDL_RenderFillRect(renderer_, &dst);
    }
  }
    bool update() noexcept {
    if (wipe_count_ == wipe_count_reach_) {
      return true;
    }

    switch (wipe_variation_) {
      case wipe_type::in:
        --wipe_count_;
        break;
      case wipe_type::out:
        ++wipe_count_;
        break;
    }
    return false;
  }

  ~Wipe() noexcept {}
};
