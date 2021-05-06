#pragma once

#include <SDL2/SDL.h>
#include <math.h>
#include <string>
#include<map>
using namespace std;

static map<string,const int> screen_specifications = {
    {"width",640},
    {"height",480},
    {"offset_x",480}
};

static map<string,int> block = {
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

  bool operator==(const Point &p) const  {
    return x == p.x && y == p.y;
  }

  bool operator!=(const Point &p) const  {
    return x != p.x || y != p.y;
  }

  Point operator+(const Point &p) const  {
    return Point{x + p.x, y + p.y};
  }

  void operator+=(const Point &p)  {
    x += p.x;
    y += p.y;
  }

 public:
  int distance(const Point &p) const  {
    int r = sqrt(pow((x-p.x),2) + pow((y-p.y),2));
    return r;
  }
};


