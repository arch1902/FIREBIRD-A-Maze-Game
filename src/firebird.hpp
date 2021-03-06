#pragma once

#include <time.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include "enemy.hpp"
#include "font.hpp"
#include "food.hpp"
#include "input.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"
#include "wipe.hpp"
#include "server.hpp"
#include "client.hpp"
#include "global.hpp"
#include "bullet.hpp"
extern bool music;

using namespace std;

struct RGB {
  Uint8 r; // 8 bit number 0-255
  Uint8 g;
  Uint8 b;
};

namespace rgb {
const RGB black = RGB{0x00, 0x00, 0x00};
const RGB red = RGB{0xff, 0x00, 0x00};
const RGB white = RGB{0xff, 0xff, 0xff};
};  // namespace rgb

class Firebird {
  enum class game_state {
    title,
    start,
    clear,
    miss,
    playing,
    gameover,
    pause,
    instructions,
  };

  const bool fullscreen_mode_;

  SDL_Window *window_;
  SDL_Renderer *renderer_;
  game_state game_state_;
  unsigned int game_level_;
  game_mode game_mode_;
  unsigned int blink_count_;
  unsigned int game_count_;

  unique_ptr<Bullet> p1_bullet;
  unique_ptr<Bullet> p2_bullet;
  unique_ptr<ImageManager> image_manager_;
  unique_ptr<MixerManager> mixer_manager_;
  unique_ptr<Wipe> wipe_;
  unique_ptr<InputManager> input_manager_;
  unique_ptr<Maze> maze_;
  unique_ptr<Food> food_;
  unique_ptr<Enemy> enemy_;
  unique_ptr<Player> p1_;
  unique_ptr<Player> p2_;
  FontManager font_manager_;

  void game_title() ;
  void game_start() ;
  void play_game() ;
  void game_clear() ;
  void game_miss() ;
  void game_over() ;
  void game_instructions();

   void game_pause()  {
    maze_->draw(game_level_);
    food_->draw();
    enemy_->draw();
    p1_->draw(game_mode_);
    p2_->draw(game_mode_);
    if (p1_bullet->present_){p1_bullet->draw();}
    if (p2_bullet->present_){p2_bullet->draw();}
    draw_score();
    draw_translucence();
    if (input_manager_->edge_key_p(0, 4)) {
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
    {
      SDL_Texture *p_texture = image_manager_->get(13);
      SDL_Rect dst;
      dst.x = 480;
      dst.y = 0;
      SDL_QueryTexture(p_texture, nullptr, nullptr, &dst.w, &dst.h);
      SDL_RenderCopy(renderer_, p_texture, nullptr, &dst);
      SDL_DestroyTexture(p_texture);
    }

    {
      const unsigned int x1 = 480 + 20;
      const unsigned int y1 = 480 / 7 + 10;
      const unsigned int x2 = x1 + 40;
      const unsigned int y2 = y1 + 30;
      const unsigned int x3 = x2 + 30;
      const unsigned int y3 = y2;

      stringstream score;
      score << "S c o r e  :  " << setw(6) << p1_->get_score();
      draw_text(1, rgb::white, Point{x1, y1}, score.str().c_str());

      SDL_Texture *p_texture = image_manager_->get(0);
      SDL_Rect src = {20, 0, 20, 20};
      SDL_Rect dst = {x2, y2, 20, 20};
      SDL_RenderCopy(renderer_, p_texture, &src, &dst);
      SDL_DestroyTexture(p_texture);

      stringstream life;
      life << "x  " << p1_->get_life();
      draw_text(1, rgb::white, Point{x3, y3}, life.str().c_str());

      if (game_mode_ == game_mode::multiplayer) {
        const unsigned int offset_y = 310;
        stringstream score;
        score << "S c o r e  :  " << setw(6) << p2_->get_score();
        draw_text(1, rgb::white, Point{x1, y1 + offset_y},
                  score.str().c_str());

        SDL_Texture *p_texture = image_manager_->get(1);
        const SDL_Rect src = {20, 0, 20, 20};
        const SDL_Rect dst = {x2, y2 + offset_y, 20, 20};
        SDL_RenderCopy(renderer_, p_texture, &src, &dst);
        SDL_DestroyTexture(p_texture);

        stringstream life;
        life << "x  " << p2_->get_life();
        draw_text(1, rgb::white, Point{x3, y3 + offset_y},
                  life.str().c_str());
      }
    }

    {
      const unsigned int x = 480 + 10;
      const unsigned int y = 480 / 6 * 4 + 80;
      if (p1_->get_power_mode()) {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
        const SDL_Rect dst = {
            x, y-260, static_cast<Uint16>(p1_->get_power_mode() / 4), 20};
        SDL_RenderFillRect(renderer_, &dst);
      }
      if (p2_->get_power_mode()) {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
        const SDL_Rect dst = {x, y + 50,
                              static_cast<Uint16>(p2_->get_power_mode() / 4),
                              20};
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
          break;
      }
    }
    return true;
  }

   void wait_game() const  {
    static Uint32 pre_count;
    const double wait_time = 1000.0 / 60;
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

      // stringstream ss;
      // ss << "FrameRate[" << setprecision(2)
      //    << setiosflags(ios::fixed) << frame_rate << "]";
      // draw_text(1, rgb::white, Point{480 + 15, 16},
      //           ss.str().c_str());
      cout<< "FrameRate +> "<<frame_rate<<endl;
    }
    
    pre_count = now_count;
  }

