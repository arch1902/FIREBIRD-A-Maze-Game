#include "firebird.hpp"
#include <SDL2/SDL_mixer.h>
#include <time.h>
#include <sstream>
#include "global.hpp"
#include "enemy.hpp"
#include "font.hpp"
#include "food.hpp"
#include "image.hpp"
#include "input.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"
#include "wipe.hpp"
#include "bullet.hpp"
extern string network_state;
int socket_;
bool music = true;
int c = 1;
int connection_number;
using namespace std;

void Firebird::game_title()  {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
    SDL_RenderClear(renderer_);

    const Point title_pos = Point{180, 160};
    const Point p1_mode_pos = Point{270, 300};
    const Point vs_mode_pos = Point{270, 350};
    const Point Ins = Point{220,215};
    const Point Mus = Point{215,245};
    const char *title_str = "F i r e  -  B i r d";
    const char *p1_mode_str = "1 Player";
    const char *vs_mode_str = "2 Player";
    const Point music_pos = Point{50,50};
    const SDL_Rect p1_str_dst = {250, 298, 112, 26};
    const SDL_Rect vs_str_dst = {250, 348, 112, 26};
    //cout<<network_state_<<endl;
    switch (game_count_) {
      case 0: {
        wipe_->set_wipe_in();
        wipe_->draw(640);
        game_count_++;
        break;
      }
      case 1: {
        draw_text(0, rgb::black, title_pos, title_str);
        wipe_->draw(640);
        if (wipe_->update()) {
          game_count_++;
        }
        break;
      }
      case 2: {
        draw_text(0, rgb::black, title_pos, title_str);
        if (blink_count_ < 30) {
          draw_text(1, rgb::black, Point{215, 300}, "P r e s s   S p a c e   K e y ");
          blink_count_++;
        } else if (blink_count_ < 60) {
          blink_count_++;
        } else {
          blink_count_=0;
        }

        if (input_manager_->edge_key_p(0, 4)) {
          game_count_++;
          blink_count_ = 0;
        }
        break;
      }
      case 3: {
        draw_text(0, rgb::black, title_pos, title_str);
        if (!input_manager_->press_key_p(0, 4)) {
          game_count_++;
        }
        break;
      }
      case 4: {
        draw_text(0, rgb::black, title_pos, title_str);
        if(music){
          draw_text(1, rgb::black, music_pos, "Music ON");
        }else{
          draw_text(1, rgb::black, music_pos, "Music OFF");
        }
        draw_text(1, rgb::black, Ins, "Press I for Instructions" );
        draw_text(1, rgb::black, Mus, "Press M for Music Toggle" );
        //draw_text(1, rgb::black, title_pos, "P l e a s e   W a i t");

        switch (game_mode_) {
          case game_mode::single: {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer_, &p1_str_dst);
            draw_text(1, rgb::white, p1_mode_pos, p1_mode_str);
            draw_text(1, rgb::black, vs_mode_pos, vs_mode_str);
            break;
          }
          case game_mode::multiplayer: {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer_, &vs_str_dst);
            draw_text(1, rgb::black, p1_mode_pos, p1_mode_str);
            draw_text(1, rgb::white, vs_mode_pos, vs_mode_str);
            break;
          }
        }

        if (input_manager_->press_key_p(0, 6)){
          game_state_ = game_state::instructions;
          game_count_ = 0;
        }
        if (input_manager_->press_key_p(0, 7)){
          if (music){
            music = false;
          }else{
            music = true;
          }
          SDL_Delay(200);
        }

        if ( input_manager_->press_key_p(0, 4)) {
            if (game_mode_ == game_mode::multiplayer){
              if(c == 2){      
                if (network_state=="server"){
                  socket_ = start_server();
                  connection_number ++;
                  cout<<"After starting"<<endl;
                }else if(network_state == "client"){
                  cout<<"Trying to connect"<<endl;
                  socket_ = connect_client();
                }else{
                  cout<<"Invalid Argument"<<endl;
                  exit(-1);
                }
                if (network_state=="server"){
                  send_from_server("hey_there",socket_);
                  cout<<"Before receive"<<endl;
                  
                  string xyz = receive_in_server(socket_);
                  cout<<"After receive"<<endl;
                  cout<<"Received "<<xyz<<endl;
                }else{
                  send_from_client("hello_server",socket_);
                  string abc = receive_in_client(socket_);
                  cout<<"Received "<<abc<<endl;
                }
                wipe_->set_wipe_out();
                wipe_->draw(640);
                game_count_++;
              }
              draw_text(1,rgb::red,{150, 400},"Please wait for the Player 2 to connect");
              c+=1;
            }
            if (game_mode_ == game_mode::single){
              wipe_->set_wipe_out();
              wipe_->draw(640);
              game_count_++;
            }

        }


        if (input_manager_->press_key_p(0, 1) || input_manager_->press_key_p(1, 1)) {
          game_mode_ = game_mode::multiplayer;
        } else if (input_manager_->press_key_p(0, 0) || input_manager_->press_key_p(1,0)) {
          game_mode_ = game_mode::single;
        }
        break;
      }
      case 5: {
        switch (game_mode_) {
          case game_mode::single: {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer_, &p1_str_dst);
            draw_text(1, rgb::white, p1_mode_pos, p1_mode_str);
            draw_text(1, rgb::black, vs_mode_pos, vs_mode_str);
            break;
          }
          case game_mode::multiplayer: {
            SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
            SDL_RenderFillRect(renderer_, &vs_str_dst);
            draw_text(1, rgb::black, p1_mode_pos, p1_mode_str);
            draw_text(1, rgb::white, vs_mode_pos, vs_mode_str);
            break;
          }
        }

        wipe_->draw(640);
        //cout<<network_state<<endl;
        // initialize globals
        if (wipe_->update()) {
          //cout<<"here1"<<endl;
          maze_->init(game_mode_);
          //cout<<"here2"<<endl;
          food_->init(*maze_);
          enemy_->init();
          p1_->init_pos();
          p2_->init_pos();
          p1_->set_life(2);
          p2_->set_life(2);
          p1_->set_score(0);
          p2_->set_score(0);
          p1_->set_damaged(false);
          p2_->set_damaged(false);
          p1_->set_power_mode(0);
          p2_->set_power_mode(0);

          game_count_ = 0;
          game_state_ = game_state::start;
          game_level_ = 1;

          srand((unsigned int)time(nullptr));
          if (music){
            Mix_PlayMusic(mixer_manager_->get_music(), -1);
          }
        }
        break;
      }
      default:
        break;
    }
}

