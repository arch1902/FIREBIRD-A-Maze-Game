#include "enemy.hpp"
#include <SDL2/SDL_mixer.h>
#include <vector>
#include "global.hpp"
#include "image.hpp"
#include "maze.hpp"
#include "mixer.hpp"
#include "player.hpp"

void Enemy::move_normal_enemy(Enemy_data &enemy, const Maze &maze,
                              const Player &p1, const Player &p2)  {
  // TODO: change moving algorithm for each enemy.
  const Point dst_pos = {enemy.next_block.x * 20, enemy.next_block.y * 20};
  if (enemy.pos != dst_pos) {
    update();

    const unsigned int move_value = 2;
    if (dst_pos.x > enemy.pos.x) enemy.pos.x += move_value;
    else if (dst_pos.x < enemy.pos.x)  enemy.pos.x -= move_value;
    
    if (dst_pos.y > enemy.pos.y) enemy.pos.y += move_value;
    else if (dst_pos.y < enemy.pos.y) enemy.pos.y -= move_value;

    return;
  }

  enemy.block = enemy.next_block;
  if (maze.check_state(enemy.block) == maze_state::enemy_house) {
    enemy.dir = 2;
    enemy.next_block.y--;
    return;
  }

  const Point front_pos[4] = {{0, 1}, {-1, 0}, {0, -1}, {1, 0}};
  const Point left_pos[4] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};
  const Point back_pos[4] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
  const Point right_pos[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

  const Point front_block = enemy.block + front_pos[enemy.dir];
  const maze_state front_block_state = maze.check_state(front_block);

  const bool move_to_front_block_p =
       front_block_state == maze_state::init_p1_pos
    || front_block_state == maze_state::init_p2_pos
    || front_block_state == maze_state::food
    || front_block_state == maze_state::counter_food
    || front_block_state == maze_state::left_warp_pos
    || front_block_state == maze_state::right_warp_pos
    || front_block_state == maze_state::warp_street;

  const Point left_block = enemy.block + left_pos[enemy.dir];
  const maze_state left_block_state = maze.check_state(left_block);

  const bool move_to_left_block_p =
       left_block_state == maze_state::food
    || left_block_state == maze_state::init_p1_pos
    || left_block_state == maze_state::init_p2_pos
    || left_block_state == maze_state::counter_food;

  const Point right_block = enemy.block + right_pos[enemy.dir];
  const maze_state right_block_state = maze.check_state(right_block);

  const bool move_to_right_block_p =
       right_block_state == maze_state::food
    || right_block_state == maze_state::init_p1_pos
    || right_block_state == maze_state::init_p2_pos
    || right_block_state == maze_state::counter_food;

  // move back at random

  if (rand() % 100 == 0
      || (390 <= p1.get_power_mode() && p1.get_power_mode() <= 400)
      || (390 <= p2.get_power_mode() && p2.get_power_mode() <= 400)) {
    if (maze.check_state(enemy.block + back_pos[enemy.dir])
        == maze_state::enemy_house) {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    } else {
      enemy.next_block += back_pos[enemy.dir];
      enemy.dir += 2;
      enemy.dir %= 4;
    }
    return;
  }

  // move front/left/right at random

  if (move_to_front_block_p && move_to_left_block_p && move_to_right_block_p) {
    const unsigned int next = rand() % 3;
    if (next == 0) {
      enemy.next_block = front_block;
    } else if (next == 1) {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    } else {
      enemy.next_block = right_block;
      ++enemy.dir;
      enemy.dir %= 4;
    }
    return;
  }

  // move front/left at random
  if (move_to_front_block_p && move_to_left_block_p && !move_to_right_block_p) {
    const unsigned int next = rand() % 2;
    if (next == 0) {
      enemy.next_block = front_block;
    } else {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    }
    return;
  }

  // move front/right at random
  if (move_to_front_block_p && !move_to_left_block_p && move_to_right_block_p) {
    const unsigned int next = rand() % 2;
    if (next == 0) {
      enemy.next_block = front_block;
    } else {
      enemy.next_block = right_block;
      ++enemy.dir;
      enemy.dir %= 4;
    }
    return;
  }

  // move front
  if (move_to_front_block_p && !move_to_left_block_p
      && !move_to_right_block_p) {
    enemy.next_block = front_block;
    return;
  }

  // move left/right at random
  if (!move_to_front_block_p && move_to_left_block_p
      && move_to_right_block_p) {
    const unsigned int next = rand() % 2;
    if (next == 0) {
      enemy.next_block = left_block;
      enemy.dir += 3;
      enemy.dir %= 4;
    } else {
      enemy.next_block = right_block;
      ++enemy.dir;
      enemy.dir %= 4;
    }
    return;
  }

  // move left
  if (!move_to_front_block_p && move_to_left_block_p
      && !move_to_right_block_p) {
    enemy.next_block = left_block;
    enemy.dir += 3;
    enemy.dir %= 4;
    return;
  }

  // move right
  if (!move_to_front_block_p && !move_to_left_block_p
      && move_to_right_block_p) {
    enemy.next_block = right_block;
    ++enemy.dir;
    enemy.dir %= 4;
    return;
  }

  // move back
  if (!move_to_front_block_p && !move_to_left_block_p &&
      !move_to_right_block_p) {
    enemy.next_block += back_pos[enemy.dir];
    enemy.dir += 2;
    enemy.dir %= 4;
    return;
  }

  // NOTREACHED
}

