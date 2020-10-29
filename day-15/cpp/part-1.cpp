
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

  auto print_map = [&] {
    int minx{999}, maxx{-999},
        miny{999}, maxy{-999};
    for (const auto& [key, _] : map) {
      minx = std::min(minx, key.first);
      miny = std::min(miny, key.second);
      maxx = std::max(maxx, key.first);
      maxy = std::max(maxy, key.second);
    }
    for (int y{miny}; y <= maxy; ++y) {
      for (int x{minx}; x <= maxx; ++x) {
        if (x == robot_x and y == robot_y) {
          std::cout << "O";
          continue;
        }
        switch (map[{x, y}]) {
          case 0:  std::cout << " "; break;
          case 1:  std::cout << "."; break;
          case 2:  std::cout << "#"; break;
          default: std::cout << "E";
        }
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
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
    //auto tx{0}, ty{0};
    for (const auto& [pos, plan] : tile_plans) {
      if (map[pos] == 0 and
          plan.size() > 0 and 
          plan.size() < min_plan_length) {
        min_plan_length = plan.size();
        next_dir = plan.top();
        //tx = pos.first;
        //ty = pos.second;
      }
    }
    //std::cout << "planning to go to (" << tx << "," << ty << ")" << std::endl;
    return next_dir;
  };


  for (;;) {
    //print_map();

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
        //std::cout << "Wall at (" << tmp_x << "," << tmp_y
        //          << ")" << std::endl;
        if (map[{tmp_x, tmp_y}] == 1) {
          throw std::runtime_error("WHAT");
        }
        map[{tmp_x, tmp_y}] = 2;
        tile_plans[{tmp_x, tmp_y}] = std::stack<int>{};
        break;
      }
      case 1: {
        move(next_step);
        map[{robot_x, robot_y}] = 1;
        tile_plans[{robot_x, robot_y}] = std::stack<int>{};
        //std::cout << "Moved to (" << robot_x << "," << robot_y
        //          << ")" << std::endl;
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
      case 2: {
        move(next_step);
        print_map();
        int total_path{0};
        while (tile_plans[{0, 0}].size()) {
          ++total_path;
          tile_plans[{0, 0}].pop();
        }
        std::cout << "Found oxygen system at ("
                  << robot_x << "," << robot_y << ")"
                  << std::endl;
        std::cout << "Shortest path is " << total_path-1
                  << std::endl;
        return EXIT_SUCCESS;
      }
    }
  }

}