void Firebird::game_start()  {
  maze_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();
  switch (game_count_) {
    case 0: {
      if (p1_->get_life() == 2 && p2_->get_life() == 2) {
        if(music){
          Mix_PlayChannel(1,mixer_manager_->get_sound(1),0);
        }
      }
      wipe_->set_wipe_in();
      wipe_->draw(480);
      game_count_++;
      break;
    }
    case 1: {
      wipe_->draw(480);
      if (wipe_->update()) {
        game_count_++;
      }
      break;
    }
    default:
      game_count_++;
      break;
  }
  if (game_count_ < 130) {
    stringstream ss;
    ss << "S t a g e " << game_level_;
    draw_text(0, rgb::red, Point{153, 170}, ss.str().c_str());
  } else if (game_count_ < 200) {
    draw_text(0, rgb::red, Point{165, 170}, "S t a r t");
  }

  if (game_count_ > 220) {
    game_count_ = 0;
    game_state_ = game_state::playing;
    p1_->set_power_mode(0);
    p2_->set_power_mode(0);
  }
}

void Firebird::play_game()  {
  maze_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();
  //cout<<"Going into moving enemy"<<endl;
  enemy_->move( *maze_,game_level_, *p1_, *p2_);
  p1_->move(*maze_, game_mode_,game_level_);
  p2_->move(*maze_, game_mode_,game_level_);

  if (network_state == "server"){
    if (p1_bullet->present_){
      //cout<<"Am about to draw the bullet"<<endl;
      p1_bullet->move(*maze_);
      //cout<<"Bullet P1"<<p1_bullet->pos_.x<<" , "<<p1_bullet->pos_.y<<endl;
      //cout<<"Pos P2"<<p2_->pos_.x<<" , "<<p2_->pos_.y<<endl;
    }
    if (!p1_bullet->present_ && input_manager_->edge_key_p(0, 5)){
      //cout<<"Player 1 fired a bullet"<<endl;
      //p1_->score_ = std::max(p1_->score_-10,0);
      p1_->score_ = (p1_->score_ >10) ? p1_->score_-10:0;
      if (music){
          Mix_PlayChannel(-1,mixer_manager_->get_sound(5), 0);
      }
      p1_bullet->present_ = true;
      p1_bullet->pos_ = p1_->pos_;
      p1_bullet->dir_ = p1_->dir_;
    }
  }else{
    if (p2_bullet->present_){
      p2_bullet->move(*maze_);
    }
    if (!p2_bullet->present_ && input_manager_->edge_key_p(1, 5)){
      //cout<<"Player 2 fired a bullet"<<endl;
      p2_->score_ = (p2_->score_ >10) ? p2_->score_-10:0;
      if (music){
          Mix_PlayChannel(-1,mixer_manager_->get_sound(5), 0);
      }
      p2_bullet->present_ = true;
      p2_bullet->pos_ = p2_->pos_;
      p2_bullet->dir_ = p2_->dir_;
    }
  }
  if (game_mode_ == game_mode::multiplayer){
    if (network_state == "server"){
      string s;
      string j = (p1_bullet->present_) ? "1":"0";
      for (auto &enemy : enemy_->enemies_) {
        s = s + to_string(enemy.pos.x) + "," + to_string(enemy.pos.y) +","+to_string(enemy.dir)+","+to_string(enemy.anime_count)+",";
      }
      s += to_string(p1_->get_pos().x)+","+to_string(p1_->get_pos().y)+","+to_string(p1_->get_dir())+","+to_string(p1_->get_count())+",";
      s += j+","+to_string(p1_bullet->pos_.x)+","+to_string(p1_bullet->pos_.y)+","+to_string(p1_bullet->dir_)+",";
      s += to_string(p1_->score_);
      send_from_server(s,socket_);
      //cout<<"Server Enemy Send "<<s<<endl;
      string dump = receive_in_server(socket_);
      vector<string> v;
      stringstream ss(dump);
      while (ss.good()) {
          string substr;
          getline(ss, substr, ',');
          v.push_back(substr);
      }
      p2_->set_pos({stoi(v[0]),stoi(v[1])});
      p2_->set_dir(stoi(v[2]));
      p2_->set_count(stoi(v[3]));
      p2_bullet->present_ = (stoi(v[4]) == 1) ? true :false;
      p2_bullet->pos_.x = stoi(v[5]);
      p2_bullet->pos_.y = stoi(v[6]);
      p2_bullet->dir_ = stoi(v[7]);
      p2_->score_ = stoi(v[8]);
    }else{
        string j = (p2_bullet->present_) ? "1":"0";
        string s = to_string(p2_->get_pos().x)+","+to_string(p2_->get_pos().y)+","+to_string(p2_->get_dir())+","+to_string(p2_->get_count())+",";
        s += j+","+to_string(p2_bullet->pos_.x)+","+to_string(p2_bullet->pos_.y)+","+to_string(p2_bullet->dir_)+",";
        s += to_string(p2_->score_);
        send_from_client(s,socket_);
        string in = receive_in_client(socket_);
        //cout<<"Cient Enemy Receive "<<in<<endl;
        vector<string> v;
        stringstream ss(in);
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');
            v.push_back(substr);
        }
        int i = 0;
        for(auto &enemy : enemy_->enemies_){
          enemy.pos.x = stoi(v[i++]);
          enemy.pos.y = stoi(v[i++]);
          enemy.dir = stoi(v[i++]);
          enemy.anime_count = stoi(v[i++]);
        }
        p1_->set_pos({stoi(v[i]),stoi(v[i+1])});
        p1_->set_dir(stoi(v[i+2]));
        p1_->set_count(stoi(v[i+3]));
        i = i+4;
        p1_bullet->present_ = (stoi(v[i++]) == 1) ? true :false;
        p1_bullet->pos_.x = stoi(v[i++]);
        p1_bullet->pos_.y = stoi(v[i++]);
        p1_bullet->dir_ = stoi(v[i++]);
        p1_->score_ = stoi(v[i++]);
    }
  }

  if (p1_bullet->present_){
    p1_bullet->draw();
    if (p1_bullet->pos_.distance(p2_->pos_)<=5 && game_mode_ == game_mode::multiplayer){
      p2_->set_damaged(true);
      game_state_ = game_state::miss;
      p1_->score_ += 100;
      p1_bullet->present_ = false;
    }
    for(auto &enemy : enemy_->enemies_){
      if(enemy.pos.distance(p1_bullet->pos_)<=5){
        //cout<<"Enemy pos "<<enemy.pos.x<<","<<enemy.pos.y<<endl;
        p1_->score_ += 100;
        enemy.state = Enemy::enemy_state::lose;
        if (music){
          Mix_PlayChannel(4,mixer_manager_->get_sound(4), 0);
        }
        p1_bullet->present_ = false;
      }
    }
  }
  if (p2_bullet->present_){
    p2_bullet->draw();
    if (p2_bullet->pos_.distance(p1_->pos_)<=5){
      p1_->set_damaged(true);
      game_state_ = game_state::miss;
      p2_bullet->present_ = false;
      p2_->score_ += 100;
    }
    for(auto &enemy : enemy_->enemies_){
      if(enemy.pos.distance(p2_bullet->pos_)<=5){
        //cout<<"Enemy pos "<<enemy.pos.x<<","<<enemy.pos.y<<endl;
        enemy.state = Enemy::enemy_state::lose;
        p2_->score_ += 100;
        if (music){
          Mix_PlayChannel(4,mixer_manager_->get_sound(4), 0);
        }
        p2_bullet->present_ = false;
      }
    }
  }


  if (p1_->get_power_mode()) {
    p1_->set_power_mode(p1_->get_power_mode() - 1);
  }
  if (p2_->get_power_mode()) {
    p2_->set_power_mode(p2_->get_power_mode() - 1);
  }

  const bool food_state = food_->check_state(game_mode_, *p1_, *p2_);
  const bool hit_enemy = enemy_->check_hit_enemy(game_mode_, *p1_, *p2_);

  if (food_state) {
    game_state_ = game_state::clear;
  } else if (hit_enemy) {
    game_state_ = game_state::miss;
  }
  if (input_manager_->edge_key_p(0, 4)) {
    game_state_ = game_state::pause;
  }

}

