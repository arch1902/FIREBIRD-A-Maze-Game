#pragma once

#include <SDL2/SDL.h>
#include <math.h>
#include <string>

enum class game_mode {
  single,
  multiplayer,
};

struct Point {
  int x;
  int y;

   bool operator==(const Point &point) const  {
    return x == point.x && y == point.y;
  }
  
   bool operator!=(const Point &point) const  {
    return x != point.x || y != point.y;
  }

   Point operator+(const Point &point) const  {
    return Point{x + point.x, y + point.y};
  }

   void operator+=(const Point &point)  {
    x += point.x;
    y += point.y;
  }

 public:
      int distance(const Point &z) const {
        return (int)sqrt(pow(x-z.x,2)+pow(y-z.y,2));
    }
};
