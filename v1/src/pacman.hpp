#pragma once

#include <time.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include "enemy.hpp"
#include "font.hpp"
#include "food.hpp"
#include "input.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"
#include "wipe.hpp"

struct RGB {
  Uint8 r;
  Uint8 g;
  Uint8 b;
};

namespace rgb {
const RGB black = RGB{0x00, 0x00, 0x00};
const RGB red = RGB{0xff, 0x00, 0x00};
const RGB white = RGB{0xff, 0xff, 0xff};
}  // namespace rgb

class Pacman {
  enum class game_state {
    title,
    start,
    clear,
    miss,
    playing,
    gameover,
    pause,
  };

  const bool fullscreen_mode_;
  const bool debug_mode_;

  SDL_Window *window_;
  SDL_Renderer *renderer_;
  game_state game_state_;
  unsigned int game_level_;
  game_mode game_mode_;
  unsigned int blink_count_;
  unsigned int game_count_;
  bool debug_lose_enemy_;

  std::unique_ptr<ImageManager> image_manager_;
  std::unique_ptr<MixerManager> mixer_manager_;
  std::unique_ptr<Wipe> wipe_;
  std::unique_ptr<InputManager> input_manager_;
  std::unique_ptr<Maze> map_;
  std::unique_ptr<Food> food_;
  std::unique_ptr<Enemy> enemy_;
  std::unique_ptr<Player> p1_;
  std::unique_ptr<Player> p2_;
  FontManager font_manager_;

  void game_title() ;
  void game_start() ;
  void play_game() ;
  void game_clear() ;
  void game_miss() ;
  void game_over() ;

  void game_pause()  {
    map_->draw(game_level_);
    food_->draw();
    enemy_->draw();
    p1_->draw(game_mode_);
    p2_->draw(game_mode_);
    draw_score();
    draw_translucence();
    if (input_manager_->edge_key_p(player_type["p1"], input["space"])) {
      game_state_ = game_state::playing;
    }
  }

  void draw_text(const unsigned char font_size, const RGB &rgb,
                        const Point &p, const char *str) const  {
    const SDL_Color color = {rgb.r, rgb.g, rgb.b, 255};
    SDL_Surface *font_surface =
        TTF_RenderUTF8_Blended(font_manager_.get(font_size), str, color);
    SDL_Texture *font_texture =
        SDL_CreateTextureFromSurface(renderer_, font_surface);
    const SDL_Rect src = {0, 0, static_cast<Uint16>(font_surface->w),
                          static_cast<Uint16>(font_surface->h)};
    SDL_Rect dst;
    dst.x = static_cast<Sint16>(p.x);
    dst.y = static_cast<Sint16>(p.y);
    SDL_QueryTexture(font_texture, nullptr, nullptr, &dst.w, &dst.h);
    SDL_RenderCopy(renderer_, font_texture, &src, &dst);
    SDL_DestroyTexture(font_texture);
  }

  void draw_text(const unsigned char font_size, const RGB &&rgb,
                        const Point &p, const char *str) const  {
    draw_text(font_size, rgb, p, str);
  }

    void draw_text(const unsigned char font_size, const RGB &rgb,
                        const Point &&p, const char *str) const  {
    draw_text(font_size, rgb, p, str);
  }

  void draw_text(const unsigned char font_size, const RGB &&rgb,
                        const Point &&p, const char *str) const  {
    draw_text(font_size, rgb, p, str);
  }