void Firebird::game_clear()  {
  maze_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();

  if (game_count_ == 0) {
    wipe_->set_wipe_out();
    wipe_->draw(480);
    game_count_++;
    return;
  }

  wipe_->draw(480);
  if (wipe_->update()) {
    if (game_level_ >= 256) {
      game_count_ = 0;
      game_state_ = game_state::gameover;
    } else {
      game_count_ = 0;
      game_state_ = game_state::start;
      game_level_++;
      food_->init(*maze_);
      enemy_->init();
      p1_->init_pos();
      p2_->init_pos();
    }
  }
}

void Firebird::game_miss()  {
  maze_->draw(game_level_);
  food_->draw();
  enemy_->draw();
  p1_->draw(game_mode_);
  p2_->draw(game_mode_);
  draw_score();
  p1_bullet->present_ = false;
  p2_bullet->present_ = false;

  if (game_count_ == 0) {
    if (music){
      Mix_HaltChannel(-1);
      Mix_PlayChannel(-1, mixer_manager_->get_sound(2), 0);
    }
    wipe_->set_wipe_out();
    if (p1_->get_life() == 0 || p2_->get_life() == 0) {
      wipe_->draw(640);
    } else {
      wipe_->draw(480);
    }
    game_count_++;
    return;
  }

  if (p1_->get_life() == 0 || p2_->get_life() == 0) {
    wipe_->draw(640);
  } else {
    wipe_->draw(480);
  }

  Player *p = p1_->get_damaged() ? p1_.get() : p2_.get();
  const Point pos = p->get_pos();
  p->set_pos(Point{pos.x, pos.y - 1});
  if (wipe_->update()) {
    const int life = p->get_life() - 1;
    p->set_life(life);
    if (life >= 0) {
      game_count_ = 0;
      game_state_ = game_state::start;
      enemy_->init();
      p1_->init_pos();
      p2_->init_pos();
      p1_->set_damaged(false);
      p2_->set_damaged(false);
    } else {
      game_count_ = 0;
      blink_count_ = 0;
      game_state_ = game_state::gameover;
    }
  }
}

