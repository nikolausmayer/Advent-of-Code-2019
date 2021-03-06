
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include "intcode.hpp"

struct hash_pair {
  template <class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const
  {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T1>{}(p.first);
    return hash1 ^ hash2;
  }
};

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
      
  std::unordered_map<std::pair<int, int>, int, hash_pair> canvas;
  std::set<std::pair<int, int>> painted_tiles;
  std::vector<std::map<char, char>> turn_map{
    {{'N', 'W'}, 
     {'E', 'N'}, 
     {'S', 'E'}, 
     {'W', 'S'}},
    {{'N', 'E'}, 
     {'E', 'S'}, 
     {'S', 'W'}, 
     {'W', 'N'}}
  };
  std::map<char, int> move_x{
    {'N',  0},
    {'E',  1},
    {'S',  0},
    {'W', -1},
  };
  std::map<char, int> move_y{
    {'N', -1},
    {'E',  0},
    {'S',  1},
    {'W',  0},
  };
  Intcode robot{program};
  robot.setInput(0);
  Intcode::DATA_t paint_command{0}, turn_command{0};
  auto pos_x{0}, pos_y{0};
  auto robot_heading{'N'};
  for (auto status{robot.run()}; 
       status != Intcode::State::finished;
       status = robot.run()) {
    robot.getOutput(paint_command);
    robot.getOutput(turn_command);
    auto& pixel_color{canvas[{pos_x, pos_y}]};
    switch (paint_command) {
      case 0: pixel_color = 0; break;
      case 1: pixel_color = 1; break;
    }
    painted_tiles.insert({pos_x, pos_y});
    robot_heading = turn_map[turn_command][robot_heading];
    pos_x += move_x[robot_heading];
    pos_y += move_y[robot_heading];
    robot.setInput(canvas[{pos_x, pos_y}]);
  }
  std::cout << painted_tiles.size() << " tiles were painted."
            << std::endl;
}