  void draw_score() const  {
    // TODO: divide it into private functions
    // Draw the plate of background.
    {
      SDL_Texture *p_texture = image_manager_->get(images["plate"]);
      SDL_Rect dst;
      dst.x = screen_specifications["offset_x"];
      dst.y = 0;
      SDL_QueryTexture(p_texture, nullptr, nullptr, &dst.w, &dst.h);
      SDL_RenderCopy(renderer_, p_texture, nullptr, &dst);
      SDL_DestroyTexture(p_texture);
    }

    // Draw the score itself.
    {
      const unsigned int x1 = screen_specifications["offset_x"] + 20;
      const unsigned int y1 = screen_specifications["height"] / 7 + 10;
      const unsigned int x2 = x1 + 40;
      const unsigned int y2 = y1 + 30;
      const unsigned int x3 = x2 + 30;
      const unsigned int y3 = y2;

      std::stringstream score;
      score << "S c o r e  :  " << std::setw(6) << p1_->get_score();
      draw_text(font_size["x16"], rgb::white, Point{static_cast<int>(x1), static_cast<int>(y1)}, score.str().c_str());

      SDL_Texture *p_texture = image_manager_->get(images["p1"]);
      SDL_Rect src = {block["size"], 0, block["size"], block["size"]};
      SDL_Rect dst = {static_cast<int>(x2), static_cast<int>(y2), block["size"], block["size"]};
      SDL_RenderCopy(renderer_, p_texture, &src, &dst);
      SDL_DestroyTexture(p_texture);

      std::stringstream life;
      life << "x  " << p1_->get_life();
      draw_text(font_size["x16"], rgb::white, Point{static_cast<int>(x3), static_cast<int>(y3)}, life.str().c_str());

      if (game_mode_ == game_mode::multiplayer) {
        const unsigned int offset_y = 80;
        std::stringstream score;
        score << "S c o r e  :  " << std::setw(6) << p2_->get_score();
        draw_text(font_size["x16"], rgb::white, Point{static_cast<int>(x1), static_cast<int>(y1 + offset_y)},
                  score.str().c_str());

        SDL_Texture *p_texture = image_manager_->get(images["p2"]);
        const SDL_Rect src = {block["size"], 0, block["size"], block["size"]};
        const SDL_Rect dst = {static_cast<int>(x2), static_cast<int>(y2 + offset_y), block["size"], block["size"]};
        SDL_RenderCopy(renderer_, p_texture, &src, &dst);
        SDL_DestroyTexture(p_texture);

        std::stringstream life;
        life << "x  " << p2_->get_life();
        draw_text(font_size["x16"], rgb::white, Point{static_cast<int>(x3), static_cast<int>(y3 + offset_y)},
                  life.str().c_str());
      }
    }

    // Draw the rest time of power mode.
    {
      const unsigned int x = screen_specifications["offset_x"] + 10;
      const unsigned int y = screen_specifications["height"] / 6 * 4;
      if (p1_->get_power_mode()) {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
        const SDL_Rect dst = {
            static_cast<int>(x), static_cast<int>(y), static_cast<Uint16>(p1_->get_power_mode() / 4), block["size"]};
        SDL_RenderFillRect(renderer_, &dst);
      }
      if (p2_->get_power_mode()) {
        SDL_SetRenderDrawColor(renderer_, 128, 128, 0, 255);
        const SDL_Rect dst = {static_cast<int>(x), static_cast<int>(y + 30),
                              static_cast<Uint16>(p2_->get_power_mode() / 4),
                              block["size"]};
        SDL_RenderFillRect(renderer_, &dst);
      }
    }
  }