void Firebird::game_instructions()  {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);

  const Point gameover_pos = Point{150, 20};
  const char *gameover_str = "Game Instructions";
  switch (game_count_) {
    case 0: {
      draw_text(0, rgb::red, gameover_pos, gameover_str);
      wipe_->set_wipe_in();
      wipe_->draw(640);
      game_count_++;
      break;
    }
    case 1: {
      draw_text(0, rgb::red, gameover_pos, gameover_str);
      wipe_->draw(640);
      if (wipe_->update()) {
        game_count_++;
      }
      break;
    }
    case 2: {
      draw_text(0, rgb::red, gameover_pos, gameover_str);
      //string ss;
      //ss = "Keys" ;
      draw_text(1, rgb::black, Point{80, 90}, "Keys");
      //ss = "Use up, down, left, right arrow keys to move";
      draw_text(1, rgb::black, Point{100, 120}, "-> Use up, down, left, right arrow keys to move.");
      //ss = "Use Left-Shift to shoot Enemies or Other Player";
      draw_text(1, rgb::black, Point{100, 150}, "-> Use Left-Shift to shoot Enemies or Other Player.");
      //ss = "Use space to pause the Game";  
      draw_text(1, rgb::black, Point{100, 180}, "-> Use space to pause the Game."); 
      draw_text(1, rgb::black, Point{80, 210}, "Scoring Criteria"); 
      draw_text(1, rgb::black, Point{100, 240}, "-> Eating food = 10 points & Firing a bullet = -10 points");
      draw_text(1, rgb::black, Point{100, 270}, "-> Killing or Shooting Enemy or Other Player = 100 points.");
      //draw_text(1, rgb::black, Point{100, 300}, "-> Use space to pause the Game"); 
      draw_text(1, rgb::black, Point{80, 300}, "Special Power");
      draw_text(1, rgb::black, Point{100, 330}, "-> Eating Apple makes you Immune to enemies for some time.");
      draw_text(1, rgb::black, Point{80, 360}, "Wining Criteria");
      draw_text(1, rgb::black, Point{100, 390}, "-> The player with highest score at end of game wins."); 
      //draw_text(1, rgb::black, Point{100, 390}, "Each player has three lives, upon ending the one with highest score wins"); 
      if (blink_count_ < 30) {
        draw_text(1, rgb::black, Point{210, 420}, "Press 'I' Key to Return");
        blink_count_++;
      } else if (blink_count_ < 60) {
        blink_count_++;
      } else {
        blink_count_ = 0;
      }

      if (input_manager_->press_key_p(0, 6)) {
        game_count_++;
        wipe_->set_wipe_out();
        wipe_->draw(640);
      }
      break;
    }
    case 3: {
      wipe_->draw(640);
      if (wipe_->update()) {
        blink_count_ = 0;
        game_count_ = 4;
        game_state_ = game_state::title;
        Mix_HaltMusic();
      }
      break;
    }
    default:
      break;
  }
}

