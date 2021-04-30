#pragma once

#include <SDL2/SDL.h>
#include <math.h>

enum class game_mode {
  single,
  multiplayer,
};

struct Point {
  int x;
  int y;

   bool operator==(const Point &rhs) const  {
    return x == rhs.x && y == rhs.y;
  }

   bool operator!=(const Point &rhs) const  {
    return x != rhs.x || y != rhs.y;
  }

   Point operator+(const Point &rhs) const  {
    return Point{x + rhs.x, y + rhs.y};
  }

   void operator+=(const Point &rhs)  {
    x += rhs.x;
    y += rhs.y;
  }

 public:
      int distance(const Point &z) const {
        return (int)sqrt(pow(x-z.x,2)+pow(y-z.y,2));
    }
};