   void draw_translucence()  {
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
        SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32,
                             rmask, gmask, bmask, amask);
    if (trans_surface == nullptr) {
      cerr << "CreateRGBSurface failed: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }
    SDL_Texture *trans_texture =
        SDL_CreateTextureFromSurface(renderer_, trans_surface);
    SDL_FreeSurface(trans_surface);
    const SDL_Rect dst = {0, 0, 640, 480};
    SDL_RenderCopy(renderer_, trans_texture, nullptr, &dst);
    SDL_DestroyTexture(trans_texture);
    if (blink_count_ < 30) {
      draw_text(0, rgb::white, Point{165, 170}, "P a u s e");
      blink_count_++;
    } else if (blink_count_ < 60) {
      blink_count_++;
    } else {
      blink_count_ = 0;
    }
  }

 public:
  Firebird(const bool fullscreen_mode) 
      : fullscreen_mode_(fullscreen_mode),
        window_(nullptr),
        renderer_(nullptr),
        game_state_(game_state::title),
        game_mode_(game_mode::single),
        blink_count_(0),
        game_count_(0) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    Uint32 flags = SDL_WINDOW_SHOWN;
    if (fullscreen_mode_) {
      flags |= SDL_WINDOW_FULLSCREEN;
    }
    cout<<network_state<<endl;
    string t = "firebird-sdl_"+network_state;
    const char * s = t.c_str();
    cout<<network_state<<endl;
    window_ = SDL_CreateWindow(s, SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED, 640, 480, flags);
    if (window_ == nullptr) {
      cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (renderer_ == nullptr) {
      cerr << "error: " << SDL_GetError() << '\n';
      exit(EXIT_FAILURE);
    }

    image_manager_ = make_unique<ImageManager>(renderer_);
    mixer_manager_ = make_unique<MixerManager>();
    input_manager_ = make_unique<InputManager>();
    p1_bullet = make_unique<Bullet>(image_manager_.get());
    p2_bullet = make_unique<Bullet>(image_manager_.get());
    wipe_ = make_unique<Wipe>(renderer_);
    maze_ = make_unique<Maze>(renderer_, image_manager_.get());
    food_ = make_unique<Food>(image_manager_.get(), mixer_manager_.get());
    enemy_ = make_unique<Enemy>(image_manager_.get(), mixer_manager_.get());
    p1_ = make_unique<Player>(0, image_manager_.get(), input_manager_.get());
    p2_ = make_unique<Player>(1, image_manager_.get(), input_manager_.get());

    //SDL_ShowCursor(SDL_DISABLE);
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
        case game_state::instructions:
          game_instructions();
          break;
      }
      if (!poll_event()) {
        return;
      }
      SDL_RenderPresent(renderer_);
      wait_game();
      //draw_fps();
    }
  }

  ~Firebird()  {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    atexit(SDL_Quit);
  }
};
