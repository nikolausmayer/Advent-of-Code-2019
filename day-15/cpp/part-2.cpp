
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <stdexcept>
#include <utility>
#include "intcode.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2) {
    throw std::runtime_error("Bad arguments");
  }
  std::ifstream infile{argv[1]};
  if (infile.bad() or not infile.is_open()) {
    throw std::runtime_error("Could not open file");
  }
  std::vector<Intcode::DATA_t> program;
  {
    auto filename = argv[1];
    std::ifstream infile{filename};
    if (infile.bad() or not infile.is_open()) {
      throw std::runtime_error("cannot open file");
    }
    Intcode::DATA_t opcode{0};
    auto negative{false};
    for (;;) {
      auto c{infile.get()};
      if (infile.eof()) {
         program.push_back(negative ? -opcode : opcode);
        break; 
      }
       if (c == ',' or c == '\n') {
         program.push_back(negative ? -opcode : opcode);
        opcode = 0;
        negative = false;
      } else if (c == '-') {
        negative = true;
      } else {
         opcode = 10*opcode + (c - 48);
      }
    }
  }

  const int N{1}, E{4}, S{2}, W{3};

  Intcode robot{program};
  int robot_x{0}, robot_y{0};

  std::map<std::pair<int, int>, int> map;
  std::map<std::pair<int, int>, std::stack<int>> tile_plans;
  map[{0, 0}] = 1;
  /// Special plan for origin
  tile_plans[{ 0,  0}].push(-1);
  /// Initial plans for unexplored tiles
  tile_plans[{-1,  0}].push(W);
  tile_plans[{ 1,  0}].push(E);
  tile_plans[{ 0, -1}].push(N);
  tile_plans[{ 0,  1}].push(S);

  auto update_plan = [&](int step, auto& plan) {
    if (plan.size() == 0)
      return;

    if (plan.top() == step) {
      plan.pop();
    } else {
      plan.push((step == N ? S :
                 step == S ? N :
                 step == W ? E :
                             W));
    }
  };

  auto move = [&](auto dir) {
    /// Move robot
    switch (dir) {
      case N: robot_y -= 1; break;
      case E: robot_x += 1; break;
      case S: robot_y += 1; break;
      case W: robot_x -= 1; break;
    }

    /// Update all tile plans
    for (auto& [pos, plan] : tile_plans) {
      update_plan(dir, plan);
    }
  };

  auto next_step_to_nearest_unexplored_tile = [&]{
    auto min_plan_length{99999u};
    auto next_dir{-1};
    for (const auto& [pos, plan] : tile_plans) {
      if (map[pos] == 0 and
          plan.size() > 0 and 
          plan.size() < min_plan_length) {
        min_plan_length = plan.size();
        next_dir = plan.top();
      }
    }
    return next_dir;
  };

  int oxy_x{0}, oxy_y{0};

  for (;;) {
    auto next_step = next_step_to_nearest_unexplored_tile();
    /// No unexplored tiles left?
    if (next_step == -1)
      break;
    robot.setInput(next_step);
    robot.run();
    Intcode::DATA_t reply{0};
    robot.getOutput(reply);


    switch (reply) {
      case 0: {
        auto tmp_x = (next_step == W ? robot_x - 1 :
                      next_step == E ? robot_x + 1 :
                                       robot_x);
        auto tmp_y = (next_step == N ? robot_y - 1 :
                      next_step == S ? robot_y + 1 :
                                       robot_y);
        map[{tmp_x, tmp_y}] = 2;
        tile_plans[{tmp_x, tmp_y}] = std::stack<int>{};
        break;
      }
      case 1: 
      case 2: {
        move(next_step);
        if (reply == 2) {
          oxy_x = robot_x;
          oxy_y = robot_y;
        }
        map[{robot_x, robot_y}] = 1;
        tile_plans[{robot_x, robot_y}] = std::stack<int>{};
        /// Seed new plans for adjacent tiles
        if (     map[{robot_x, robot_y - 1}] == 0) {
          tile_plans[{robot_x, robot_y - 1}] = std::stack<int>{};
          tile_plans[{robot_x, robot_y - 1}].push(N);
        }
        if (     map[{robot_x, robot_y + 1}] == 0) {
          tile_plans[{robot_x, robot_y + 1}] = std::stack<int>{};
          tile_plans[{robot_x, robot_y + 1}].push(S);
        }
        if (     map[{robot_x - 1, robot_y}] == 0) {
          tile_plans[{robot_x - 1, robot_y}] = std::stack<int>{};
          tile_plans[{robot_x - 1, robot_y}].push(W);
        }
        if (     map[{robot_x + 1, robot_y}] == 0) {
          tile_plans[{robot_x + 1, robot_y}] = std::stack<int>{};
          tile_plans[{robot_x + 1, robot_y}].push(E);
        }
        break;
      }
    }
  }


  map[{oxy_x, oxy_y}] = 3;
  auto timestep_counter{0};

  int minx{999}, maxx{-999},
      miny{999}, maxy{-999};
  for (const auto& [key, _] : map) {
    minx = std::min(minx, key.first);
    miny = std::min(miny, key.second);
    maxx = std::max(maxx, key.first);
    maxy = std::max(maxy, key.second);
  }

  for (;;) {
    auto map_copy{map};
    auto deltas{0};
    for (auto y{miny}; y <= maxy; ++y) {
      for (auto x{minx}; x <= maxx; ++x) {
        /// 0 is unknown, 1 is free, 2 is wall, 3 is oxygen
        if (map[{x, y}] != 3)
          continue;
        if (map[{x, y-1}] == 1) { map_copy[{x, y-1}] = 3; ++deltas; }
        if (map[{x, y+1}] == 1) { map_copy[{x, y+1}] = 3; ++deltas; }
        if (map[{x-1, y}] == 1) { map_copy[{x-1, y}] = 3; ++deltas; }
        if (map[{x+1, y}] == 1) { map_copy[{x+1, y}] = 3; ++deltas; }
      }
    }
    if (deltas == 0)
      break;
    map = map_copy;
    ++timestep_counter;
  }
  std::cout << timestep_counter << " time steps" << std::endl;
}