void Enemy::move_lose_enemy(Enemy_data &enemy, const Maze &maze, const Player &p1,
                            const Player &p2)  {
  if (p1.get_power_mode() == 0 && p2.get_power_mode() == 0) {
    enemy.state = enemy_state::normal;
  }

  const Point dst_pos = {enemy.next_block.x * 20,
                         enemy.next_block.y * 20};
  if (enemy.pos != dst_pos) {
    update();

    const int move_value = 2;
    if (dst_pos.x > enemy.pos.x) enemy.pos.x += move_value;
    else if (dst_pos.x < enemy.pos.x) enemy.pos.x -= move_value;

    if (dst_pos.y > enemy.pos.y) enemy.pos.y += move_value;
    else if (dst_pos.y < enemy.pos.y) enemy.pos.y -= move_value;

    return;
  }

  enemy.block = enemy.next_block;
  const unsigned int now_value = maze.get_home_distance(enemy.block);
  int x_ = enemy.block.x;
  int y_ = enemy.block.y;
  if (now_value > maze.get_home_distance(Point{x_, y_ - 1})) {
    enemy.next_block.y--;
    return;
  }

  if (now_value > maze.get_home_distance(Point{x_ - 1, y_})) {
    enemy.next_block.x--;
    return;
  }

  if (now_value > maze.get_home_distance(Point{x_ + 1, y_})) {
    enemy.next_block.x++;
    return;
  }

  if (now_value > maze.get_home_distance(Point{x_, y_ + 1})) {
    enemy.next_block.y++;
    return;
  }

}

bool Enemy::check_hit_enemy(const game_mode mode, Player &p1,
                            Player &p2)  {
  const unsigned int Hit_distance = 20 / 2;

  const Point pos = p1.get_pos();
  for (auto &enemy : enemies_) {
    const unsigned int d = pos.distance(enemy.pos);
    if (d < Hit_distance) {
      if (p1.get_power_mode() == 0) {
        p1.set_damaged(true);
        return true;
      }
      if (enemy.state == enemy_state::normal) {
        Mix_PlayChannel(4,
                        mixer_manager_->get_se(4), 0);
        p1.set_score(p1.get_score() + 100);
      }
      enemy.state = enemy_state::lose;
    }
  }

  if (mode == game_mode::multiplayer) {
    const Point pos = p2.get_pos();
    for (auto &enemy : enemies_) {
      const unsigned int d = pos.distance(enemy.pos);
      if (d < Hit_distance) {
        if (p2.get_power_mode() == 0) {
          p2.set_damaged(true);
          return true;
        }
        if (enemy.state == enemy_state::normal) {
          Mix_PlayChannel(4,
                          mixer_manager_->get_se(4), 0);
          p2.set_score(p2.get_score() + 100);
        }
        enemy.state = enemy_state::lose;
      }
    }
  }

  return false;
}
