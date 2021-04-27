#pragma once

#include <SDL2/SDL.h>
#include <math.h>
#include <string>
#include<map>
using namespace std;

map<string,int> screen_specifications = {
    {"width",640},
    {"height",480},
    {"offset_x",480}
};

map<string,int> block = {
    {"size",20},
    {"count_x",24},
    {"count_y",24}
};

enum class game_mode {
  single,
  multiplayer
};


struct Point {
  int x;
  int y;

  bool operator==(const Point &p) const noexcept {
    return x == p.x && y == p.y;
  }

  bool operator!=(const Point &p) const noexcept {
    return x != p.x || y != p.y;
  }

  Point operator+(const Point &p) const noexcept {
    return Point{x + p.x, y + p.y};
  }

  void operator+=(const Point &p) noexcept {
    x += p.x;
    y += p.y;
  }

 public:
  int distance(const Point &p) const noexcept {
    return (int)math.sqrt( pow((x-p.x),2) + pow((y-p.y),2) );
  }
};