  bool poll_event() const  {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          return false;
        case SDL_KEYDOWN:
          if (event.key.keysym.sym == SDLK_ESCAPE) {
            return false;
          }
          break;
        default:
          // do nothing
          break;
      }
    }
    return true;
  }

  inline void wait_game() const  {
    static Uint32 pre_count;
    const double wait_time = 1000.0 / screen_specifications["max_fps"];
    const Uint32 wait_count = (wait_time + 0.5);
    if (pre_count) {
      const Uint32 now_count = SDL_GetTicks();
      const Uint32 interval = now_count - pre_count;
      if (interval < wait_count) {
        const Uint32 delay_time = wait_count - interval;
        SDL_Delay(delay_time);
      }
    }
    pre_count = SDL_GetTicks();
  }

  void draw_fps() const  {
    static Uint32 pre_count;
    const Uint32 now_count = SDL_GetTicks();
    if (pre_count) {
      static double frame_rate;
      Uint32 mut_interval = now_count - pre_count;
      if (mut_interval < 1) {
        mut_interval = 1;
      }
      const Uint32 interval = mut_interval;

      if (!(pre_count % 30)) {
        frame_rate = 1000.0 / interval;
      }

      std::stringstream ss;
      ss << "FrameRate[" << std::setprecision(2)
         << std::setiosflags(std::ios::fixed) << frame_rate << "]";
      draw_text(font_size["x16"], rgb::white, Point{screen_specifications["offset_x"] + 15, 16},
                ss.str().c_str());
    }
    pre_count = now_count;
  }

  inline void draw_translucence()  {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface *trans_surface =
        SDL_CreateRGBSurface(SDL_SWSURFACE, screen_specifications["width"], screen_specifications["height"], 32,
                             rmask, gmask, bmask, amask);
    if (trans_surface == nullptr) {
      std::cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    SDL_Texture *trans_texture =
        SDL_CreateTextureFromSurface(renderer_, trans_surface);
    SDL_FreeSurface(trans_surface);
    const SDL_Rect dst = {0, 0, screen_specifications["width"], screen_specifications["height"]};
    SDL_RenderCopy(renderer_, trans_texture, nullptr, &dst);
    SDL_DestroyTexture(trans_texture);
    if (blink_count_ < 30) {
      draw_text(font_size["x36"], rgb::white, Point{165, 170}, "P a u s e");
      ++blink_count_;
    } else if (blink_count_ < 60) {
      ++blink_count_;
    } else {
      blink_count_ = 0;
    }
  }

 public:
  Pacman(const bool fullscreen_mode, const bool debug_mode) 
      : fullscreen_mode_(fullscreen_mode),
        debug_mode_(debug_mode),
        window_(nullptr),
        renderer_(nullptr),
        game_state_(game_state::title),
        game_mode_(game_mode::single),
        blink_count_(0),
        game_count_(0),
        debug_lose_enemy_(false) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    Uint32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen_mode_) {
      flags |= SDL_WINDOW_FULLSCREEN;
    }
    window_ = SDL_CreateWindow("pacman-sdl", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, screen_specifications["width"],
                               screen_specifications["height"], flags);
    if (window_ == nullptr) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == nullptr) {
      std::cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    image_manager_ = std::make_unique<ImageManager>(renderer_);
    mixer_manager_ = std::make_unique<MixerManager>();
    input_manager_ = std::make_unique<InputManager>();
    wipe_ = std::make_unique<Wipe>(renderer_);
    map_ = std::make_unique<Maze>(renderer_, image_manager_.get());
    food_ = std::make_unique<Food>(image_manager_.get(), mixer_manager_.get());
    enemy_ =
        std::make_unique<Enemy>(image_manager_.get(), mixer_manager_.get());
    p1_ = std::make_unique<Player>(player_type["p1"], image_manager_.get(),
                                   input_manager_.get());
    p2_ = std::make_unique<Player>(player_type["p2"], image_manager_.get(),
                                   input_manager_.get());

    SDL_ShowCursor(SDL_DISABLE);
  }

  void run()  {
    while(true) {
      input_manager_->update();
      switch (game_state_) {
        case game_state::title:
          game_title();
          break;
        case game_state::start:
          game_start();
          break;
        case game_state::playing:
          play_game();
          break;
        case game_state::clear:
          game_clear();
          break;
        case game_state::miss:
          game_miss();
          break;
        case game_state::gameover:
          game_over();
          break;
        case game_state::pause:
          game_pause();
          break;
      }
      if (!poll_event()) {
        return;
      }
      if (debug_mode_) {
        draw_fps();
      }
      SDL_RenderPresent(renderer_);
      wait_game();
    }
  }

  ~Pacman()  {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    atexit(SDL_Quit);
  }
};