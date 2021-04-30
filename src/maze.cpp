#include "maze.hpp"
#include <math.h>
#include <stdlib.h>
#include <string>
#include <queue>
#include "global.hpp"
#include "image.hpp"

maze_state char_to_maze_state(const char c) {
  switch (c) {
    case '#':
      return maze_state::block;
    case '.':
      return maze_state::food;
    case 'C':
      return maze_state::counter_food;
    case 'E':
      return maze_state::enemy_house;
    case 'P':
      return maze_state::init_p1_pos;
    case 'L':
      return maze_state::left_warp_pos;
    case 'R':
      return maze_state::right_warp_pos;
    case '=':
      return maze_state::warp_street;
    default:
      std::cerr << "error: undefined character of maze" << '\n';
      exit(EXIT_FAILURE);
  }
}

void Maze::init(const game_mode mode)  {
  const std::string block_src =
      "########################"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#C##.#####.##.#####.##C#"
      "#.##.#####.##.#####.##.#"
      "#......................#"
      "#.##.##.########.##.##.#"
      "#.##.##....##....##.##.#"
      "#....#####.##.#####....#"
      "####.#####.##.#####.####"
      "####.##..........##.####"
      "####.##.###EE###.##.####"
      "L===....##EEEE##....===R"
      "####.##.########.##.####"
      "####.##..........##.####"
      "####.##.########.##.####"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#C.#.....P..........#.C#"
      "##.#.##.########.##.#.##"
      "#....##....##....##....#"
      "#.########.##.########.#"
      "#......................#"
      "########################";

  for (unsigned int y = 0; y < 24; ++y) {
    for (unsigned int x = 0; x < 24; ++x) {
      block_[y][x] = char_to_maze_state(block_src[y * 24 + x]);
    }
  }

  if (mode == game_mode::multiplayer) {
    block_[18][14] = maze_state::init_p2_pos;
  }

  // '.' : enemy can reach, '#' : enemy cannot reach
  const std::string enemy_reaches_block_p =
      "########################"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#.##.#####.##.#####.##.#"
      "#.##.#####.##.#####.##.#"
      "#......................#"
      "#.##.##.########.##.##.#"
      "#.##.##....##....##.##.#"
      "#....#####.##.#####....#"
      "####.#####.##.#####.####"
      "####.##..........##.####"
      "####.##.###.####.##.####"
      "........###.####........"
      "####.##.########.##.####"
      "####.##..........##.####"
      "####.##.########.##.####"
      "#..........##..........#"
      "#.##.#####.##.#####.##.#"
      "#..#................#..#"
      "##.#.##.########.##.#.##"
      "#....##....##....##....#"
      "#.########.##.########.#"
      "#......................#"
      "########################";

  // Max value which doesn't appear in the distance maze.
  const unsigned int maze_max_num = 10000;

  // BFS
  bool reachedp[24][24];
  for (unsigned int y = 0; y < 24; ++y) {
    for (unsigned int x = 0; x < 24; ++x) {
      home_distance_[y][x] = maze_max_num;
      reachedp[y][x] = enemy_reaches_block_p[y * 24 + x] == '#';
    }
  }
  const Point losed = Point{11, 12};  // Point where a losed enemy is gone.
  const Point dirs[4] = {
      {0, -1}, {-1, 0}, {0, 1}, {1, 0}};  // down, left, up, right
  home_distance_[losed.y][losed.x] = 1;
  reachedp[losed.y][losed.x] = true;
  std::queue<Point> que;
  que.push(losed);
  while (!que.empty()) {
    const Point p = que.front();
    que.pop();
    const unsigned int d = home_distance_[p.y][p.x] + 1;
    for (const Point &dir : dirs) {
      const Point e = p + dir;
      if (e.x < 0 || e.x >= 24 || e.y < 0 || e.y >= 24
          || reachedp[e.y][e.x]) {
        continue;
      }
      reachedp[e.y][e.x] = true;
      home_distance_[e.y][e.x] = d;
      que.push(e);
    }
  }
}

void Maze::draw(const unsigned int game_level) const  {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);

  SDL_Texture *p_texture = nullptr;
  const unsigned int mod = game_level % 4;
  if (mod == 1) {
    p_texture = image_manager_->get(2);
  } else if (mod == 2) {
    p_texture = image_manager_->get(3);
  } else if (mod == 3) {
    p_texture = image_manager_->get(4);
  } else {
    p_texture = image_manager_->get(5);
  }

  {
    const SDL_Rect src[2] = {{0, 0, 20, 20},
                             {20, 0, 20, 20}};
    for (unsigned int y = 0; y < 24; ++y) {
      for (unsigned int x = 0; x < 24; ++x) {
        const SDL_Rect dst = {static_cast<Sint16>(20 * x),
                              static_cast<Sint16>(20 * y), 20,
                              20};
        switch (block_[y][x]) {
          case maze_state::food:
          case maze_state::counter_food:
          case maze_state::enemy_house:
          case maze_state::init_p1_pos:
          case maze_state::init_p2_pos:
          case maze_state::left_warp_pos:
          case maze_state::right_warp_pos:
          case maze_state::warp_street:
            image_manager_->render_copy(*p_texture, src[0], dst);
            break;
          case maze_state::block:
            image_manager_->render_copy(*p_texture, src[1], dst);
            break;
        }
      }
    }
  }
  {
    const SDL_Rect src = {20, 20, 20,
                          20 / 2};
    for (unsigned int y = 0; y < 24 - 1; ++y) {
      for (unsigned int x = 0; x < 24; ++x) {
        const maze_state block = block_[y][x];
        maze_state mut_under_block = block_[y + 1][x];
        switch (mut_under_block) {
          case maze_state::counter_food:
          case maze_state::enemy_house:
          case maze_state::init_p1_pos:
          case maze_state::init_p2_pos:
          case maze_state::left_warp_pos:
          case maze_state::right_warp_pos:
          case maze_state::warp_street:
            mut_under_block = maze_state::food;
            break;
          default:
            // do nothing
            break;
        }
        const maze_state under_block = mut_under_block;
        if ((block == maze_state::block) && (under_block == maze_state::food)) {
          const SDL_Rect dst = {static_cast<Sint16>(20 * x),
                                static_cast<Sint16>(20 * y + 20 / 2),
                                20,
                                20};
          image_manager_->render_copy(*p_texture, src, dst);
        }
      }
    }
  }
  SDL_DestroyTexture(p_texture);
}
