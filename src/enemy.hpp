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
extern string network_state;
extern int socket_;

using namespace std;

class Enemy {
  public:
  enum class enemy_state {
    normal,
    lose,
  };

  struct Enemy_data {
    const int type;
    Point pos;
    Point block;
    Point next_block;
    int dir;           // max value is 4
    int anime_count;   // 0 or 1
    int anime_weight;  // max value is 8
    enemy_state state;

    Enemy_data(const unsigned char enemy_type) : type(enemy_type) {}
  };

  vector<Enemy_data> enemies_;
  const ImageManager *image_manager_;
  const MixerManager *mixer_manager_;

   void update()  {
    for (auto &enemy : enemies_) {
      enemy.anime_weight++;
      if (enemy.anime_weight >= 8) {
        enemy.anime_weight = 0;
        enemy.anime_count++;
        enemy.anime_count %= 2;
      }
    }
  }

  void move_normal_enemy(Enemy_data &enemy, const Maze &maze,int game_level, const Player &p1, const Player &p2) ;

  void move_lose_enemy(Enemy_data &enemy, const Maze &maze, const Player &p1, const Player &p2) ;

  Enemy(const ImageManager *image_manager,const MixerManager *mixer_manager) 
      : image_manager_(image_manager), mixer_manager_(mixer_manager) {
    enemies_.reserve(4);
    for (unsigned char i = 0; i < 4; ++i) {
      enemies_.push_back(Enemy_data(i));
    }
  }

   void init()  {
    //  cout<<"------------------------------------------"<<endl;
    //  cout<<network_state<<endl;
    const Point start_block[4] = {{11, 12}, {12, 12}, {11, 11}, {12, 11}};
    for (auto &enemy : enemies_) {
      enemy.pos = {20 * start_block[enemy.type].x, 20 * start_block[enemy.type].y};
      enemy.block = start_block[enemy.type];
      enemy.next_block = start_block[enemy.type];
      enemy.dir = 2;
      enemy.anime_count = 0;
      enemy.anime_weight = 0;
      enemy.state = enemy_state::normal;
    }
  }

   void draw() const  {
    // cout<<"------------------------------------------"<<endl;
    // cout<<network_state<<endl;
    SDL_Texture *enemies_texture[4];
    enemies_texture[0] = image_manager_->get(8);
    enemies_texture[1] = image_manager_->get(9);
    enemies_texture[2] = image_manager_->get(10);
    enemies_texture[3] = image_manager_->get(11);
    SDL_Texture *mon_run_texture = image_manager_->get(12);
    for (const auto &enemy : enemies_) {
      const SDL_Rect dst = {(enemy.pos.x),(enemy.pos.y),20,20};
      switch (enemy.state) {
        case enemy_state::normal: {
          const SDL_Rect src = {(20 * enemy.dir),(20 * enemy.anime_count),20,20};
          image_manager_->render_copy(*enemies_texture[enemy.type], src, dst);
          break;
        }
        case enemy_state::lose: {
          const SDL_Rect src = {0,(20 * enemy.anime_count),20,20};
          image_manager_->render_copy(*mon_run_texture, src, dst);
          break;
        }
      }
    }

    for (auto &t : enemies_texture) {
      SDL_DestroyTexture(t);
    }
    SDL_DestroyTexture(mon_run_texture);
  }

   void move( const Maze &maze,int game_level, const Player &p1, const Player &p2)  {
    for (auto &enemy : enemies_) {
      if ( enemy.state == enemy_state::lose) {
        move_lose_enemy(enemy, maze, p1, p2);
      } else {
        move_normal_enemy(enemy, maze,game_level, p1, p2);
      }
      // if (network_state == "server"){
      //   string s;
      //   s = to_string(enemy.pos.x) + "," + to_string(enemy.pos.y) +","+to_string(enemy.dir)+","+to_string(enemy.anime_count)+","+to_string(enemy.type);
      //   cout<<"Server Enemy Sending "<<s<<endl;
      //   send_from_server(s,socket_);
      //   string dump = receive_in_server(socket_);
      //   string if_received = receive_in_server(socket_);
      //   //cout<<dump<<endl;
      // }else{
      //   send_from_client("Come",socket_);
      //   string in = receive_in_client(socket_);
      //   send_from_client("Got it",socket_);
      //   cout<<"Cient Enemy Receive "<<in<<endl;
      //   vector<string> v;
      //   stringstream ss(in);
      //   while (ss.good()) {
      //       string substr;
      //       getline(ss, substr, ',');
      //       v.push_back(substr);
      //   }
      //   cout << (enemy.type == stoi(v[4]))<<endl;
      //   enemy.pos.x = stoi(v[0]);
      //   enemy.pos.y = stoi(v[1]);
      //   enemy.dir = stoi(v[2]);
      //   enemy.anime_count = stoi(v[3]);
      // }
    }
  }
  bool check_hit_enemy(const game_mode mode, Player &p1, Player &p2) ;

  ~Enemy()  {}
};