void Firebird::game_over()  {
  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
  SDL_RenderClear(renderer_);

  const Point gameover_pos = Point{200, 100};
  const char *gameover_str = "Game Over";
  c = 1;
  switch (game_mode_) {
    case game_mode::single: {
      switch (game_count_) {
        case 0: {
          draw_text(0, rgb::red, gameover_pos, gameover_str);
          wipe_->set_wipe_in();
          wipe_->draw(640);
          game_count_++;
          break;
        }
        case 1: {
          draw_text(0, rgb::red, gameover_pos, gameover_str);
          wipe_->draw(640);
          if (wipe_->update()) {
            game_count_++;
          }
          break;
        }
        case 2: {
          draw_text(0, rgb::red, gameover_pos, gameover_str);
          stringstream ss;
          ss << "Y o u r  S c o r e   " << p1_->get_score();
          draw_text(0, rgb::black, Point{120, 220}, ss.str().c_str());

          if (blink_count_ < 30) {
            draw_text(1, rgb::black, Point{210, 350}, "P r e s s  S p a c e  K e y");
            blink_count_++;
          } else if (blink_count_ < 60) {
            blink_count_++;
          } else {
            blink_count_ = 0;
          }

          if (input_manager_->press_key_p(0, 4)) {
            game_count_++;
            wipe_->set_wipe_out();
            wipe_->draw(640);
          }
          break;
        }
        case 3: {
          wipe_->draw(640);
          if (wipe_->update()) {
            blink_count_ = 0;
            game_count_ = 0;
            game_state_ = game_state::title;
            Mix_HaltMusic();
          }
          break;
        }
        default:
          break;
      }
      break;
    }
    case game_mode::multiplayer: {
      switch (game_count_) {
        case 0: {
          draw_text(0, rgb::red, gameover_pos, gameover_str);
          wipe_->set_wipe_in();
          wipe_->draw(640);
          game_count_++;
          break;
        }
        case 1: {
          draw_text(0, rgb::red, gameover_pos, gameover_str);
          wipe_->draw(640);
          if (wipe_->update()) {
            game_count_++;
          }
          break;
        }
        case 2: {
          draw_text(0, rgb::red, gameover_pos, gameover_str);
          stringstream ss;
          const unsigned int p1_score = p1_->get_score();
          const unsigned int p2_score = p2_->get_score();
          if (p1_score > p2_score) {
            ss << "P 1  W I N S  : " << p1_score;
            draw_text(0, rgb::black, Point{170, 240},
                      ss.str().c_str());
          } else if (p1_score < p2_score) {
            ss << "P 2  W I N S  : " << p2_score;
            draw_text(0, rgb::black, Point{170, 240},
                      ss.str().c_str());
          } else {
            ss << "D R A W  " << p1_score;
            draw_text(0, rgb::black, Point{170, 240},
                      ss.str().c_str());
          }

          if (blink_count_ < 30) {
            draw_text(1, rgb::black, Point{210, 380}, "P r e s s  S p a c e  K e y");
            blink_count_++;
          } else if (blink_count_ < 60) {
            blink_count_++;
          } else {
            blink_count_ = 0;
          }

          if (input_manager_->press_key_p(0, 4)) {
            cout<<"here 1"<<endl;
            if (network_state == "server"){close_connection_server(socket_);}
            if (network_state == "client"){close_connection_client(socket_);}
            cout<<"here 11"<<endl;
            game_count_++;
            wipe_->set_wipe_out();
            wipe_->draw(640);
          }
          break;
        }
        case 3: {
          wipe_->draw(640);
          if (wipe_->update()) {
            blink_count_ = 0;
            game_count_ = 0;
            game_state_ = game_state::title;
          }
          break;
        }
        default:
          break;
      }
      break;
    }
  }
}
